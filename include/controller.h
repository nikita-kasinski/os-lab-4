#pragma once

class Controller
{
private:
    std::string binaryFileName;
    int currentAvailableMessage = 0;
    int messageCount;
public:
    Controller(const std::string& binaryFileName, int messageCount);
    bool postMessage(const std::string& message) const;
    std::string getMessage() const;
};