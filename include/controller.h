#pragma once
#include <string>
#include <iostream>
#include <fstream>

class Controller
{
private:
    std::string binaryFileName;
    static const size_t maxMessageSize = 20;
    static const char dummyChar = '0';
    static const size_t atomicOffset = sizeof(size_t);
    static const size_t atomicSize = atomicOffset;
    static const size_t headOffset = 0;
    static const size_t tailOffset = atomicOffset;
    static const size_t messageCountOffset = 2 * atomicOffset;
    static const size_t maxMessageCountOffset = 3 * atomicOffset;
    static const size_t overallOffset = 4 * atomicOffset;

    //static void outBinFile(std::fstream &f);

    void movePointer(std::fstream &f, size_t &pointer) const;

    // first - head, second - tail, third - messageCount
    size_t getHead(std::fstream &fin) const;

    // first - head, second - tail, third - messageCount
    size_t getTail(std::fstream &fin) const;

    size_t getMessageCount(std::fstream &fin) const;

    size_t getMaxMessageCount(std::fstream &f) const;

    void moveHead(std::fstream &f) const;

    void moveTail(std::fstream &f) const;

    bool increaseMessageCount(std::fstream &fout) const;

    bool decreaseMessageCount(std::fstream &fout) const;

public:
    Controller(const std::string &binaryFileName);
    bool postMessage(const std::string &message) const;
    bool getMessage(std::string &message) const;
    static void initBinaryFile(const std::string &binaryFileName, size_t maxMessageCount);
    static size_t safeUnsignedIntegerInput(std::istream &in, std::ostream &out, const std::string &inputPrompt, const std::string &inputFailedPrompt);
};