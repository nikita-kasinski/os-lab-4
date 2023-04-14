#include <iostream>
#include <windows.h>
#include <sstream>
#include "controller.h"

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        std::cerr << "There must be exactly two args: binary file name and id\n";
        return 1;
    }
    std::string binaryFile = argv[1];
    size_t id = static_cast<size_t>(atoi(argv[2]));

    Controller ctrl(binaryFile);

    std::ostringstream eventName;
    eventName << "Sent message " << id;
    HANDLE sendEvent = OpenEventA(EVENT_MODIFY_STATE, FALSE, eventName.str().c_str());
    HANDLE readEvent = OpenEventA(EVENT_MODIFY_STATE, FALSE, "Got message");
    HANDLE iomtx = OpenMutexA(MUTEX_MODIFY_STATE, FALSE, "IO mutex");
    HANDLE fmtx = OpenMutexA(MUTEX_MODIFY_STATE, FALSE, "File mutex");
    
    std::string menuPrompt = "Enter respective symbol:\nq to quit\n m to message the receiver\n";
    std::string senderPrompt = "Sender ";
    std::string messagePrompt = "Enter message: ";
    std::string response;

    while(true)
    {
        std::cout << senderPrompt << id << "." << menuPrompt;
        std::cin >> response;
        if (response == "q")
        {
            break;
        }
        else if (response == "m")
        {
            std::string message;

            std::cout << messagePrompt;
            std::cin >> message;
            if (message.length() > 20)
            {
                std::cout << "Message must be no longer than 20. Skipping\n";
            }
            std::cout << senderPrompt << id << " is waiting for the file to become available\n";
            WaitForSingleObject(fmtx, INFINITE);
            // now we can work with the file safely
            if (ctrl.postMessage(message))
            {
                // message posted successfully
                std::cout << "Message posted successfully\n";
                SetEvent(sendEvent);
            }
            else
            {
                std::cout << "Message wasn't posted as file is full.\n";
                ResetEvent(readEvent);
                std::cout << "Waiting for message to be read\n";
                WaitForSingleObject(readEvent, INFINITE); // waiting for message to be read
            }
            ReleaseMutex(fmtx);
        }
        else
        {
            std::cout << "Unknown option: " << response << ". Try again\n";
        }
    }
    return 0;
}