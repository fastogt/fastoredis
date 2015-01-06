# - Find SASL security library.
# This module defines
# SASL_INCLUDE_DIR, where to find SASL headers
# SASL_LIBRARIES, the library to use.
# SASL_FOUND, If false, do not try to use.

FIND_PATH(SASL_INCLUDE_DIR sasl/sasl.h)
FIND_LIBRARY(SASL_LIBRARIES sasl2)

IF(MINGW)
    FIND_PATH(SASL_INCLUDE_DIR
            sasl/sasl.h
        HINTS
            $ENV{MSYS_ROOT}/usr
        PATH_SUFFIXES
            include
    )
    FIND_LIBRARY(SASL_LIBRARIES
        NAMES
            sasl2
        HINTS
            $ENV{MSYS_ROOT}/usr
        PATH_SUFFIXES
            lib
    )
ENDIF(MINGW)


IF(SASL_LIB_PATH)
    SET(SASL_FOUND TRUE)
ELSE(SASL_LIB_PATH)
    SET(SASL_FOUND FALSE)
ENDIF(SASL_LIB_PATH)

mark_as_advanced(SASL_LIBRARIES SASL_INCLUDE_DIR)
