#include "global/global.h"

#include "common/string_util.h"

namespace fastoredis
{
    FastoObject::FastoObject(FastoObject* parent, common::Value* val, const std::string& delemitr)
        : observer_(NULL), value_(val), parent_(parent), childrens_(), delemitr_(delemitr)
    {
        DCHECK(val);
    }

    FastoObject::~FastoObject()
    {
        for(int i = 0; i < childrens_.size(); ++i){
            FastoObject* item = childrens_[i];
            delete item;
        }
        childrens_.clear();
    }

    common::Value::Type FastoObject::type() const
    {
        return value_->getType();
    }

    std::string FastoObject::toString() const
    {
        std::string result = value_->toString();//getAsString(&result);
        return result;
    }

    FastoObject *FastoObject::createRoot(const std::string &text, IFastoObjectObserver* observer)
    {
        FastoObject* root =  new FastoObject(NULL, common::Value::createStringValue(text), "");
        root->observer_ = observer;
        return root;
    }

    void FastoObject::addChildren(FastoObject* child)
    {
        if(child){
            DCHECK(child->parent_ == this);
            childrens_.push_back(child);
            if(observer_){
                observer_->addedChildren(child);
                child->observer_ = observer_;
            }
        }
    }

    FastoObject* FastoObject::parent() const
    {
        return parent_;
    }

    void FastoObject::clear()
    {
        for (child_container_type::const_iterator it = childrens_.begin(); it != childrens_.end(); ++it){
            FastoObject* child = (*it);
            delete child;
        }
        childrens_.clear();
    }

    std::string FastoObject::delemitr() const
    {
        return delemitr_;
    }

    common::Value* FastoObject::value() const
    {
        return value_.get();
    }

    void FastoObject::setValue(common::Value* val)
    {
        value_.reset(val);
        if(observer_){
            observer_->updated(this, val);
        }
    }

    FastoObject::child_container_type FastoObject::childrens() const
    {
        return childrens_;
    }

    FastoObjectCommand::FastoObjectCommand(FastoObject* parent, common::CommandValue* cmd, const std::string& delemitr)
        : FastoObject(parent, cmd, delemitr)
    {

    }

    common::CommandValue* FastoObjectCommand::cmd() const
    {
        return dynamic_cast<common::CommandValue*>(value_.get());
    }

    std::string FastoObjectCommand::toString() const
    {
        return std::string();
    }

    std::string FastoObjectCommand::inputCommand() const
    {
        common::CommandValue* command = cmd();
        if(command){
            return command->inputCommand();
        }

        return std::string();
    }

    std::string FastoObjectCommand::oppositeCommand() const
    {
        common::CommandValue* command = cmd();
        if(command){
            return command->oppositeCommand();
        }

        return std::string();
    }

    common::Value::CommandType FastoObjectCommand::commandType() const
    {
        common::CommandValue* command = cmd();
        if(command){
            return command->commandType();
        }

        return common::Value::C_UNKNOWN;
    }

    FastoObjectCommand::~FastoObjectCommand()
    {

    }

    std::pair<std::string, std::string> getKeyValueFromLine(const std::string& input)
    {
        if(input.empty()){
            return std::pair<std::string, std::string>();
        }

        size_t pos = input.find_first_of(' ');
        std::string key;
        std::string value;
        if(pos != std::string::npos){
            key = input.substr(0, pos);
            value = input.substr(pos + 1);
        }

        std::string trimed;
        common::TrimWhitespaceASCII(value, common::TRIM_ALL, &trimed);
        return std::make_pair(key, value);
    }

    std::string getOppositeCommand(const std::string& command, const std::vector<std::pair<std::string, std::string > >& srcOppositeCommands)
    {
        DCHECK(!command.empty());
        if(command.empty()){
            return std::string();
        }

        std::string uppercmd = StringToUpperASCII(command);
        for(int i = 0; i < srcOppositeCommands.size(); ++i){
            std::pair<std::string, std::string > p = srcOppositeCommands[i];
            if(p.first == uppercmd){
                return p.second;
            }
            else if(p.second == uppercmd){
                return p.first;
            }
        }

        return std::string();
    }

    FastoObjectArray::FastoObjectArray(FastoObject* parent, common::ArrayValue* ar, const std::string& delemitr)
        : FastoObject(parent, ar, delemitr)
    {

    }

    void FastoObjectArray::append(common::Value* in_value)
    {
        common::ArrayValue* ar = dynamic_cast<common::ArrayValue*>(value_.get());
        if(!ar){
            return;
        }

        ar->append(in_value);
    }

    std::string FastoObjectArray::toString() const
    {
        common::ArrayValue* ar = dynamic_cast<common::ArrayValue*>(value_.get());
        if(!ar){
            return std::string();
        }

        std::string result;
        for(common::ArrayValue::const_iterator it = ar->begin(); it != ar->end(); ++it){
            std::string val = (*it)->toString();
            common::ArrayValue::const_iterator lastIt = it + 1;
            result += val;
            if(!val.empty() && lastIt != ar->end()){
                result += delemitr();
            }
        }
        return result;
    }

    common::ArrayValue* FastoObjectArray::array() const
    {
        return dynamic_cast<common::ArrayValue*>(value_.get());
    }
}

namespace common
{
    std::string convertToString(fastoredis::FastoObject* obj)
    {
        using namespace fastoredis;
        std::string result;
        if(obj){
            const std::string str = obj->toString();
            if(!str.empty()){
                result += str + obj->delemitr();
            }
            FastoObject::child_container_type childrens = obj->childrens();
            for(FastoObject::child_container_type::const_iterator it = childrens.begin(); it != childrens.end(); ++it ){
                result += convertToString(*it);
            }
        }
        return result;
    }
}
