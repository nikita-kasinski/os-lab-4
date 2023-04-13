#include <gtest/gtest.h>
#include <fstream>
#include <sstream>

#include "controller.h"

const char* testFile = "testfile";

TEST(TestPostMessage, TestInitial)
{
    const int maxMessageCount = 5;
    {
        // ensure that file exists
        std::fstream fout(testFile, std::ios::out);
        fout.close();
    }
    Controller::initBinaryFile(testFile, maxMessageCount);
    Controller ctrl(testFile, maxMessageCount);
    for(size_t i = 0; i < maxMessageCount; ++i)
    {
        std::ostringstream message;
        message << "Message " << i;
        ASSERT_TRUE(ctrl.postMessage(message.str()));
    }
    ASSERT_FALSE(ctrl.postMessage("Execessive message"));
}
