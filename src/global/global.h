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

    std::vector<common::string16> allSupportedViews();

    class FastoObject
            : public common::boost_extension::intrusive_ptr_base<FastoObject>
    {
	public:
        typedef std::vector<FastoObject*> child_container_type;

        FastoObject(FastoObject* parent, common::Value *val);
        ~FastoObject();

        common::Value::Type type() const;
        common::string16 toString() const;
        FastoObject *deepCopy(FastoObject *parent) const;
        FastoObject *deepCopyChangeParent(FastoObject *parent) const;

		child_container_type childrens() const;
        static FastoObject* createRoot(const common::string16 &text = common::string16());
        void addChildren(FastoObject *child);
        bool isRoot() const;

    private:
		DISALLOW_COPY_AND_ASSIGN(FastoObject);

        FastoObject* const parent_;
		child_container_type childrens_;

        const boost::scoped_ptr<common::Value> value_;
    };

    typedef boost::intrusive_ptr<FastoObject> FastoObjectPtr;
}

namespace common
{
    string16 convertToString16(fastoredis::supportedViews v);
    string16 convertToString16(fastoredis::FastoObject* obj);
}
