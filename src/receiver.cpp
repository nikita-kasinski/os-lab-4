#include <iostream>
#include <string>
#include <fstream>
#include "controller.h"
#include "windows.h"

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

    for (size_t i = 0; i < numberOfSenders; ++i)
    {
        CloseHandle(sendMessageEvents[i]);
        CloseHandle(senders[i].hThread);
        CloseHandle(senders[i].hProcess);
    }
    delete[] sendMessageEvents;
    delete[] senders;
    return 0;
}