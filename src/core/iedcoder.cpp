#include "core/iedcoder.h"

#include "core/base64_edcoder.h"
#include "core/compress_edcoder.h"
#include "core/hex_edcoder.h"
#include "core/msgpack_edcoder.h"

namespace fastoredis
{
    IEDcoder::EDTypes IEDcoder::type() const
    {
        return type_;
    }

    IEDcoder::IEDcoder(EDTypes type)
        : type_(type)
    {

    }

    common::ErrorValueSPtr IEDcoder::encode(const std::string& data, std::string &out)
    {
        if(data.empty()){
            return common::make_error_value("Invalid input", common::ErrorValue::E_ERROR);
        }

        return encodeImpl(data, out);
    }

    common::ErrorValueSPtr IEDcoder::decode(const std::string& data, std::string& out)
    {
        if(data.empty()){
            return common::make_error_value("Invalid input", common::ErrorValue::E_ERROR);
        }

        return decodeImpl(data, out);
    }

    IEDcoder* IEDcoder::createEDCoder(EDTypes type)
    {
        if(type == Base64){
            return new Base64EDcoder;
        }
        else if(type == Compress){
            return new CompressEDcoder;
        }
        else if(type == Hex){
            return new HexEDcoder;
        }
        else if(type == MsgPack){
            return new MsgPackEDcoder;
        }
        else{
            NOTREACHED();
            return NULL;
        }
    }
}
