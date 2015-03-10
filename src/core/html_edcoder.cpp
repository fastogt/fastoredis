#include "core/html_edcoder.h"

#include "common/utils.h"

namespace fastoredis
{
    HtmlEscEDcoder::HtmlEscEDcoder()
        : IEDcoder(Hex)
    {

    }

    common::ErrorValueSPtr HtmlEscEDcoder::encodeImpl(const std::string& data, std::string& out)
    {
        out = common::utils::html::encode(data);
        return common::ErrorValueSPtr();
    }

    common::ErrorValueSPtr HtmlEscEDcoder::decodeImpl(const std::string& data, std::string& out)
    {
        out = common::utils::html::decode(data);
        return common::ErrorValueSPtr();
    }
}
