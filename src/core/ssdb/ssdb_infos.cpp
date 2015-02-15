#include "core/ssdb/ssdb_infos.h"

#include <ostream>
#include <sstream>

namespace fastoredis
{   
    SsdbServerInfo::Common::Common()
    {

    }

    SsdbServerInfo::Common::Common(const std::string& common_text)
    {
        const std::string &src = common_text;
        size_t pos = 0;
        size_t start = 0;

        while((pos = src.find(("\r\n"), start)) != std::string::npos){
            std::string line = src.substr(start, pos-start);
            size_t delem = line.find_first_of(':');
            std::string field = line.substr(0, delem);
            std::string value = line.substr(delem + 1);
            if(field == SSDB_PID_LABEL){
                pid_ = common::convertFromString<uint32_t>(value);
            }
            else if(field == SSDB_UPTIME_LABEL){
                uptime_ = common::convertFromString<uint32_t>(value);
            }
            else if(field == SSDB_TIME_LABEL){
                time_ = common::convertFromString<uint32_t>(value);
            }
            else if(field == SSDB_VERSION_LABEL){
                version_ = value;
            }
            else if(field == SSDB_POINTER_SIZE_LABEL){
                pointer_size_ = common::convertFromString<uint32_t>(value);
            }
            else if(field == SSDB_RUSAGE_USER_LABEL){
                rusage_user_ = common::convertFromString<uint32_t>(value);
            }
            else if(field == SSDB_RUSAGE_SYSTEM_LABEL){
                rusage_system_ = common::convertFromString<uint32_t>(value);
            }
            else if(field == SSDB_CURR_ITEMS_LABEL){
                curr_items_ = common::convertFromString<uint32_t>(value);
            }
            else if(field == SSDB_TOTAL_ITEMS_LABEL){
                total_items_ = common::convertFromString<uint32_t>(value);
            }
            else if(field == SSDB_BYTES_LABEL){
                bytes_ = common::convertFromString<uint32_t>(value);
            }
            else if(field == SSDB_CURR_CONNECTIONS_LABEL){
                curr_connections_ = common::convertFromString<uint32_t>(value);
            }
            else if(field == SSDB_TOTAL_CONNECTIONS_LABEL){
                total_connections_ = common::convertFromString<uint32_t>(value);
            }
            else if(field == SSDB_CONNECTION_STRUCTURES_LABEL){
                connection_structures_ = common::convertFromString<uint32_t>(value);
            }
            else if(field == SSDB_CMD_GET_LABEL){
                cmd_get_ = common::convertFromString<uint32_t>(value);
            }
            else if(field == SSDB_CMD_SET_LABEL){
                cmd_set_ = common::convertFromString<uint32_t>(value);
            }
            else if(field == SSDB_GET_HITS_LABEL){
                get_hits_ = common::convertFromString<uint32_t>(value);
            }
            else if(field == SSDB_GET_MISSES_LABEL){
                get_misses_ = common::convertFromString<uint32_t>(value);
            }
            else if(field == SSDB_EVICTIONS_LABEL){
                evictions_ = common::convertFromString<uint32_t>(value);
            }
            else if(field == SSDB_BYTES_READ_LABEL){
                bytes_read_ = common::convertFromString<uint32_t>(value);
            }
            else if(field == SSDB_BYTES_WRITTEN_LABEL){
                bytes_written_ = common::convertFromString<uint32_t>(value);
            }
            else if(field == SSDB_LIMIT_MAXBYTES_LABEL){
                limit_maxbytes_ = common::convertFromString<uint32_t>(value);
            }
            else if(field == SSDB_THREADS_LABEL){
                threads_ = common::convertFromString<uint32_t>(value);
            }
            start = pos + 2;
        }
    }

    common::Value* SsdbServerInfo::Common::valueByIndex(unsigned char index) const
    {
        switch (index) {
        case 0:
            return new common::FundamentalValue(pid_);
        case 1:
            return new common::FundamentalValue(uptime_);
        case 2:
            return new common::FundamentalValue(time_);
        case 3:
            return new common::StringValue(version_);
        case 4:
            return new common::FundamentalValue(pointer_size_);
        case 5:
            return new common::FundamentalValue(rusage_user_);
        case 6:
            return new common::FundamentalValue(rusage_system_);
        case 7:
            return new common::FundamentalValue(curr_items_);
        case 8:
            return new common::FundamentalValue(total_items_);
        case 9:
            return new common::FundamentalValue(bytes_);
        case 10:
            return new common::FundamentalValue(curr_connections_);
        case 11:
            return new common::FundamentalValue(total_connections_);
        case 12:
            return new common::FundamentalValue(connection_structures_);
        case 13:
            return new common::FundamentalValue(cmd_get_);
        case 14:
            return new common::FundamentalValue(cmd_set_);
        case 15:
            return new common::FundamentalValue(get_hits_);
        case 16:
            return new common::FundamentalValue(get_misses_);
        case 17:
            return new common::FundamentalValue(evictions_);
        case 18:
            return new common::FundamentalValue(bytes_read_);
        case 19:
            return new common::FundamentalValue(bytes_written_);
        case 20:
            return new common::FundamentalValue(limit_maxbytes_);
        case 21:
            return new common::FundamentalValue(threads_);
        default:
            NOTREACHED();
            break;
        }
        return NULL;
    }

    SsdbServerInfo::SsdbServerInfo()
        : ServerInfo(SSDB)
    {

    }

    SsdbServerInfo::SsdbServerInfo(const Common& common)
        : ServerInfo(SSDB), common_(common)
    {

    }

    common::Value* SsdbServerInfo::valueByIndexes(unsigned char property, unsigned char field) const
    {
        switch (property) {
        case 0:
            return common_.valueByIndex(field);
        default:
            NOTREACHED();
            break;
        }
        return NULL;
    }

    std::ostream& operator<<(std::ostream& out, const SsdbServerInfo::Common& value)
    {
        return out << SSDB_PID_LABEL":" << value.pid_ << ("\r\n")
                    << SSDB_UPTIME_LABEL":" << value.uptime_ << ("\r\n")
                    << SSDB_TIME_LABEL":" << value.time_ << ("\r\n")
                    << SSDB_VERSION_LABEL":" << value.version_ << ("\r\n")
                    << SSDB_POINTER_SIZE_LABEL":" << value.pointer_size_ << ("\r\n")
                    << SSDB_RUSAGE_USER_LABEL":" << value.rusage_user_ << ("\r\n")
                    << SSDB_RUSAGE_SYSTEM_LABEL":" << value.rusage_system_ << ("\r\n")
                    << SSDB_CURR_ITEMS_LABEL":" << value.curr_items_ << ("\r\n")
                    << SSDB_TOTAL_ITEMS_LABEL":" << value.total_items_ << ("\r\n")
                    << SSDB_BYTES_LABEL":" << value.bytes_ << ("\r\n")
                    << SSDB_CURR_CONNECTIONS_LABEL":" << value.curr_connections_ << ("\r\n")
                    << SSDB_TOTAL_CONNECTIONS_LABEL":" << value.total_connections_ << ("\r\n")
                    << SSDB_CONNECTION_STRUCTURES_LABEL":" << value.connection_structures_ << ("\r\n")
                    << SSDB_CMD_GET_LABEL":" << value.cmd_get_ << ("\r\n")
                    << SSDB_CMD_SET_LABEL":" << value.cmd_set_ << ("\r\n")
                    << SSDB_GET_HITS_LABEL":" << value.get_hits_ << ("\r\n")
                    << SSDB_GET_MISSES_LABEL":" << value.get_misses_ << ("\r\n")
                    << SSDB_EVICTIONS_LABEL":" << value.evictions_ << ("\r\n")
                    << SSDB_BYTES_READ_LABEL":" << value.bytes_read_ << ("\r\n")
                    << SSDB_BYTES_WRITTEN_LABEL":" << value.bytes_written_ << ("\r\n")
                    << SSDB_LIMIT_MAXBYTES_LABEL":" << value.limit_maxbytes_ << ("\r\n")
                    << SSDB_THREADS_LABEL":" << value.threads_ << ("\r\n");
    }

    std::ostream& operator<<(std::ostream& out, const SsdbServerInfo& value)
    {
        return out << value.toString();
    }

    SsdbServerInfo* makeSsdbServerInfo(const std::string &content)
    {
        if(content.empty()){
            return NULL;
        }

        SsdbServerInfo* result = new SsdbServerInfo;

        int j = 0;
        std::string word;
        size_t pos = 0;
        for(int i = 0; i < content.size(); ++i)
        {
            char ch = content[i];
            word += ch;
            if(word == SsdbHeaders[j]){
                if(j+1 != SsdbHeaders.size()){
                    pos = content.find(SsdbHeaders[j+1], pos);
                }
                else{
                    break;
                }

                if(pos != std::string::npos){
                    std::string part = content.substr(i + 1, pos - i - 1 );
                    switch(j)
                    {
                    case 0:
                        result->common_ = SsdbServerInfo::Common(part);
                        break;
                    default:
                        break;
                    }
                    i = pos-1;
                    ++j;
                }
                word.clear();
            }
        }

        return result;
    }


    std::string SsdbServerInfo::toString() const
    {
        std::stringstream str;
        str << SSDB_COMMON_LABEL"\r\n" << common_;
        return str.str();
    }

    SsdbServerInfo* makeSsdbServerInfo(FastoObject* root)
    {
        const std::string content = common::convertToString(root);
        return makeSsdbServerInfo(content);
    }

    SsdbDataBaseInfo::SsdbDataBaseInfo(const std::string& name, size_t size, bool isDefault)
        : DataBaseInfo(name, size, isDefault, SSDB)
    {

    }

    DataBaseInfo* SsdbDataBaseInfo::clone() const
    {
        return new SsdbDataBaseInfo(*this);
    }
}
