#include "core/memcached/memcached_config.h"

#include "common/utils.h"

#include "core/logger.h"

namespace fastoredis
{
    namespace
    {
        int parseOptions(int argc, char **argv, memcachedConfig& cfg) {
            int i;

            for (i = 0; i < argc; i++) {
                int lastarg = i==argc-1;

                if (!strcmp(argv[i],"-h") && !lastarg) {
                    free(cfg.hostip);
                    cfg.hostip = strdup(argv[++i]);
                }
                else if (!strcmp(argv[i],"-p") && !lastarg) {
                    cfg.hostport = atoi(argv[++i]);
                }
                else if (!strcmp(argv[i],"-d") && !lastarg) {
                    free(cfg.mb_delim);
                    cfg.mb_delim = strdup(argv[++i]);
                }
                else {
                    if (argv[i][0] == '-') {
                        const uint16_t size_buff = 256;
                        char buff[size_buff] = {0};
                        sprintf(buff, "Unrecognized option or bad number of args for: '%s'", argv[i]);
                        LOG_MSG(buff, common::logging::L_WARNING);
                        break;
                    } else {
                        /* Likely the command name, stop here. */
                        break;
                    }
                }
            }
            return i;
        }
    }

    memcachedConfig::memcachedConfig()
    {
        init();
    }

    memcachedConfig::memcachedConfig(const memcachedConfig &other)
    {
        init();
        copy(other);
    }

    memcachedConfig& memcachedConfig::operator=(const memcachedConfig &other)
    {
        copy(other);
        return *this;
    }

    void memcachedConfig::copy(const memcachedConfig& other)
    {
        using namespace common::utils;
        freeifnotnull(hostip);
        hostip = strdupornull(other.hostip); //

        hostport = other.hostport;

        freeifnotnull(mb_delim);
        mb_delim = strdupornull(other.mb_delim); //
    }

    void memcachedConfig::init()
    {
        hostip = strdup("127.0.0.1");
        hostport = 11211;

        mb_delim = strdup("\n");
    }

    memcachedConfig::~memcachedConfig()
    {
        using namespace common::utils;
        freeifnotnull(hostip);
        freeifnotnull(mb_delim);
    }
}

namespace common
{
    std::string convertToString(const fastoredis::memcachedConfig &conf)
    {
        std::vector<std::string> argv;

        if(conf.hostip){
            argv.push_back("-h");
            argv.push_back(conf.hostip);
        }

        if(conf.hostport){
            argv.push_back("-p");
            argv.push_back(convertToString(conf.hostport));
        }

        if (conf.mb_delim) {
            argv.push_back("-d");
            argv.push_back(conf.mb_delim);
        }

        std::string result;
        for(int i = 0; i < argv.size(); ++i){
            result+= argv[i];
            if(i != argv.size()-1){
                result+=" ";
            }
        }

        return result;
    }

    template<>
    fastoredis::memcachedConfig convertFromString(const std::string& line)
    {
        fastoredis::memcachedConfig cfg;
        enum { kMaxArgs = 64 };
        int argc = 0;
        char *argv[kMaxArgs] = {0};

        char* p2 = strtok((char*)line.c_str(), " ");
        while(p2){
            argv[argc++] = p2;
            p2 = strtok(0, " ");
        }

        fastoredis::parseOptions(argc, argv, cfg);
        return cfg;
    }
}
