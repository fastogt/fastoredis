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
            if(field == SSDB_VERSION_LABEL){
                version_ = value;
            }
            else if(field == SSDB_LINKS_LABEL){
                links_ = common::convertFromString<uint32_t>(value);
            }
            else if(field == SSDB_TOTAL_CALLS_LABEL){
                total_calls_ = common::convertFromString<uint32_t>(value);
            }
            else if(field == SSDB_DBSIZE_LABEL){
                dbsize_ = common::convertFromString<uint32_t>(value);
            }
            else if(field == SSDB_BINLOGS_LABEL){
                binlogs_ = value;
            }
            start = pos + 2;
        }
    }

    common::Value* SsdbServerInfo::Common::valueByIndex(unsigned char index) const
    {
        switch (index) {
        case 0:
            return new common::StringValue(version_);
        case 1:
            return new common::FundamentalValue(links_);
        case 2:
            return new common::FundamentalValue(total_calls_);
        case 3:
            return new common::FundamentalValue(dbsize_);
        case 4:
            return new common::StringValue(binlogs_);
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
        return out << SSDB_VERSION_LABEL":" << value.version_ << ("\r\n")
                    << SSDB_LINKS_LABEL":" << value.links_ << ("\r\n")
                    << SSDB_TOTAL_CALLS_LABEL":" << value.total_calls_ << ("\r\n")
                    << SSDB_DBSIZE_LABEL":" << value.dbsize_ << ("\r\n")
                    << SSDB_BINLOGS_LABEL":" << value.binlogs_ << ("\r\n");
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
