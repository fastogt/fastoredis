#pragma once

#include "global/global.h"

namespace fastoredis
{
    class IEDcoder
    {
    public:
        enum EDTypes
        {
            Base64,
            Compress,
            Hex,
            MsgPack
        };

        common::ErrorValueSPtr encode(const std::string& data, std::string& out) WARN_UNUSED_RESULT;
        common::ErrorValueSPtr decode(const std::string& data, std::string& out) WARN_UNUSED_RESULT;
        EDTypes type() const;

        static IEDcoder* createEDCoder(EDTypes type);

    protected:
        IEDcoder(EDTypes type);

    private:
        virtual common::ErrorValueSPtr encodeImpl(const std::string& data, std::string& out) = 0;
        virtual common::ErrorValueSPtr decodeImpl(const std::string& data, std::string& out) = 0;
        const EDTypes type_;
    };
}
