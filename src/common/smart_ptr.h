#pragma once

#include "common/intrusive_ptr.h"

#ifdef BOOST_ENABLED
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

namespace common
{
    template<class T>
    using shared_ptr = boost::shared_ptr<T>;

    template<class T>
    using scoped_ptr = boost::scoped_ptr<T>;
}

#elif defined(HAVE_CXX_STANDART)

#include <memory>

namespace common
{
    template<class T>
    using shared_ptr = std::shared_ptr<T>;

    template<class T>
    using scoped_ptr = std::unique_ptr<T>;
}

#endif
