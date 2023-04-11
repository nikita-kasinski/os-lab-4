#include "controller.h"
#include <string>
#include <fstream>

Controller::Controller(const std::string &binaryFileName, int messageCount)
{
    this->binaryFileName = binaryFileName;
    this->messageCount = messageCount;
}

bool Controller::postMessage(const std::string& message) const
{
    std::ofstream fout(binaryFileName, std::ios::binary | std::ios::out);
}