#include "gtest/gtest.h"

#include "global/global.h"

TEST(FastoObject, LifeTime)
{
    fastoredis::FastoObjectPtr obj = fastoredis::FastoObject::createRoot();
    obj.reset();
    fastoredis::FastoObject *ptr = obj.get();
    ASSERT_EQ(ptr, NULL);
}

TEST(FastoObject, StringText)
{
    using namespace common;
    const std::string text = "Test";
    fastoredis::FastoObjectPtr obj = fastoredis::FastoObject::createRoot();
    obj->addChildren(new fastoredis::FastoObject(obj, new common::StringValue(text)));
    ASSERT_EQ(escapedText(text), common::convert2string(obj));

    const std::string text2 = "Test2";
    obj->addChildren(new fastoredis::FastoObject(obj, new common::StringValue(text2)));
    std::string line2 = escapedText(text) + escapedText(text2);
    ASSERT_EQ(line2, common::convert2string(obj));

    const std::string text3 = "Test3";
    obj->addChildren(new fastoredis::FastoObject(obj, new common::StringValue(text3)));
    std::string line3 = escapedText(text) + escapedText(text2) + escapedText(text3);
    ASSERT_EQ(line3, common::convert2string(obj));
}
