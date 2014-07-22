#pragma once

/**/

#include "common/boost_extension.h"
#include "common/value.h"
#include "common/convert2string.h"

namespace fastoredis
{
    enum supportedViews
    {
        Tree = 0,
        Table,
        Text
    };

    std::vector<common::unicode_string> allSupportedViews();

    class FastoObject;
    typedef boost::intrusive_ptr<FastoObject> FastoObjectPtr;

    class FastoObject
            : public common::boost_extension::intrusive_ptr_base<FastoObject>
    {
	public:
		typedef std::vector<FastoObjectPtr> child_container_type;

        FastoObject(const FastoObjectPtr &parent, common::Value *val);
        ~FastoObject();

        common::Value::Type type() const;
        common::unicode_string toString() const;
        FastoObjectPtr deepCopy(const FastoObjectPtr &parent) const;

		child_container_type childrens() const;
        static FastoObjectPtr createRoot(const std::string& text = std::string());
        void addChildren(const FastoObjectPtr &child);
        bool isRoot() const;

    private:
		DISALLOW_COPY_AND_ASSIGN(FastoObject);

        const FastoObjectPtr parent_;
		child_container_type childrens_;

        const boost::scoped_ptr<common::Value> value_;
    };
}

namespace common
{
    unicode_string convert2string(fastoredis::supportedViews v);
    unicode_string convert2string(const fastoredis::FastoObjectPtr& obj);
}
