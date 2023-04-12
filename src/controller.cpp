#include "controller.h"
#include <string>
#include <fstream>
#include <iostream>
#include <cassert>

Controller::Controller(const std::string &binaryFileName, size_t messageCount)
{
    this->binaryFileName = binaryFileName;
    this->maxMessageCount = messageCount;
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

size_t Controller::getHead(std::fstream& fin) const
{
    
    fin.seekp(0);
    size_t head;
    fin.read((char*)&head, atomicSize);
    return head;
}

size_t Controller::getTail(std::fstream& fin) const
{
    fin.seekp(0);
    size_t tail;
    fin.read((char*)&tail, atomicSize);
    return tail;
}
void Controller::initBinaryFile(const std::string &binaryFileName, size_t maxMessageCount)
{
    std::fstream fout(binaryFileName, std::ios::binary | std::ios::out);
    std::string dummyString;

    for (size_t i = 0; i < maxMessageSize; ++i)
    {
        dummyString += dummyChar;
    }

    //first - head, second - tail, then messages
    fout.write(0, atomicOffset);
    fout.write(0, atomicOffset);
    for (size_t i = 0; i < maxMessageCount; ++i)
    {
        fout.write(dummyString.c_str(), sizeof(dummyString.c_str()));
    }
}

bool Controller::postMessage(const std::string &message) const
{
    assert(message.length() <= maxMessageSize);
    std::fstream fout(binaryFileName, std::ios::binary | std::ios::out | std::ios::in);
    //fout.seekp(messageTail * (maxMessageSize + 1));
    fout.write(message.c_str(), sizeof(message.c_str()));
    fout.close();
}