#pragma once

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
            : public common::intrusive_ptr_base<FastoObject>
    {
	public:
        typedef std::vector<FastoObject*> child_container_type;

        FastoObject(FastoObject* parent, common::Value* val, const std::string& delemitr);
        virtual ~FastoObject();

        common::Value::Type type() const;
        virtual std::string toString() const;

		child_container_type childrens() const;
        static FastoObject* createRoot(const std::string& text, IFastoObjectObserver* observer = NULL);
        void addChildren(FastoObject* child);
        FastoObject* parent() const;
        void clear();
        std::string delemitr() const;

        void changeValue(common::Value* val);

    protected:
        IFastoObjectObserver* observer_;
        scoped_ptr_t<common::Value> value_;

    private:
        DISALLOW_COPY_AND_ASSIGN(FastoObject);

        FastoObject* const parent_;
		child_container_type childrens_;
        const std::string delemitr_;
    };

    class FastoObjectArray
            : public FastoObject
    {
    public:
        FastoObjectArray(FastoObject* parent, common::ArrayValue* ar, const std::string& delemitr);

        // Appends a Value to the end of the list.
        void append(common::Value* in_value);
        virtual std::string toString() const;

        common::ArrayValue* array() const;
    };

    class IFastoObjectObserver
    {
    public:
        virtual void addedChildren(FastoObject* child) = 0;
        virtual void updated(FastoObject* item, common::Value* val) = 0;
    };

    typedef common::intrusive_ptr<FastoObject> FastoObjectIPtr;
}

namespace common
{
    string16 convertToString16(fastoredis::supportedViews v);
    std::string convertToString(fastoredis::FastoObject* obj);
}
