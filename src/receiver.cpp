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
        maxMessageCount = Controller::safeUnsignedIntegerInput(std::cin, maxMessageCountPrompt, maxMessageCountFailedPrompt);
    }

    Controller::initBinaryFile(binaryFile, maxMessageCount);

    {
        // retreiving number of Senders
        std::string numberOfSendersPrompt = "Enter number of sender processes: ";
        std::string numberOfSendersFailedPrompt = "Input must be positive integer. Try again\n";
        numberOfSenders = Controller::safeUnsignedIntegerInput(std::cin, numberOfSendersPrompt, numberOfSendersFailedPrompt);
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