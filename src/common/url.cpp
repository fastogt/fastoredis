#include "common/url.h"

#include <algorithm>
#include <string.h>

namespace
{
    using namespace common;
    const memory_string protocols_array[]=
    {
        memory_string(UTEXT("http")),
        memory_string(UTEXT("ftp")),
        memory_string(UTEXT("file"))
    };
}
namespace common
{
    namespace url
    {
        namespace detail
        {
            char from_hex(char ch)
            {
                return isdigit(ch) ? ch - '0' : tolower(ch) - 'a' + 10;
            }

            char to_hex(char code)
            {
                static char hex[] = "0123456789abcdef";
                return hex[code & 15];
            }

            char *url_encode(const char *str, size_t len)
            {
                const char *pstr = str;
                char *buf = (char *)malloc(len * 3 + 1);
                char *pbuf = buf;

                while (*pstr)
                {
                    if (isalnum(*pstr) || *pstr == '-' || *pstr == '_' || *pstr == '.' || *pstr == '~'){
                        *pbuf++ = *pstr;
                    }
                    else if (*pstr == ' '){
                        *pbuf++ = '+';
                    }
                    else{
                        *pbuf++ = '%', *pbuf++ = to_hex(*pstr >> 4), *pbuf++ = to_hex(*pstr & 15);
                    }
                    pstr++;
                }

                *pbuf = 0;
                return buf;
            }

            char *url_encode(const char *str)
            {
                return url_encode(str, strlen(str));
            }

            char *url_decode(const char *str, size_t len)
            {
                const char *pstr = str;
                char *buf = (char*)malloc(len + 1);
                char *pbuf = buf;

                while (*pstr)
                {
                    if (*pstr == '%') {
                        if (pstr[1] && pstr[2]) {
                            *pbuf++ = from_hex(pstr[1]) << 4 | from_hex(pstr[2]);
                            pstr += 2;
                        }
                    } else if (*pstr == '+') {
                        *pbuf++ = ' ';
                    } else {
                        *pbuf++ = *pstr;
                    }
                    pstr++;
                }

                *pbuf = 0;
                return buf;
            }

            char *url_decode(const char *str)
            {
                return url_decode(str, strlen(str));
            }

            bool get_protocol(const char *url_s, size_t len, url::supported_protocols &prot)
            {
                bool result=false;
                if(url_s&&5<len)
                {
                    if((url_s[4]==':'&&url_s[5]=='/'&&url_s[6]=='/')
                            ||(url_s[3]==':'&&url_s[4]=='/'&&url_s[5]=='/'))
                    {
                        if(url_s[0]=='f'||url_s[0]=='F')
                        {
                            if(url_s[0]=='i'||url_s[0]=='I')
                            {
                                prot=url::file;
                                result = true;
                            }
                            else
                            {
                                prot=url::ftp;
                                result = true;
                            }
                        }
                        else if(url_s[0]=='h'||url_s[0]=='h')
                        {
                            prot=url::http;
                            result = true;
                        }
                    }
                }
                return result;
            }

            bool get_protocol(const char *url_s, url::supported_protocols &prot)
            {
                return get_protocol(url_s, strlen(url_s),prot);
            }
        }

        url::url():protocol_(http),host_(host_size),path_(path_size),query_(query_size)
        {

        }

        url::url(const char* url_s)
            :protocol_(http),host_(host_size),path_(path_size),query_(query_size)
        {
            parse(url_s);
        }

        void url::parse(const char* url_s)
        {
            //ftp,http,file
            size_t len = strlen(url_s);
            size_t start=0;
            if(detail::get_protocol(url_s,len,protocol_))
            {
                if(protocol_==ftp)
                {
                    start = 6;
                }
                else if(protocol_==http||protocol_==file)
                {
                    start = 7;
                }
            }
            memory_string *cur_member = &host_;
            for(size_t i=start;i<len;++i)
            {
                if(url_s[i]==UTEXT('/')||url_s[i]==UTEXT('?'))
                {
                    if(url_s[i]==UTEXT('/')&&cur_member==&host_)
                    {
                        cur_member = &path_;
                    }
                    else if(url_s[i]==UTEXT('?')&&cur_member==&path_)
                    {
                        cur_member = &query_;
                    }
                }
                cur_member->append(tolower(url_s[i]));
            }
        }

        bool url::is_valid() const
        {
            return host_.c_str()!=NULL;
        }

        const memory_string &url::protocol()const
        {
            return protocols_array[protocol_];
        }

        const memory_string &url::host()const
        {
            return host_;
        }

        const memory_string &url::path()const
        {
            return path_;
        }

        const memory_string &url::query()const
        {
            return query_;
        }

        const std::string url::get_url()const
        {
            char buf[host_size + path_size + query_size + 7] = {0};
            sprintf(buf, "%s://%s%s%s", protocol().c_str(), host_.c_str(), path_.c_str(), query_.c_str());
            return buf;
        }

        url::~url()
        {
        }

        url::url(const url &other):protocol_(other.protocol_),host_(other.host_),
            path_(other.path_),query_(other.query_)
        {
        }

        url& url::operator=(const url &other)
        {
            protocol_ = other.protocol_;
            host_ = other.host_;
            path_= other.path_;
            query_ = other.query_;
            return *this;
        }
    }
}
