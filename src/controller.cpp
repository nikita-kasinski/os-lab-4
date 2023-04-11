#include "controller.h"
#include <string>
#include <fstream>
#include <iostream>
#include <cassert>

Controller::Controller(const std::string &binaryFileName, int messageCount)
{
    this->binaryFileName = binaryFileName;
    this->maxMessageCount = messageCount;
    this->messageHead = 0;
    this->messageTail = 0;
}

void Controller::movePointer(int& pointer) const
{

}
bool Controller::postMessage(const std::string& message) const
{
    assert(message.length() <= 20);
    std::fstream fout(binaryFileName, std::ios::binary | std::ios::out | std::ios::in);
    fout.seekp(5);
    fout.write("##", 2);
    fout.close();
    return false;
}