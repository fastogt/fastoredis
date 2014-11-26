#include "core/redis/redis_config.h"

#include "core/logger.h"


namespace fastoredis
{
    namespace
    {
        int parseOptions(int argc, char **argv, redisConfig& cfg) {
            int i;

            for (i = 0; i < argc; i++) {
                int lastarg = i==argc-1;

                if (!strcmp(argv[i],"-h") && !lastarg) {
                    sdsfree(cfg.hostip);
                    cfg.hostip = sdsnew(argv[++i]);
                }/* else if (!strcmp(argv[i],"-h") && lastarg) {
                    usage();
                } else if (!strcmp(argv[i],"--help")) {
                    usage();
                } else if (!strcmp(argv[i],"-x")) {
                    cfg.stdinarg = 1;
                }*/ else if (!strcmp(argv[i],"-p") && !lastarg) {
                    cfg.hostport = atoi(argv[++i]);
                } else if (!strcmp(argv[i],"-s") && !lastarg) {
                    cfg.hostsocket = argv[++i];
                } else if (!strcmp(argv[i],"-r") && !lastarg) {
                    cfg.repeat = strtoll(argv[++i],NULL,10);
                } else if (!strcmp(argv[i],"-i") && !lastarg) {
                    double seconds = atof(argv[++i]);
                    cfg.interval = seconds*1000000;
                } else if (!strcmp(argv[i],"-n") && !lastarg) {
                    cfg.dbnum = atoi(argv[++i]);
                } else if (!strcmp(argv[i],"-a") && !lastarg) {
                    cfg.auth = argv[++i];
                }
                /*else if (!strcmp(argv[i],"--raw")) {
                    cfg.output = OUTPUT_RAW;
                } else if (!strcmp(argv[i],"--no-raw")) {
                    cfg.output = OUTPUT_STANDARD;
                } else if (!strcmp(argv[i],"--csv")) {
                    cfg.output = OUTPUT_CSV;
                }*/ else if (!strcmp(argv[i],"--latency")) {
                    cfg.latency_mode = 1;
                } else if (!strcmp(argv[i],"--latency-history")) {
                    cfg.latency_mode = 1;
                    cfg.latency_history = 1;
                } else if (!strcmp(argv[i],"--slave")) {
                    cfg.slave_mode = 1;
                } else if (!strcmp(argv[i],"--stat")) {
                    cfg.stat_mode = 1;
                } else if (!strcmp(argv[i],"--scan")) {
                    cfg.scan_mode = 1;
                } else if (!strcmp(argv[i],"--pattern") && !lastarg) {
                    cfg.pattern = argv[++i];
                } else if (!strcmp(argv[i],"--intrinsic-latency") && !lastarg) {
                    cfg.intrinsic_latency_mode = 1;
                    cfg.intrinsic_latency_duration = atoi(argv[++i]);
                } else if (!strcmp(argv[i],"--rdb") && !lastarg) {
                    cfg.getrdb_mode = 1;
                    cfg.rdb_filename = argv[++i];
                } else if (!strcmp(argv[i],"--pipe")) {
                    cfg.pipe_mode = 1;
                } else if (!strcmp(argv[i],"--pipe-timeout") && !lastarg) {
                    cfg.pipe_timeout = atoi(argv[++i]);
                } else if (!strcmp(argv[i],"--bigkeys")) {
                    cfg.bigkeys = 1;
                } else if (!strcmp(argv[i],"--eval") && !lastarg) {
                    cfg.eval = argv[++i];
                } else if (!strcmp(argv[i],"-c")) {
                    cfg.cluster_mode = 1;
                }
                else if (!strcmp(argv[i],"-d") && !lastarg) {
                    sdsfree(cfg.mb_delim);
                    cfg.mb_delim = sdsnew(argv[++i]);
                }
                /*else if (!strcmp(argv[i],"-v") || !strcmp(argv[i], "--version")) {
                    sds version = cliVersion();
                    printf("redis-cli %s\n", version);
                    sdsfree(version);
                }*/
                else {
                    if (argv[i][0] == '-') {
                        const uint16_t size_buff = 256;
                        char buff[size_buff] = {0};
                        sprintf(buff, "Unrecognized option or bad number of args for: '%s'", argv[i]);
                        common::ErrorValueSPtr er(new common::ErrorValue(buff, common::Value::E_ERROR));
                        LOG_ERROR(er);
                        break;
                    } else {
                        /* Likely the command name, stop here. */
                        break;
                    }
                }
            }
            return i;
        }

        redisConfig parseOptions(const std::string& commandLine)
        {
            redisConfig cfg;
            enum { kMaxArgs = 64 };
            int argc = 0;
            char *argv[kMaxArgs] = {0};

            char* p2 = strtok((char*)commandLine.c_str(), " ");
            while (p2)
            {
                argv[argc++] = p2;
                p2 = strtok(0, " ");
            }

            int i = parseOptions(argc, argv, cfg);
            return cfg;
        }
    }

    redisConfig::redisConfig()
    {
        hostip = sdsnew("127.0.0.1");
        hostport = 6379;
        hostsocket = NULL;
        repeat = 1;
        interval = 0;
        dbnum = 0;
        interactive = 0;
        shutdown = 0;
        monitor_mode = 0;
        pubsub_mode = 0;
        latency_mode = 0;
        latency_history = 0;
        cluster_mode = 0;
        slave_mode = 0;
        getrdb_mode = 0;
        stat_mode = 0;
        scan_mode = 0;
        intrinsic_latency_mode = 0;
        intrinsic_latency_duration = 0;
        cluster_reissue_command = 0;
        pattern = NULL;
        rdb_filename = NULL;
        pipe_mode = 0;
        pipe_timeout = REDIS_CLI_DEFAULT_PIPE_TIMEOUT;
        bigkeys = 0;
        auth = NULL;
        eval = NULL;
        last_cmd_type = -1;

        mb_delim = sdsnew("\n");
    }

    redisConfig::~redisConfig()
    {

    }
}

namespace common
{
    std::string convertToString(const fastoredis::redisConfig& conf)
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
        if(conf.hostsocket){
            argv.push_back("-s");
            argv.push_back(conf.hostsocket);
        }
        if(conf.repeat){
            argv.push_back("-r");
            argv.push_back(convertToString(conf.repeat));
        }
        if(conf.interval){
            argv.push_back("-i");
            argv.push_back(convertToString(conf.interval/1000000));
        }
        if(conf.dbnum){
            argv.push_back("-n");
            argv.push_back(convertToString(conf.dbnum));
        }

        if(conf.auth){
            argv.push_back("-a");
            argv.push_back(conf.auth);
        }

        if(conf.latency_mode){
            if(conf.latency_history){
                argv.push_back("--latency-history");
            }
            else{
                argv.push_back("--latency");
            }
        }

        if(conf.slave_mode){
            argv.push_back("--slave");
        }
        if(conf.stat_mode){
            argv.push_back("--stat");
        }
        if(conf.scan_mode){
            argv.push_back("--scan");
        }
        if(conf.pattern){
            argv.push_back("--pattern");
            argv.push_back(conf.pattern);
        }
        if(conf.intrinsic_latency_mode){
            argv.push_back("--intrinsic-latency");
            argv.push_back(convertToString(conf.intrinsic_latency_mode));
            argv.push_back(convertToString(conf.intrinsic_latency_duration));
        }

        if(conf.getrdb_mode){
            argv.push_back("--rdb");
            argv.push_back(conf.rdb_filename);
        }
        if(conf.pipe_mode){
            argv.push_back("--pipe");
        }
        if(conf.pipe_timeout){
            argv.push_back("--pipe-timeout");
            argv.push_back(convertToString(conf.pipe_timeout));
        }
        if(conf.bigkeys){
            argv.push_back("--bigkeys");
        }

        if(conf.eval){
           argv.push_back("--eval");
           argv.push_back(conf.eval);
        }

        if (conf.mb_delim) {
            argv.push_back("-d");
            argv.push_back(conf.mb_delim);
        }

        if(conf.cluster_mode){
            argv.push_back("-c");
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
    fastoredis::redisConfig convertFromString(const std::string& line)
    {
        fastoredis::redisConfig r = fastoredis::parseOptions(line);
        return r;
    }
}
