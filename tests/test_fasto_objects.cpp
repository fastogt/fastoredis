#include "gtest/gtest.h"
#include "global/global.h"

TEST(FastoObject, LifeTime)
{
    fastoredis::FastoObjectPtr obj = fastoredis::FastoObject::createRoot();
    obj.reset();
    ASSERT_EQ(obj.get(), 0);
}

TEST(FastoObject, StringText)
{
    using namespace common;
    const std::string text = "Test";
    fastoredis::FastoObjectPtr obj = fastoredis::FastoObject::createRoot();
    obj->addChildren(new fastoredis::FastoObject(obj, new common::StringValue(text)));
    ASSERT_EQ(escapedText(text), fastoredis::toStdString(obj));

    const std::string text2 = "Test2";
    obj->addChildren(new fastoredis::FastoObject(obj, new common::StringValue(text2)));

    ASSERT_EQ(escapedText(text) + escapedText(text2), fastoredis::toStdString(obj));

    const std::string text3 = "Test3";
    obj->addChildren(new fastoredis::FastoObject(obj, new common::StringValue(text3)));

    ASSERT_EQ(escapedText(text) + escapedText(text2) + escapedText(text3), fastoredis::toStdString(obj));
}
