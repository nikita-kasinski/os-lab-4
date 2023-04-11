#pragma once
#include <string> 

class Controller
{
private:
    std::string binaryFileName;
    size_t messageHead;
    size_t messageTail;
    size_t maxMessageCount;
    
    void movePointer(size_t& pointer) const;
public:
    Controller(const std::string& binaryFileName, size_t messageCount);
    bool postMessage(const std::string& message) const;
    std::string getMessage() const;
    static void initBinaryFile(const std::string& binaryFileName, size_t maxMessageCount);
};