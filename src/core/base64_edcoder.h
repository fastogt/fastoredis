#pragma once

#include "core/iedcoder.h"

namespace fastoredis
{
    class Base64EDcoder
            : public IEDcoder
    {
    public:
        Base64EDcoder();

    private:
        virtual common::ErrorValueSPtr encodeImpl(const std::string& data, std::string& out);
        virtual common::ErrorValueSPtr decodeImpl(const std::string& data, std::string& out);
    };
}
