#pragma once
#include <string> 

class Controller
{
private:
    std::string binaryFileName;
    int messageHead;
    int messageTail;
    int messageCount;
    void movePointer(int& pointer) const;
public:
    Controller(const std::string& binaryFileName, int messageCount);
    bool postMessage(const std::string& message) const;
    std::string getMessage() const;
};