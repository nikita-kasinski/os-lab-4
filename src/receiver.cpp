#include <iostream>
#include <string>
#include <fstream>
#include <limits>
#include "controller.h"

int main()
{
    std::string binaryFile;
    int maxMessageCount = -1;
    constexpr auto maxStreamSize = std::numeric_limits<std::streamsize>::max();

    std::cout << "Input binary file name: ";
    std::cin >> binaryFile;

    {
        // retreiving maxMessageCount
        std::string maxMessageCountPrompt = "Enter maximum message count: ";
        std::string maxMessageCountFailedPrompt = "Input must be positive integer. Try again\n";

        bool failed = false;
        while (maxMessageCount <= 0)
        {
            if (failed)
            {
                std::cout << maxMessageCountFailedPrompt;
            }
            else
            {
                failed = true;
            }
            std::cout << maxMessageCountPrompt;
            std::cin >> maxMessageCount;
            if (!std::cin.good())
            {
                std::cin.clear();
                std::cin.ignore(maxStreamSize, '\n');
                maxMessageCount  = -1;
            }
        }
    }
    return 0;
}