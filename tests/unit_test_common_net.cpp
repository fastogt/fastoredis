#include "gtest/gtest.h"

#include "global/global.h"

#include "common/net.h"

TEST(common_net, getRemoteMacAddress)
{
    const std::string google = "google.ru";
    std::string out_mac;
    bool res = common::net::getRemoteMacAddress(google, out_mac);
    ASSERT_TRUE(res);
}
