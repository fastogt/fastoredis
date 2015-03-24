#include "core/iedcoder.h"

#include "core/base64_edcoder.h"
#include "core/compress_edcoder.h"
#include "core/hex_edcoder.h"
#include "core/msgpack_edcoder.h"
#include "core/html_edcoder.h"

namespace common
{
    std::string convertToString(fastoredis::EDTypes t)
    {
        return fastoredis::EDecoderTypes[t];
    }

    template<>
    fastoredis::EDTypes convertFromString(const std::string& text)
    {
        for (uint32_t i = 0; i < SIZEOFMASS(fastoredis::EDecoderTypes); ++i){
            if (text == fastoredis::EDecoderTypes[i]){
                return static_cast<fastoredis::EDTypes>(i);
            }
        }

        NOTREACHED();
        return fastoredis::Base64;
    }

}

namespace fastoredis
{
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

    EDTypes IEDcoder::type() const
    {
        return type_;
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
        else if(type == HtmlEsc){
            return new HtmlEscEDcoder;
        }
        else{
            NOTREACHED();
            return NULL;
        }
    }

    IEDcoder* IEDcoder::createEDCoder(const std::string& name)
    {
        fastoredis::EDTypes t = common::convertFromString<fastoredis::EDTypes>(name);
        return createEDCoder(t);
    }
}


