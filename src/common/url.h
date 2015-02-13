#pragma once

#include "common/memory_string.h"

namespace common
{
    namespace url
    {
        class url
        {
            public:
                enum supported_protocols
                {
                    http = 0,//defualt value
                    ftp = 1,
                    file = 2
                };
                url();
                explicit url(const char* url_s);
                url(const url& other);
                url& operator=(const url& other);
                bool is_valid()const;
                const memory_string& protocol()const;
                const memory_string& host()const;
                const memory_string& path()const;
                const memory_string& query()const;
                std::string get_url()const;
                ~url();

            private:
                enum
                {
                    host_size = 64,
                    path_size = 256,
                    query_size = 512
                };
                void init(const url& other);
                void parse(const char* url_s);
                supported_protocols protocol_;
                memory_string host_;
                memory_string path_;
                memory_string query_;
        };

        namespace detail
        {
            bool get_protocol(const char *url_s, url::supported_protocols &prot);
            bool get_protocol(const char *url_s, size_t len, url::supported_protocols &prot);
            /* Returns a url-encoded version of str */
            /* IMPORTANT: be sure to free() the returned string after use */
            char *url_encode(const char *str);
            char *url_encode(const char *str, size_t len);
            /* Returns a url-decoded version of str */
            /* IMPORTANT: be sure to free() the returned string after use */
            char *url_decode(const char *str);
            char *url_decode(const char *str, size_t len);
        }
    }
}
