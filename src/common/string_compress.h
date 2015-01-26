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
#include "common/string16.h"

#ifdef HAVE_ZLIB

namespace common
{
    bool encodeZlib(const std::string& data, std::string& out, int compressionlevel);
    bool decodeZlib(const std::string& data, std::string& out);
}  // namesapce base_icu

#endif
