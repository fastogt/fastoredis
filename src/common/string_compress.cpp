#include "common/string_compress.h"

#include <string.h>

#ifdef HAVE_ZLIB

#include "zlib.h"

#define BUFFER_SIZE 1024 * 32

namespace common
{
    bool encodeZlib(const std::string& data, std::string& out, int compressionlevel)
    {
        if(data.empty()){
            return false;
        }

        out.clear();

        z_stream zs;                        // z_stream is zlib's control structure
        memset(&zs, 0, sizeof(zs));

        if (deflateInit(&zs, compressionlevel) != Z_OK){
            return false;
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
            return false;
        }

        return true;
    }

    bool decodeZlib(const std::string& data, std::string& out)
    {
        if(data.empty()){
            return false;
        }

        out.clear();

        z_stream zs;                        // z_stream is zlib's control structure
        memset(&zs, 0, sizeof(zs));

        if (inflateInit(&zs) != Z_OK){
            return false;
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
            return false;
        }

        return true;
    }
}  // namespace common

#endif
