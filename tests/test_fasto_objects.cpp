#include "gtest/gtest.h"

#include "global/global.h"

using namespace fastoredis;

TEST(FastoObject, LifeTime)
{
    FastoObjectPtr obj = FastoObject::createRoot();
    obj.reset();
    FastoObject *ptr = obj.get();
    ASSERT_TRUE(ptr == NULL);
}

TEST(FastoObject, LifeTimeScope)
{
    common::StringValue* obj = common::Value::createStringValue("Sasha");
    {
        FastoObjectPtr root = FastoObject::createRoot();
        FastoObject* ptr(new FastoObject(root.get(), obj));
        root->addChildren(ptr);
    }
}

TEST(FastoObject, StringText)
{
    using namespace common;
    const string16 text = convertToString16("Test");
    fastoredis::FastoObject* obj = fastoredis::FastoObject::createRoot();
    obj->addChildren(new fastoredis::FastoObject(obj, new common::StringValue(text)));
    ASSERT_EQ(escapedText(text), common::convertToString16(obj));

    const string16 text2 = convertToString16("Test2");
    obj->addChildren(new fastoredis::FastoObject(obj, new common::StringValue(text2)));
    string16 line2 = escapedText(text) + escapedText(text2);
    ASSERT_EQ(line2, common::convertToString16(obj));

    const string16 text3 = convertToString16("Test3");
    obj->addChildren(new fastoredis::FastoObject(obj, new common::StringValue(text3)));
    string16 line3 = escapedText(text) + escapedText(text2) + escapedText(text3);
    ASSERT_EQ(line3, common::convertToString16(obj));
}
