#include "core/redis/RedisConfig.h"

#include <ostream>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

namespace fastoredis
{
    redisConfig::redisConfig()
    {
        hostip = std::string();
        hostport = 0;
        hostsocket = std::string();
        repeat = 0;
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
        rdb_filename = std::string();
        pipe_mode = 0;
        pipe_timeout = 0;
        bigkeys = 0;
        stdinarg = 0;
        auth = std::string();
        eval = std::string();
        /*if (!isatty(fileno(stdout)) && (getenv("FAKETTY") == NULL))
            output = OUTPUT_RAW;
        else
            output = OUTPUT_STANDARD;*/
        mb_delim = std::string();
    }

    void redisConfig::parseOptions(const std::vector<std::string> &argv) {
        int argc = argv.size();
        for (int i = 0; i < argc; i++) {
            int lastarg = i==argc-1;
            const char *curArg = argv[i].c_str();

            if (!strcmp(curArg,"-h") && !lastarg) {
                hostip = argv[++i];
            } else if (!strcmp(curArg,"-h") && lastarg) {
            } else if (!strcmp(curArg,"--help")) {
            } else if (!strcmp(curArg,"-x")) {
                stdinarg = 1;
            } else if (!strcmp(curArg,"-p") && !lastarg) {
                hostport = boost::lexical_cast<int>(argv[++i]);
            } else if (!strcmp(curArg,"-s") && !lastarg) {
                hostsocket = argv[++i];
            } else if (!strcmp(curArg,"-r") && !lastarg) {
                repeat = strtoll(argv[++i].c_str(),NULL,10);
            } else if (!strcmp(curArg,"-i") && !lastarg) {
                double seconds = boost::lexical_cast<double>(argv[++i]);
                interval = seconds*1000000;
            } else if (!strcmp(curArg,"-n") && !lastarg) {
                dbnum = boost::lexical_cast<int>(argv[++i]);
            } else if (!strcmp(curArg,"-a") && !lastarg) {
                auth = argv[++i];
            } else if (!strcmp(curArg,"--raw")) {
                //output = OUTPUT_RAW;
            } else if (!strcmp(curArg,"--csv")) {
                //output = OUTPUT_CSV;
            } else if (!strcmp(curArg,"--latency")) {
                latency_mode = 1;
            } else if (!strcmp(curArg,"--latency-history")) {
                latency_mode = 1;
                latency_history = 1;
            } else if (!strcmp(curArg,"--slave")) {
                slave_mode = 1;
            } else if (!strcmp(curArg,"--stat")) {
                stat_mode = 1;
            } else if (!strcmp(curArg,"--rdb") && !lastarg) {
                getrdb_mode = 1;
                rdb_filename = argv[++i];
            } else if (!strcmp(curArg,"--pipe")) {
                pipe_mode = 1;
            } else if (!strcmp(curArg,"--pipe-timeout") && !lastarg) {
                pipe_timeout = boost::lexical_cast<int>(argv[++i]);
            } else if (!strcmp(curArg,"--bigkeys")) {
                bigkeys = 1;
            } else if (!strcmp(curArg,"--eval") && !lastarg) {
                eval = argv[++i];
            } else if (!strcmp(curArg,"-c")) {
                cluster_mode = 1;
            } else if (!strcmp(curArg,"-d") && !lastarg) {
                mb_delim = argv[++i];
            } else if (!strcmp(curArg,"-v") || !strcmp(curArg, "--version")) {
                break;
            } else {
                if (argv[i][0] == '-') {
                    fprintf(stderr,
                        "Unrecognized option or bad number of args for: '%s'\n",
                        curArg);
                    break;
                } else {
                    /* Likely the command name, stop here. */
                    break;
                }
            }
        }
    }

    std::string toStdString(const redisConfig &conf)
    {
        std::vector<std::string> argv;

        if(!conf.hostip.empty()){
            argv.push_back("-h");
            argv.push_back(conf.hostip);
        }
        if(conf.hostport){
            argv.push_back("-p");
            argv.push_back(boost::lexical_cast<std::string>(conf.hostport));
        }
        if(!conf.hostsocket.empty()){
            argv.push_back("-s");
            argv.push_back(conf.hostsocket);
        }
        if(conf.repeat){
            argv.push_back("-r");
            argv.push_back(boost::lexical_cast<std::string>(conf.repeat));
        }
        if(conf.interval){
            argv.push_back("-i");
            argv.push_back(boost::lexical_cast<std::string>(conf.interval));
        }
        if(conf.dbnum){
            argv.push_back("-n");
            argv.push_back(boost::lexical_cast<std::string>(conf.dbnum));
        }
        if(conf.latency_mode){
            if(conf.latency_history){
                argv.push_back("--latency-history");
            }
            else{
                argv.push_back("--latency");
            }
        }
        if(conf.cluster_mode){
            argv.push_back("-c");
        }
        if(conf.slave_mode){
            argv.push_back("--slave");
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
            argv.push_back(boost::lexical_cast<std::string>(conf.pipe_timeout));
        }
        if(conf.bigkeys){
            argv.push_back("--bigkeys");
        }
        if(conf.stdinarg){
            argv.push_back("-x");
        }
        if(!conf.auth.empty()){
           argv.push_back("-a");
           argv.push_back(conf.auth);
        }
        if(!conf.eval.empty()){
           argv.push_back("--eval");
           argv.push_back(conf.eval);
        }
        if(!conf.mb_delim.empty()){
           argv.push_back("--d");
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

    redisConfig rcFromStdString(const std::string &line)
    {
        std::vector<std::string> argv;
        boost::split(argv, line, boost::is_any_of(" "));
        redisConfig r;
        r.parseOptions(argv);
        return r;
    }
}
