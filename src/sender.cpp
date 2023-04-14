#include <iostream>
#include <windows.h>
#include <sstream>

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        std::cerr << "There must be exactly two args: binary file name and id\n";
        return 1;
    }
    std::string binaryFile = argv[1];
    size_t id = static_cast<size_t>(atoi(argv[2]));
    std::ostringstream eventName;
    eventName << "Sent message " << id;
    HANDLE sendEvent = OpenEventA(EVENT_MODIFY_STATE, FALSE, eventName.str().c_str());
    HANDLE readEvent = OpenEventA(EVENT_MODIFY_STATE, FALSE, "Got message");
    HANDLE iomtx = OpenMutexA(MUTEX_MODIFY_STATE, FALSE, "IO mutex");
    HANDLE fmtx = OpenMutexA(MUTEX_MODIFY_STATE, FALSE, "File mutex");
    
    std::string MenuPrompt = "Enter respective symbol:\nq to quit\n m to message the receiver\n";
    while(true)
    {
        std::cout << MenuPrompt;
        SetEvent(sendEvent);
        
    }
    return 0;
}