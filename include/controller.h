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
    static const size_t atomicSize = atomicOffset;
    static const size_t headOffset = 0;
    static const size_t tailOffset = atomicOffset;
    static const size_t messageCountOffset = 2 * atomicOffset;
    static const size_t overallOffset = 3 * atomicOffset;

    void movePointer(size_t& pointer) const;

    //first - head, second - tail
    size_t getHead(std::fstream& fin) const;

    //first - head, second - tail
    size_t getTail(std::fstream& fin) const;

    size_t getMessageCount(std::fstream& fin) const;

    void moveHead(std::fstream& f) const;

    void moveTail(std::fstream& f) const;

    bool increaseMessageCount(std::fstream& fout) const;

    bool decreaseMessageCount(std::fstream& fout) const;

public:
    Controller(const std::string& binaryFileName, size_t messageCount);
    bool postMessage(const std::string& message) const;
    bool getMessage(std::string& message) const;
    static void initBinaryFile(const std::string& binaryFileName, size_t maxMessageCount);
};