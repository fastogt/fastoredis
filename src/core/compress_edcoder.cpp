#include "core/compress_edcoder.h"

#include "common/string_compress.h"

namespace fastoredis
{
    CompressEDcoder::CompressEDcoder()
        : IEDcoder(Compress)
    {

    }

    common::ErrorValueSPtr CompressEDcoder::encodeImpl(const std::string& data, std::string& out)
    {
#ifdef HAVE_ZLIB
        return common::encodeZlib(data, out);
#else
        return common::make_error_value("Compress encode not supported", common::ErrorValue::E_ERROR);
#endif
    }

    common::ErrorValueSPtr CompressEDcoder::decodeImpl(const std::string& data, std::string& out)
    {
#ifdef HAVE_ZLIB
        return common::decodeZlib(data, out);
#else
        return common::make_error_value("Compress decode not supported", common::ErrorValue::E_ERROR);
#endif
    }
}
