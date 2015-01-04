#pragma once

#ifdef BOOST_ENABLED
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

#define shared_ptr_t boost::shared_ptr
#define scoped_ptr_t boost::scoped_ptr

namespace common
{

}

#elif defined(HAVE_CXX_STANDART)

#include <memory>

#define shared_ptr_t std::shared_ptr
#define scoped_ptr_t std::unique_ptr

namespace common
{

}

#endif

#include "common/intrusive_ptr.h"
