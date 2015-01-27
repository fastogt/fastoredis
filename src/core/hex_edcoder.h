#pragma once

#include "core/iedcoder.h"

namespace fastoredis
{
    class HexEDcoder
            : public IEDcoder
    {
    public:
        HexEDcoder();

    private:
        virtual common::ErrorValueSPtr encodeImpl(const std::string& data, std::string& out);
        virtual common::ErrorValueSPtr decodeImpl(const std::string& data, std::string& out);
    };
}
