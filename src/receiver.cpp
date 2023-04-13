#include <iostream>
#include <string>
#include <fstream>
#include "controller.h"

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

    Controller::initBinaryFile(binaryFile, static_cast<size_t>(maxMessageCount));

    {
        // retreiving number of Senders

        std::string numberOfSendersPrompt = "Enter number of sender processes: ";
        std::string numberOfSendersFailedPrompt = "Input must be positive integer. Try adain\n";
        numberOfSenders = Controller::safeUnsignedIntegerInput(std::cin, numberOfSendersPrompt, numberOfSendersFailedPrompt);
    }

    return 0;
}