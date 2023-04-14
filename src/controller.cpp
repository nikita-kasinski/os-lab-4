#include "controller.h"
#include <string>
#include <fstream>
#include <iostream>
#include <cassert>
#include <limits>

Controller::Controller(const std::string &binaryFileName)
{
    this->binaryFileName = binaryFileName;
}

/*void Controller::outBinFile(std::fstream &f)
{

    std::cout << "\n---------------------\n";
    std::cout << f.fail() << f.bad() << f.eof() << "\n";
    f.seekg(0);
    while (f.good())
    {
        char x;
        f.read(&x, 1);
        std::cout << (int)x << ' ';
    }
    std::cout << "\n---------------------\n";
}*/

void Controller::movePointer(std::fstream &f, size_t &pointer) const
{
    size_t maxMessageCount = getMaxMessageCount(f);
    if (pointer == maxMessageCount - 1) // -1 as pointer is in range (0, maxMessageCount - 1)
    {
        pointer = 0;
    }
    else
    {
        ++pointer;
    }
}

size_t Controller::getHead(std::fstream &f) const
{
    size_t head;
    f.clear();
    f.seekg(headOffset);
    f.read(reinterpret_cast<char *>(&head), atomicSize);
    return head;
}

size_t Controller::getTail(std::fstream &f) const
{
    size_t tail;
    f.clear();
    f.seekg(tailOffset);
    f.read(reinterpret_cast<char *>(&tail), atomicSize);
    return tail;
}

size_t Controller::getMessageCount(std::fstream &f) const
{
    size_t messageCount;
    f.clear();
    f.seekg(messageCountOffset);
    f.read(reinterpret_cast<char *>(&messageCount), atomicSize);
    return messageCount;
}

size_t Controller::getMaxMessageCount(std::fstream &f) const
{
    size_t maxMessageCount;
    f.clear();
    f.seekg(maxMessageCountOffset);
    f.read(reinterpret_cast<char *>(&maxMessageCount), atomicSize);
    return maxMessageCount;
}

void Controller::moveTail(std::fstream &f) const
{
    size_t tail = getTail(f);
    movePointer(f, tail);
    f.clear();
    f.seekp(tailOffset);
    f.write(reinterpret_cast<char *>(&tail), atomicSize);
}

void Controller::moveHead(std::fstream &f) const
{
    size_t head = getHead(f);
    movePointer(f, head);
    f.clear();
    f.seekp(headOffset);
    f.write(reinterpret_cast<char *>(&head), atomicSize);
}

bool Controller::increaseMessageCount(std::fstream &f) const
{
    size_t messageCount = getMessageCount(f);
    size_t maxMessageCount = getMaxMessageCount(f);
    f.clear();
    f.seekp(messageCountOffset);
    if (messageCount == maxMessageCount)
    {
        return false;
    }
    else
    {
        ++messageCount;
        f.write(reinterpret_cast<char *>(&messageCount), atomicSize);
        return true;
    }
}

bool Controller::decreaseMessageCount(std::fstream &f) const
{
    size_t messageCount = getMessageCount(f);
    f.clear();
    f.seekp(messageCountOffset);
    if (messageCount == 0)
    {
        return false;
    }
    else
    {
        --messageCount;
        f.write(reinterpret_cast<char *>(&messageCount), atomicSize);
        return true;
    }
}

void Controller::initBinaryFile(const std::string &binaryFileName, size_t maxMessageCount)
{
    std::fstream f(binaryFileName.c_str(), std::ios::binary | std::ios::out);
    std::string dummyString;

    for (size_t i = 0; i < maxMessageSize; ++i)
    {
        dummyString += dummyChar;
    }
    // first - head, second - tail, third - messageCount,  forth - maxMessageCount then messages
    size_t zero = 0;
    f.clear();
    f.seekp(0);
    f.write(reinterpret_cast<char *>(&zero), atomicSize);
    f.write(reinterpret_cast<char *>(&zero), atomicSize);
    f.write(reinterpret_cast<char *>(&zero), atomicSize);
    f.write(reinterpret_cast<char *>(&maxMessageCount), atomicSize);
    for (size_t i = 0; i < maxMessageCount; ++i)
    {
        f.write(dummyString.c_str(), dummyString.length() + 1);
    }
    f.close();
}

bool Controller::postMessage(const std::string &message) const
{
    assert(message.length() <= maxMessageSize);
    std::fstream f(binaryFileName.c_str(), std::ios::binary | std::ios::out | std::ios::in);
    if (increaseMessageCount(f))
    {
        size_t tail = getTail(f);
        f.clear();
        f.seekp(overallOffset + tail * (maxMessageSize + 1));
        f.write(message.c_str(), message.length() + 1);
        moveTail(f);
        f.close();
        return true;
    }
    else
    {
        f.close();
        return false;
    }
}

bool Controller::getMessage(std::string &message) const
{
    std::fstream f(binaryFileName.c_str(), std::ios::binary | std::ios::out | std::ios::in);
    if (decreaseMessageCount(f))
    {
        size_t head = getHead(f);
        char charMessage[maxMessageSize + 1];
        f.clear();
        f.seekg(overallOffset + head * (maxMessageSize + 1));
        f.read(charMessage, maxMessageSize + 1);
        message = std::string(charMessage);
        moveHead(f);
        return true;
    }
    else
    {
        return false;
    }
}

size_t Controller::safeUnsignedIntegerInput(std::istream &in, std::ostream &out, const std::string &inputPrompt, const std::string &inputFailedPrompt)
{
    constexpr auto maxStreamSize = std::numeric_limits<std::streamsize>::max();
    int tempAns = -1;
    bool failed = false;

    while (tempAns <= 0)
    {
        if (failed)
        {
            out << inputFailedPrompt;
        }
        else
        {
            failed = true;
        }
        out << inputPrompt;
        in >> tempAns;
        if (!in.good())
        {
            in.clear();
            in.ignore(maxStreamSize, '\n');
            tempAns = -1;
        }
    }
    return static_cast<size_t>(tempAns);
}