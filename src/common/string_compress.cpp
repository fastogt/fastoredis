#include "common/string_compress.h"

#include <string.h>

#include "common/sprintf.h"

#ifdef HAVE_ZLIB

#define BUFFER_SIZE 1024 * 32

namespace common
{
    ErrorValueSPtr encodeZlib(const std::string& data, std::string& out, int compressionlevel)
    {
        if(data.empty()){
            return common::make_error_value("Invalid input", common::ErrorValue::E_ERROR);
        }

        out.clear();

        z_stream zs;                        // z_stream is zlib's control structure
        memset(&zs, 0, sizeof(zs));

        if (deflateInit(&zs, compressionlevel) != Z_OK){
            return common::make_error_value("Zlib init error", common::ErrorValue::E_ERROR);
        }

        zs.next_in = (Bytef*)data.c_str();
        zs.avail_in = data.size();           // set the z_stream's input

        int ret;
        char* outbuffer = new char[BUFFER_SIZE];

        // retrieve the compressed bytes blockwise
        do {
            zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
            zs.avail_out = BUFFER_SIZE;

            ret = deflate(&zs, Z_FINISH);

            if (out.size() < zs.total_out) {
                // append the block to the output string
                out.append(outbuffer, zs.total_out - out.size());
            }
        } while (ret == Z_OK);

        deflateEnd(&zs);

        delete [] outbuffer;

        if (ret != Z_STREAM_END) {          // an error occurred that was not EOF
            char buffer[256];
            SNPrintf(buffer, sizeof(buffer), "Zlib encode error returned code: %d", ret);
            return common::make_error_value(buffer, common::ErrorValue::E_ERROR);
        }

        return ErrorValueSPtr();
    }

    ErrorValueSPtr decodeZlib(const std::string& data, std::string& out)
    {
        if(data.empty()){
            return common::make_error_value("Invalid input", common::ErrorValue::E_ERROR);
        }

        out.clear();

        z_stream zs;                        // z_stream is zlib's control structure
        memset(&zs, 0, sizeof(zs));

        if (inflateInit(&zs) != Z_OK){
            return common::make_error_value("Zlib init error", common::ErrorValue::E_ERROR);
        }

        zs.next_in = (Bytef*)data.data();
        zs.avail_in = data.size();

        int ret;
        char* outbuffer = new char[BUFFER_SIZE];
        // get the decompressed bytes blockwise using repeated calls to inflate
        do {
            zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
            zs.avail_out = BUFFER_SIZE;

            ret = inflate(&zs, 0);

            if (out.size() < zs.total_out) {
                out.append(outbuffer, zs.total_out - out.size());
            }

        } while (ret == Z_OK);

        inflateEnd(&zs);

        delete [] outbuffer;

        if (ret != Z_STREAM_END) {          // an error occurred that was not EOF
            char buffer[256];
            SNPrintf(buffer, sizeof(buffer), "Zlib decode error returned code: %d", ret);
            return common::make_error_value(buffer, common::ErrorValue::E_ERROR);
        }

        return ErrorValueSPtr();
    }
}  // namespace common

#endif
