#include "core/hex_edcoder.h"

#include "common/convert2string.h"

namespace fastoredis
{
    HexEDcoder::HexEDcoder()
        : IEDcoder(Hex)
    {

    }

    common::ErrorValueSPtr encodeImpl(const std::string& data, std::string& out)
    {
        out = common::HexEncode(data);
        return common::ErrorValueSPtr();
    }

    common::ErrorValueSPtr decodeImpl(const std::string& data, std::string& out)
    {
        out = common::HexDecode(data);
        return common::ErrorValueSPtr();
    }
}
