#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "controller.h"
#include "windows.h"

BOOL StartSender(const std::string& binaryFile, PROCESS_INFORMATION& pi, size_t id)
{
    STARTUPINFO si;
    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);

    std::ostringstream consoleCommand;
    std::string sender = "sender.exe";
    consoleCommand << sender << " " << binaryFile << " " << id;
    return CreateProcessA(NULL, const_cast<char*>(consoleCommand.str().c_str()), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
}

int main()
{
    std::string binaryFile;
    size_t maxMessageCount;
    size_t numberOfSenders;

    std::cout << "Input binary file name: ";
    std::cin >> binaryFile;

    {
        // retreiving maxMessageCount
        std::string maxMessageCountPrompt = "Enter maximum message count: ";
        std::string maxMessageCountFailedPrompt = "Input must be positive integer. Try again\n";
        maxMessageCount = Controller::safeUnsignedIntegerInput(std::cin, std::cout, maxMessageCountPrompt, maxMessageCountFailedPrompt);
    }

    Controller::initBinaryFile(binaryFile, maxMessageCount);

    {
        // retreiving number of Senders
        std::string numberOfSendersPrompt = "Enter number of sender processes: ";
        std::string numberOfSendersFailedPrompt = "Input must be positive integer. Try again\n";
        numberOfSenders = Controller::safeUnsignedIntegerInput(std::cin, std::cout, numberOfSendersPrompt, numberOfSendersFailedPrompt);
    }

    HANDLE *sendMessageEvents = new HANDLE[numberOfSenders];
    PROCESS_INFORMATION *senders = new PROCESS_INFORMATION[numberOfSenders];
    HANDLE readEvent = CreateEventA(NULL, TRUE, FALSE, "Got message");
    HANDLE iomtx = CreateMutexA(NULL, FALSE, "IO mutex");
    HANDLE fmtx = CreateMutexA(NULL, FALSE, "File mutex");

    for (size_t i = 0; i < numberOfSenders; ++i)
    {
        std::ostringstream eventName;
        eventName << "Sent message " << i;
        sendMessageEvents[i] = CreateEventA(NULL, TRUE, FALSE, eventName.str().c_str());
        if (!StartSender(binaryFile, senders[i], i))
        {
            std::cerr << "Error " << GetLastError() << "\n";
            std::cerr << "Unable to create sender " << i << ". Abort\n";
            return 0;
        }
    }

    WaitForMultipleObjects(numberOfSenders, sendMessageEvents, TRUE, INFINITE); // wait for all senders for the first time

    std::string menuPrompt = "Enter respective symbol:\nq to quit\nr to read message\n";
    std::string receiverPrompt = "Receiver";
    std::string response;
    Controller ctrl(binaryFile);

    while(true)
    {
        std::cout << receiverPrompt << ". " << menuPrompt;
        std::cin >> response;
        if (response == "q")
        {
            break;
        }
        else if (response == "r")
        {
            std::cout << receiverPrompt << " is waiting for file to become available\n";
            WaitForSingleObject(fmtx, INFINITE);
            // safe work with file

            std::string messageRead;
            if (ctrl.getMessage(messageRead))
            {
                ReleaseMutex(fmtx);
                std::cout << receiverPrompt << " read message: " << messageRead << "\n";
                SetEvent(readEvent);
                for (size_t i = 0; i < numberOfSenders; ++i)
                {
                    ResetEvent(sendMessageEvents[i]);
                }
            }
            else
            {
                ReleaseMutex(fmtx);
                std::cout << receiverPrompt << " failed to read message as file is empty\n";
                std::cout << receiverPrompt << " is waiting for message to appear in file\n";
                WaitForMultipleObjects(numberOfSenders, sendMessageEvents, FALSE, INFINITE);
            }
        }
        else 
        {
            std::cout << "Unknown option " << response << ". Try again\n";
        }
        std::cout << "\n";
    }

    for (size_t i = 0; i < numberOfSenders; ++i)
    {
        CloseHandle(sendMessageEvents[i]);
        CloseHandle(senders[i].hThread);
        CloseHandle(senders[i].hProcess);
    }
    CloseHandle(iomtx);
    CloseHandle(fmtx);
    delete[] sendMessageEvents;
    delete[] senders;
    return 0;
}