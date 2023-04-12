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

size_t Controller::getHead(std::fstream &fin) const
{
    fin.seekp(headOffset);
    size_t head;
    fin.read((char *)&head, atomicSize);
    return head;
}

size_t Controller::getTail(std::fstream &fin) const
{
    fin.seekp(tailOffset);
    size_t tail;
    fin.read((char *)&tail, atomicSize);
    return tail;
}

size_t Controller::getMessageCount(std::fstream &fin) const
{
    fin.seekp(messageCountOffset);
    size_t messageCount;
    fin.read((char *)&messageCount, atomicSize);
    return messageCount;
}

void Controller::moveTail(std::fstream &f) const
{
    size_t tail = getTail(f);
    movePointer(tail);
    f.seekp(tailOffset);
    f.write((char *)&tail, atomicSize);
}

void Controller::moveHead(std::fstream &f) const
{
    size_t head = getHead(f);
    movePointer(head);
    f.seekp(headOffset);
    f.write((char *)&head, atomicSize);
}

bool Controller::increaseMessageCount(std::fstream &f) const
{
    size_t messageCount = getMessageCount(f);
    f.seekp(messageCountOffset);
    if (messageCount == maxMessageCount)
    {
        return false;
    }
    else
    {
        ++messageCount;
        f.write((char *)&(messageCount), atomicSize);
        return true;
    }
}

bool Controller::decreaseMessageCount(std::fstream &f) const
{
    size_t messageCount = getMessageCount(f);
    f.seekp(messageCountOffset);
    if (messageCount == 0)
    {
        return false;
    }
    else
    {
        --messageCount;
        f.write((char*)&(messageCount), atomicSize);
        return true;
    }
}

void Controller::initBinaryFile(const std::string &binaryFileName, size_t maxMessageCount)
{
    std::fstream fout(binaryFileName, std::ios::binary | std::ios::out);
    std::string dummyString;

    for (size_t i = 0; i < maxMessageSize; ++i)
    {
        dummyString += dummyChar;
    }

    // first - head, second - tail, third - messageCount,  then messages
    fout.write(0, atomicSize);
    fout.write(0, atomicSize);
    fout.write(0, atomicSize);
    for (size_t i = 0; i < maxMessageCount; ++i)
    {
        fout.write(dummyString.c_str(), sizeof(dummyString.c_str()));
    }
}

bool Controller::postMessage(const std::string &message) const
{
    assert(message.length() <= maxMessageSize);
    std::fstream fout(binaryFileName, std::ios::binary | std::ios::out | std::ios::in);
    size_t tail = getTail(fout);
    fout.seekp(overallOffset + tail * (maxMessageSize + 1));
    fout.write(message.c_str(), sizeof(message.c_str()));
    moveTail(fout);
    fout.close();
}