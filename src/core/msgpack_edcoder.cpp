#include "core/msgpack_edcoder.h"

#include <stdio.h>
#include <errno.h>
#include <inttypes.h>
#include <string.h>

#include "third-party/cmp/cmp.h"

namespace
{
    bool read_bytes(void *data, size_t sz, char *ch)
    {
        int len = strlen(ch);
        bool isComp = len >= sz;
        if(isComp){
            memcpy(data, ch, sz);
            memmove(ch, ch + sz, len - sz);
            ch[len - sz] = 0;
        }
        else{
            memcpy(data, ch, len);
            ch[0] = 0;
        }

        return isComp;
    }

    bool stream_reader(cmp_ctx_t *ctx, void *data, size_t limit)
    {
        return read_bytes(data, limit, (char*)ctx->buf);
    }

    size_t stram_writer(cmp_ctx_t *ctx, const void *data, size_t count)
    {
        return 0;
    }
}

namespace fastoredis
{
    MsgPackEDcoder::MsgPackEDcoder()
        : IEDcoder(MsgPack)
    {

    }

    common::ErrorValueSPtr MsgPackEDcoder::encodeImpl(const std::string& data, std::string& out)
    {
        cmp_ctx_t cmp;
        char* copy = (char*)calloc(data.size() + 1, sizeof(char));
        memcpy(copy, data.c_str(), data.size());
        cmp_init(&cmp, (void*)copy, NULL, stram_writer);

        out = "Not supported now";

        free(copy);
        return common::ErrorValueSPtr();
    }

    common::ErrorValueSPtr MsgPackEDcoder::decodeImpl(const std::string& data, std::string& out)
    {
        cmp_ctx_t cmp;
        char* copy = (char*)calloc(data.size() + 1, sizeof(char));
        memcpy(copy, data.c_str(), data.size());

        cmp_init(&cmp, (void*)copy, stream_reader, NULL);

        while (1) {
           cmp_object_t obj;

           if (!cmp_read_object(&cmp, &obj)) {
               if (copy[0] == 0){
                   break;
               }

               return common::make_error_value(cmp_strerror(&cmp), common::ErrorValue::E_ERROR);
           }

            char sbuf[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

            switch (obj.type) {
                case CMP_TYPE_POSITIVE_FIXNUM:
                case CMP_TYPE_UINT8:
                out += common::convertToString(obj.as.u8);
                break;
            case CMP_TYPE_FIXMAP:
            case CMP_TYPE_MAP16:
            case CMP_TYPE_MAP32:
                break;
            case CMP_TYPE_FIXARRAY:
            case CMP_TYPE_ARRAY16:
            case CMP_TYPE_ARRAY32:
                break;
            case CMP_TYPE_FIXSTR:
            case CMP_TYPE_STR8:
            case CMP_TYPE_STR16:
            case CMP_TYPE_STR32:
                if (!read_bytes(sbuf, obj.as.str_size, copy)){
                    free(copy);
                    return common::make_error_value(cmp_strerror(&cmp), common::ErrorValue::E_ERROR);
                }
                sbuf[obj.as.str_size] = 0;
                out += sbuf;
                break;
            case CMP_TYPE_BIN8:
            case CMP_TYPE_BIN16:
            case CMP_TYPE_BIN32:
                memset(sbuf, 0, sizeof(sbuf));
                if (!read_bytes(sbuf, obj.as.bin_size, copy)){
                    free(copy);
                    return common::make_error_value(cmp_strerror(&cmp), common::ErrorValue::E_ERROR);
                }
                out.append(sbuf, obj.as.bin_size);
                break;
            case CMP_TYPE_NIL:
                out += "NULL";
                break;
            case CMP_TYPE_BOOLEAN:
                if (obj.as.boolean)
                    out += "true";
                else
                    out += "false";
                break;
            case CMP_TYPE_EXT8:
            case CMP_TYPE_EXT16:
            case CMP_TYPE_EXT32:
            case CMP_TYPE_FIXEXT1:
            case CMP_TYPE_FIXEXT2:
            case CMP_TYPE_FIXEXT4:
            case CMP_TYPE_FIXEXT8:
            case CMP_TYPE_FIXEXT16:
                if (obj.as.ext.type == 1) { /* Date object */
                    uint16_t year = 0;
                    uint8_t month = 0;
                    uint8_t day = 0;
                    if (!read_bytes(&year, sizeof(uint16_t), copy)){
                        free(copy);
                        return common::make_error_value(cmp_strerror(&cmp), common::ErrorValue::E_ERROR);
                    }

                    if (!read_bytes(&month, sizeof(uint8_t), copy)){
                        free(copy);
                        return common::make_error_value(cmp_strerror(&cmp), common::ErrorValue::E_ERROR);
                    }

                    if (!read_bytes(&day, sizeof(uint8_t), copy)){
                        free(copy);
                        return common::make_error_value(cmp_strerror(&cmp), common::ErrorValue::E_ERROR);
                    }

                    char buff[32] = {0};
                    sprintf(buff, "%u/%u/%u", year, month, day);
                    out += buff;
                }
                else {
                    while (obj.as.ext.size--) {
                        read_bytes(sbuf, sizeof(uint8_t), copy);
                        char buff[32] = {0};
                        sprintf(buff, "%02x ", sbuf[0]);
                        out += buff;
                    }
                }
                break;
            case CMP_TYPE_FLOAT:
                out += common::convertToString(obj.as.flt);
                break;
            case CMP_TYPE_DOUBLE:
                out += common::convertToString(obj.as.dbl);
                break;
            case CMP_TYPE_UINT16:
                out += common::convertToString(obj.as.u16);
                break;
            case CMP_TYPE_UINT32:
                out += common::convertToString(obj.as.u32);
                break;
            case CMP_TYPE_UINT64:
                {
                    char buff[32] = {0};
                    sprintf(buff, "%" PRIu64 "", obj.as.u64);
                    out += buff;
                    break;
                }
            case CMP_TYPE_NEGATIVE_FIXNUM:
            case CMP_TYPE_SINT8:
                out += common::convertToString(obj.as.s8);
                break;
            case CMP_TYPE_SINT16:
                out += common::convertToString(obj.as.s16);
                break;
            case CMP_TYPE_SINT32:
                out += common::convertToString(obj.as.s32);
                break;
            case CMP_TYPE_SINT64:
                {
                    char buff[32] = {0};
                    sprintf(buff, "%" PRId64 "", obj.as.s64);
                    out += buff;
                    break;
                }
            default:
                {
                    char buff[64] = {0};
                    sprintf(buff, "Unrecognized object type %u\n", obj.type);
                    return common::make_error_value(buff, common::ErrorValue::E_ERROR);
                }
            break;
            }
        }

        free(copy);
        return common::ErrorValueSPtr();
    }
}
