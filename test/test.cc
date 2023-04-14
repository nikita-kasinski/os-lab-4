#include <gtest/gtest.h>
#include <fstream>
#include <sstream>

#include "controller.h"

const char *testFile = "testfile";

void ensureTestFileExists()
{
    // ensure that file exists
    std::fstream fout(testFile, std::ios::out);
    fout.close();
}

TEST(TestPostMessage, TestFullCapacity)
{
    const int maxMessageCount = 5;
    ensureTestFileExists();
    Controller::initBinaryFile(testFile, maxMessageCount);
    Controller ctrl(testFile);
    for (size_t i = 0; i < maxMessageCount; ++i)
    {
        std::ostringstream message;
        message << "Message " << i;
        ASSERT_TRUE(ctrl.postMessage(message.str()));
    }
    ASSERT_FALSE(ctrl.postMessage("Execessive message"));
}

TEST(TestPostMessage, TestWritingInAvailableSlot)
{
    const int maxMessageCount = 3;
    ensureTestFileExists();
    Controller::initBinaryFile(testFile, maxMessageCount);
    Controller ctrl(testFile);
    for (size_t i = 0; i < maxMessageCount; ++i)
    {
        std::ostringstream message;
        message << "Message " << i;
        ASSERT_TRUE(ctrl.postMessage(message.str()));
    }
    std::ostringstream lastMessage;
    lastMessage << "Message " << maxMessageCount;
    ASSERT_FALSE(ctrl.postMessage(lastMessage.str()));
    std::string firstMessage = "Message 0";
    std::string messageRead;
    ASSERT_TRUE(ctrl.getMessage(messageRead));
    ASSERT_EQ(firstMessage, messageRead);

    ASSERT_TRUE(ctrl.postMessage(lastMessage.str()));
    for (size_t i = 1; i < maxMessageCount; ++i)
    {
        ASSERT_TRUE(ctrl.getMessage(messageRead));
    }
    ASSERT_TRUE(ctrl.getMessage(messageRead));
    ASSERT_EQ(messageRead, lastMessage.str());
}

TEST(TestGetMessage, TestSingleMessage)
{
    const int maxMessageCount = 5;
    Controller::initBinaryFile(testFile, maxMessageCount);
    Controller ctrl(testFile);
    std::string messagePosted;
    std::string messageRead;

    messagePosted = "1";
    ASSERT_TRUE(ctrl.postMessage(messagePosted));
    ASSERT_TRUE(ctrl.getMessage(messageRead));
    EXPECT_EQ(messagePosted, messageRead);
}

TEST(TestFIFOStructure, TestGetAndPostMessage)
{
    const int maxMessageCount = 10;
    const int numberOfIterations = 113;
    Controller::initBinaryFile(testFile, maxMessageCount);
    Controller ctrl(testFile);
    for (size_t i = 0; i < numberOfIterations; ++i)
    {
        std::ostringstream message;
        message << "Message " << i;
        std::string messageRead;
        ASSERT_TRUE(ctrl.postMessage(message.str()));
        ASSERT_TRUE(ctrl.getMessage(messageRead));
        ASSERT_EQ(messageRead, message.str());
    }
}

TEST(TestGetMessage, TestNoMessage)
{
    const int maxMessageCount = 5;
    Controller::initBinaryFile(testFile, maxMessageCount);
    Controller ctrl(testFile);
    std::string messageRead;
    std::string message = "abacaba";
    ASSERT_FALSE(ctrl.getMessage(messageRead));
    ASSERT_TRUE(ctrl.postMessage(message));
    ASSERT_TRUE(ctrl.getMessage(messageRead));
    ASSERT_EQ(messageRead, message);
}

TEST(TestSafeUnsignedIntegerInput, TestStandardInput)
{
    std::istringstream testStream("1 2 3 4\n");
    std::ostringstream ostream;
    size_t a, b, c, d;
    a = Controller::safeUnsignedIntegerInput(testStream, ostream, "", "");
    b = Controller::safeUnsignedIntegerInput(testStream, ostream, "", "");
    c = Controller::safeUnsignedIntegerInput(testStream, ostream, "", "");
    d = Controller::safeUnsignedIntegerInput(testStream, ostream, "", "");
    EXPECT_EQ(a, 1);
    EXPECT_EQ(b, 2);
    EXPECT_EQ(c, 3);
    EXPECT_EQ(d, 4);
}

TEST(TestSafeUnsignedIntegerInput, TestFailedInput)
{
    std::istringstream testStream("asdfbsda1\n2 sadgsadg123gasd\n 3\n");
    std::ostringstream ostream;
    size_t a, b;
    a = Controller::safeUnsignedIntegerInput(testStream, ostream, "", "");
    b = Controller::safeUnsignedIntegerInput(testStream, ostream, "", "");
    EXPECT_EQ(a, 2);
    EXPECT_EQ(b, 3);
}

TEST(TestSafeUnsignedIntegerInput, TestNegativeInput)
{
    std::istringstream testStream("skldghuhasdkjgn\n -1 \n2\n");
    std::ostringstream ostream;
    size_t a;
    a = Controller::safeUnsignedIntegerInput(testStream, ostream, "", "");
    EXPECT_EQ(a, 2);
}
