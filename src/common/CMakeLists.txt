CMAKE_MINIMUM_REQUIRED(VERSION 3.0.0)
PROJECT(common)

# use Boost
IF(BOOST_ENABLED)
    INTEGRATE_BOOST(system)
ENDIF(BOOST_ENABLED)

INCLUDE_DIRECTORIES(../)

IF(DATABASE_SUPPORT)
    ADD_DEFINITIONS(-DDATABASE_SUPPORT)
    #INCLUDE_DIRECTORIES(/usr/include/oracle/11.2/client)#$ENV{oracle_HOME}
    STRING(REPLACE "lib" "include" ORACLE_HOME_INCLUDE $ENV{ORACLE_HOME} )
    INCLUDE_DIRECTORIES(${ORACLE_HOME_INCLUDE})
    INCLUDE_DIRECTORIES(database/postgresql)
    SET(DATABASE_SUPPORT_SOURCES
            database/connection_info.h
            database/connection_info.cpp
            database/oracle_helper.h
            database/oracle_helper.cpp
            database/mysql_helper.h
            database/mysql_helper.cpp
            database/sqlite_helper.h
            database/sqlite_helper.cpp
            database/postgresql_helper.h
            database/postgresql_helper.cpp
            database/common_helper.h
            database/common_helper.cpp
    )
    #find_path(GLIB_INCLUDE_DIR NAMES glib.h HINTS ${GSTREAMER_SDK_ROOT}/include PATH_SUFFIXES glib-2.0)
    FIND_LIBRARY(DATABASE_SUPPORT_LIBS_ORACLE_OCCI occi  PATHS $ENV{ORACLE_HOME} PATH_SUFFIXES lib)
    FIND_LIBRARY(DATABASE_SUPPORT_LIBS_ORACLE_CLNTSH clntsh PATHS $ENV{ORACLE_HOME} PATH_SUFFIXES lib)
    FIND_LIBRARY(DATABASE_SUPPORT_LIBS_MYSQL mysqlcppconn)
    FIND_LIBRARY(DATABASE_SUPPORT_LIBS_SQLITE sqlite3)
    FIND_LIBRARY(DATABASE_SUPPORT_LIBS_POSTGRESQL pq)

    SET(DATABASE_SUPPORT_LIBS ${DATABASE_SUPPORT_LIBS_ORACLE_OCCI} ${DATABASE_SUPPORT_LIBS_ORACLE_CLNTSH} ${DATABASE_SUPPORT_LIBS_MYSQL} ${DATABASE_SUPPORT_LIBS_SQLITE} ${DATABASE_SUPPORT_LIBS_POSTGRESQL})
ENDIF(DATABASE_SUPPORT)

IF(SETTINGS_SUPPORT)
    IF(NOT BOOST_ENABLED)
        MESSAGE(FATAL_ERROR "Please enable boost support")
    ENDIF(NOT BOOST_ENABLED)
ENDIF(SETTINGS_SUPPORT)

IF(SETTINGS_SUPPORT AND BOOST_ENABLED)
    SET(STORAGES settings/storages/storages_def.h settings/storages/ini_xml_storage.h)
    SET(MPL_STRING settings/storages/mpl_string/initializer_vector.h settings/storages/mpl_string/mpl.h settings/storages/mpl_string/string_template.h)
    SET(SETTINGS_SOURCES settings/settings.h ${STORAGES} ${PLATFORM_DEPENDED_STORAGES} ${MPL_STRING} ${SETTINGS_HDR})
ENDIF(SETTINGS_SUPPORT AND BOOST_ENABLED)

IF(QT_ENABLED)
    SET(QT_UTILS_SOURCES
            qt/utils_qt.h
            qt/utils_qt.cpp
            qt/convert_string.cpp
            qt/convert_string.h
    )
ENDIF(QT_ENABLED)

SET(PATTERNS_SOURCES
        patterns/crtp_pattern.h
        patterns/singleton_pattern.h
)

SET(MULTI_THREADING_SOURCES
        multi_threading/types.h
        multi_threading/thread_pool.h
        multi_threading/std_thread_pool_impl.hpp
)

SET(COMMON_SOURCES
    log_levels.h logger.h logger.cpp common_config.h basictypes.h string_compress.h string_compress.cpp
    safe_sprintf.h safe_sprintf.cpp sprintf.h sprintf.cpp utf_string_conversions.h utf_string_conversions.cpp utf_string_conversion_utils.h utf_string_conversion_utils.cpp
    string_util.h string_util.cpp string_util_constants.cpp icu_utf.h icu_utf.cpp string_piece.h string_piece.cpp string16.h string16.cpp hash_tables.h
    convert2string.h convert2string.cpp types.h types.cpp lexical_cast.h lexical_cast.cpp smart_ptr.h
    macros.h macros.cpp utils.h utils.cpp file_system.h file_system.cpp value.h value.cpp
    intrusive_ptr.h url.h url.cpp memory_string.h memory_string.cpp time.h time.cpp net/net.h net/net.cpp net/socket_tcp.h net/socket_tcp.cpp
    ${QT_UTILS_SOURCES} ${PATTERNS_SOURCES} ${DATABASE_SUPPORT_SOURCES} ${SETTINGS_SOURCES} ${MULTI_THREADING_SOURCES})

SET(COMMON_LIBS ${DATABASE_SUPPORT_LIBS})

IF(BOOST_ENABLED)
    SET(COMMON_LIBS ${COMMON_LIBS} ${Boost_LIBRARIES})
ENDIF(BOOST_ENABLED)

IF(MINGW OR CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANGCXX)
    ADD_APP_STATIC_LIBRARY (${PROJECT_NAME} "${COMMON_SOURCES}" "${COMMON_LIBS}")
ENDIF(MINGW OR CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANGCXX)
