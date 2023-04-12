#pragma once
#include <string> 

class Controller
{
private:
    std::string binaryFileName;
    size_t maxMessageCount;
    static const size_t maxMessageSize = 20;
    static const char dummyChar = '0';
    static const size_t atomicOffset = sizeof(size_t);

    void movePointer(size_t& pointer) const;
    size_t getHead() const;
    size_t getTail() const;
public:
    Controller(const std::string& binaryFileName, size_t messageCount);
    bool postMessage(const std::string& message) const;
    std::string getMessage() const;
    static void initBinaryFile(const std::string& binaryFileName, size_t maxMessageCount);
};