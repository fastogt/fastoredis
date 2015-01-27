/*
*******************************************************************************
*
*   Copyright (C) 1999-2004, International Business Machines
*   Corporation and others.  All Rights Reserved.
*
*******************************************************************************
*   file name:  utf.h
*   encoding:   US-ASCII
*   tab size:   8 (not used)
*   indentation:4
*
*   created on: 1999sep09
*   created by: Markus W. Scherer
*/

#pragma once

#include "common/basictypes.h"
#include "common/value.h"

#ifdef HAVE_ZLIB

#include "zlib.h"

namespace common
{
    ErrorValueSPtr encodeZlib(const std::string& data, std::string& out, int compressionlevel = Z_BEST_COMPRESSION) WARN_UNUSED_RESULT;
    ErrorValueSPtr decodeZlib(const std::string& data, std::string& out) WARN_UNUSED_RESULT;
}  // namesapce base_icu

#endif
