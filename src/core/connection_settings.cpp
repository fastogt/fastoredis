#include "core/connection_settings.h"

#include <QObject>

#include "core/settings_manager.h"

#include "common/utils.h"
#include "common/net/net.h"
#include "common/logger.h"

#define LOGGING_FILE_EXTENSION ".red"

namespace
{
    const std::string defaultNameConnection = "New Connection";
}

namespace fastoredis
{
    IConnectionSettingsBase::IConnectionSettingsBase(const std::string &connectionName)
        : connectionName_(), hash_(), logging_enabled_(false), sshInfo_()
    {
        setConnectionName(connectionName.empty() ? defaultNameConnection : connectionName);
    }

    IConnectionSettingsBase::~IConnectionSettingsBase()
    {

    }

    connectionTypes IConnectionSettingsBase::connectionType() const
    {
        return badConnectionType();
    }

    std::string IConnectionSettingsBase::loggingPath() const
    {
        std::string logDir = SettingsManager::instance().loggingDirectory();
        return logDir + hash() + LOGGING_FILE_EXTENSION;
    }

    IConnectionSettingsBase *IConnectionSettingsBase::fromString(const std::string &val)
    {
        IConnectionSettingsBase *result = NULL;
        if(!val.empty()){
            size_t len = val.size();

            uint8_t commaCount = 0;
            std::string elText;

            for(size_t i = 0; i < len; ++i ){
                char ch = val[i];
                if(ch == ','){
                    if(commaCount == 0){
                        int crT = elText[0] - 48;
                        switch(crT){
                            case REDIS:{
                                result = new RedisConnectionSettings("", redisConfig());
                                break;
                            }
                            default:{
                                NOTREACHED();
                                return result;
                            }
                        }
                    }
                    else if(commaCount == 1){
                        result->setConnectionName(elText);
                    }
                    else if(commaCount == 2){
                        result->setLoggingEnabled(common::convertFromString<uint8_t>(elText));
                    }
                    else if(commaCount == 3){
                        result->initFromCommandLine(elText);
                        result->setSshInfo(SSHInfo(val.substr(i+1)));
                        break;
                    }
                    commaCount++;
                    elText.clear();
                }
                else{
                   elText += ch;
                }
            }
        }
        return result;
    }

    std::string IConnectionSettingsBase::toString() const
    {
        std::string res;
        connectionTypes crT = connectionType();
        if(crT != badConnectionType()){
            std::stringstream str;
            str << crT << ',' << connectionName() << ',' << logging_enabled_ << ',' << toCommandLine() << ',' << sshInfo_.toString();
            res = str.str();
        }
        return res;
    }

    std::string IConnectionSettingsBase::hash() const
    {
        return hash_;
    }

    bool IConnectionSettingsBase::loggingEnabled() const
    {
        return logging_enabled_;
    }

    void IConnectionSettingsBase::setLoggingEnabled(bool isLogging)
    {
        logging_enabled_ = isLogging;
    }

    SSHInfo IConnectionSettingsBase::sshInfo() const
    {
        return sshInfo_;
    }

    void IConnectionSettingsBase::setSshInfo(const SSHInfo& info)
    {
        sshInfo_ = info;
    }

    void IConnectionSettingsBase::setConnectionName(const std::string& name)
    {        
        connectionName_ = name;
        using namespace common::utils;
        common::buffer_type bcon = common::convertFromString<common::buffer_type>(connectionName_);
        uint64_t v = hash::crc64(0, bcon);
        hash_ = common::convertToString(v);
    }

    std::string IConnectionSettingsBase::connectionName() const
    {
        return connectionName_;
    }

    QString useHelpText(connectionTypes type)
    {
        if(type == DBUNKNOWN){
            return QString();
        }
        else if(type == REDIS){
            return QObject::tr("<b>Usage: [OPTIONS] [cmd [arg [arg ...]]]</b><br/>"
                               "<b>-h &lt;hostname&gt;</b>      Server hostname (default: 127.0.0.1).<br/>"
                               "<b>-p &lt;port&gt;</b>          Server port (default: 6379).<br/>"
                               "<b>-s &lt;socket&gt;</b>        Server socket (overrides hostname and port).<br/>"
                               "<b>-a &lt;password&gt;</b>      Password to use when connecting to the server.<br/>"
                               "<b>-r &lt;repeat&gt;</b>        Execute specified command N times.<br/>"
                               "<b>-i &lt;interval&gt;</b>      When <b>-r</b> is used, waits &lt;interval&gt; seconds per command.<br/>"
                               "                   It is possible to specify sub-second times like <b>-i</b> 0.1.<br/>"
                               "<b>-n &lt;db&gt;</b>            Database number.<br/>"
                               "<b>-d &lt;delimiter&gt;</b>     Multi-bulk delimiter in for raw formatting (default: \\n).<br/>"
                               "<b>-c</b>                 Enable cluster mode (follow -ASK and -MOVED redirections).<br/>"
                               "<b>--latency</b>          Enter a special mode continuously sampling latency.<br/>"
                               "<b>--latency-history</b>  Like <b>--latency</b> but tracking latency changes over time.<br/>"
                               "                   Default time interval is 15 sec. Change it using <b>-i</b>.<br/>"
                               "<b>--slave</b>            Simulate a slave showing commands received from the master.<br/>"
                               "<b>--rdb &lt;filename&gt;</b>   Transfer an RDB dump from remote server to local file.<br/>"
                               /*"<b>--pipe</b>             Transfer raw Redis protocol from stdin to server.<br/>"
                               "<b>--pipe-timeout &lt;n&gt;</b> In <b>--pipe mode</b>, abort with error if after sending all data.<br/>"
                               "                   no reply is received within &lt;n&gt; seconds.<br/>"
                               "                   Default timeout: %d. Use 0 to wait forever.<br/>"
                               "<b>--bigkeys</b>          Sample Redis keys looking for big keys.<br/>"*/
                               "<b>--scan</b>             List all keys using the SCAN command.<br/>"
                               "<b>--pattern &lt;pat&gt;</b>    Useful with <b>--scan</b> to specify a SCAN pattern.<br/>"
                               "<b>--intrinsic-latency &lt;sec&gt;</b> Run a test to measure intrinsic system latency.<br/>"
                               "                   The test will run for the specified amount of seconds.<br/>"
                               "<b>--eval &lt;file&gt;</b>      Send an EVAL command using the Lua script at <b>&lt;file&gt;</b>."
                               );
        }

        return QString();
    }

    RedisConnectionSettings::RedisConnectionSettings(const std::string &connectionName, const redisConfig &info)
        :IConnectionSettingsBase(connectionName), info_(info)
    {

    }

    std::string RedisConnectionSettings::host() const
    {
        return info_.hostip;
    }

    int RedisConnectionSettings::port() const
    {
        return info_.hostport;
    }

    void RedisConnectionSettings::setPort(int port)
    {
        info_.hostport = port;
    }

    void RedisConnectionSettings::initFromCommandLine(const std::string &val)
    {
        info_ = common::convertFromString<redisConfig>(val);
    }

    std::string RedisConnectionSettings::toCommandLine() const
    {
        std::string result = common::convertToString(info_);
        return result;
    }

    void RedisConnectionSettings::setCommandLine(const std::string &line)
    {
        info_ = common::convertFromString<redisConfig>(line);
    }

    std::string RedisConnectionSettings::commandLine() const
    {
        return common::convertToString(info_);
    }

    std::string RedisConnectionSettings::fullAddress() const
    {
        common::net::hostAndPort h(host(), port());
        return common::convertToString(h);
    }

    redisConfig RedisConnectionSettings::info() const
    {
        return info_;
    }

    void RedisConnectionSettings::setInfo(const redisConfig &info)
    {
        info_ =  info;
    }

    connectionTypes RedisConnectionSettings::connectionType() const
    {
        return REDIS;
    }

    IConnectionSettingsBase *RedisConnectionSettings::clone() const
    {
        RedisConnectionSettings *red = new RedisConnectionSettings(*this);
        return red;
    }
}
