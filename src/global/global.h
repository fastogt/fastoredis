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

    std::vector<std::string> allSupportedViews();

    class IFastoObjectObserver;
    class FastoObject
            : public common::boost_extension::intrusive_ptr_base<FastoObject>
    {
	public:
        typedef std::vector<FastoObject*> child_container_type;

        FastoObject(FastoObject* parent, common::Value* val, const std::string& delemitr);
        ~FastoObject();

        common::Value::Type type() const;
        std::string toString() const;

		child_container_type childrens() const;
        static FastoObject* createRoot(const std::string& text, IFastoObjectObserver* observer = NULL);
        void addChildren(FastoObject* child);
        FastoObject* parent() const;
        void clear();
        std::string delemitr() const;

        void changeValue(common::Value* val);

    private:
        DISALLOW_COPY_AND_ASSIGN(FastoObject);

        FastoObject* const parent_;
		child_container_type childrens_;
        const std::string delemitr_;
        IFastoObjectObserver* observer_;

        boost::scoped_ptr<common::Value> value_;
    };

    class IFastoObjectObserver
    {
    public:
        virtual void addedChildren(FastoObject* child) = 0;
        virtual void updated(FastoObject* item, common::Value* val) = 0;
    };

    typedef boost::intrusive_ptr<FastoObject> FastoObjectPtr;
}

namespace common
{
    string16 convertToString16(fastoredis::supportedViews v);
    std::string convertToString(fastoredis::FastoObject* obj);
}
