#pragma once

#include "core/iedcoder.h"

namespace fastoredis
{
    class CompressEDcoder
            : public IEDcoder
    {
    public:
        CompressEDcoder();

    private:
        virtual common::ErrorValueSPtr encodeImpl(const std::string& data, std::string& out);
        virtual common::ErrorValueSPtr decodeImpl(const std::string& data, std::string& out);
    };
}
