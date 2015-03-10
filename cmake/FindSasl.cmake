# - Find SASL security library.
# This module defines
# SASL_INCLUDE_DIR, where to find SASL headers
# SASL_LIBRARIES, the library to use.
# SASL_FOUND, If false, do not try to use.

FIND_PATH(SASL_INCLUDE_DIR sasl/sasl.h HINTS $ENV{SASL_ROOT_DIR} PATH_SUFFIXES include)
FIND_LIBRARY(SASL_LIBRARIES NAMES sasl2 HINTS $ENV{SASL_ROOT_DIR} PATH_SUFFIXES lib)

IF(MINGW)
    FIND_PATH(SASL_INCLUDE_DIR
            sasl/sasl.h
        HINTS
            $ENV{MSYS_ROOT}/usr
            $ENV{SASL_ROOT_DIR}
        PATH_SUFFIXES
            include
    )
    FIND_LIBRARY(SASL_LIBRARIES
        NAMES
            sasl2
        HINTS
            $ENV{MSYS_ROOT}/usr
            $ENV{SASL_ROOT_DIR}
        PATH_SUFFIXES
            lib
    )
ENDIF(MINGW)


IF(SASL_LIBRARIES)
    find_package_handle_standard_args(Sasl
        REQUIRED_VARS
        SASL_LIBRARIES
        SASL_INCLUDE_DIR
        FAIL_MESSAGE
        "Could NOT find Sasl, try to set the path to Sasl root folder in the system variable SASL_ROOT_DIR"
    )
ENDIF(SASL_LIBRARIES)

mark_as_advanced(SASL_LIBRARIES SASL_INCLUDE_DIR)
