#pragma once

#include "global/global.h"

namespace fastoredis
{
    enum EDTypes
    {
        Base64,
        Compress,
        Hex,
        MsgPack,
        HtmlEsc
    };

    const std::string EDecoderTypes[] = { "Base64", "GZip", "Hex", "MsgPack", "HtmlEscape" };

    class IEDcoder
    {
    public:
        common::ErrorValueSPtr encode(const std::string& data, std::string& out) WARN_UNUSED_RESULT;
        common::ErrorValueSPtr decode(const std::string& data, std::string& out) WARN_UNUSED_RESULT;
        EDTypes type() const;

        static IEDcoder* createEDCoder(EDTypes type);
        static IEDcoder* createEDCoder(const std::string& name);

    protected:
        IEDcoder(EDTypes type);

    private:
        virtual common::ErrorValueSPtr encodeImpl(const std::string& data, std::string& out) = 0;
        virtual common::ErrorValueSPtr decodeImpl(const std::string& data, std::string& out) = 0;
        const EDTypes type_;
    };
}

namespace common
{
    std::string convertToString(fastoredis::EDTypes t);
}
