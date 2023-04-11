#include "controller.h"
#include <string>
#include <fstream>
#include <iostream>
#include <cassert>

Controller::Controller(const std::string &binaryFileName, size_t messageCount)
{
    this->binaryFileName = binaryFileName;
    this->maxMessageCount = messageCount;
    this->messageHead = 0;
    this->messageTail = 0;
}

void Controller::movePointer(size_t &pointer) const
{
    if (pointer == maxMessageCount - 1) // -1 as pointer is in range (0, maxMessageCount - 1)
    {
        pointer = 0;
    }
    else
    {
        ++pointer;
    }
}

void Controller::initBinaryFile(const std::string &binaryFileName, size_t maxMessageCount)
{
    std::fstream fout(binaryFileName, std::ios::binary | std::ios::out);
    constexpr int maxMessageSize = 20;
    constexpr char dummyChar = '0';
    std::string dummyString;

    for (size_t i = 0; i < maxMessageSize; ++i)
    {
        dummyString += dummyChar;
    }

    for (size_t i = 0; i < maxMessageCount; ++i)
    {
        fout.write(dummyString.c_str(), sizeof(dummyString.c_str()));
    }
}
bool Controller::postMessage(const std::string &message) const
{
    assert(message.length() <= 20);
    std::fstream fout(binaryFileName, std::ios::binary | std::ios::out | std::ios::in);
    fout.seekp(5);
    fout.write("##", 2);
    fout.close();
    return false;
}