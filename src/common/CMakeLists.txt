SET (PROJECT_COMMON_LIB common)
PROJECT (${PROJECT_COMMON_LIB})
# use Boost
IF(BOOST_ENABLED)
    INTEGRATE_BOOST(program_options)
ENDIF(BOOST_ENABLED)

INCLUDE_DIRECTORIES(../)

IF(DATABASE_SUPPORT)
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
ADD_DEFINITIONS(-DDATABASE_SUPPORT)
ENDIF()

IF(SETTINGS_SUPPORT)
IF(NOT BOOST_ENABLED)
    MESSAGE(FATAL_ERROR "Please enable boost support")
ENDIF(NOT BOOST_ENABLED)
ENDIF(SETTINGS_SUPPORT)

IF(SETTINGS_SUPPORT AND BOOST_ENABLED)
    IF(WIN32)
        SET(PLATFORM_DEPENDED_STORAGES settings/storages/registry_storage.hpp)
    ENDIF()
    SET(STORAGES settings/storages/storages_def.h settings/storages/memory_storage.hpp settings/storages/xml_storage.hpp settings/storages/ini_storage.hpp)
    SET(MPL_STRING settings/storages/mpl_string/initializer_vector.h settings/storages/mpl_string/mpl.h settings/storages/mpl_string/string_template.h)
    SET(SETTINGS_SOURCES settings/settings.hpp ${STORAGES} ${PLATFORM_DEPENDED_STORAGES} ${MPL_STRING} ${SETTINGS_HDR})
ENDIF(SETTINGS_SUPPORT AND BOOST_ENABLED)

IF(QT_ENABLED)
SET(QT_UTILS_SOURCES
        qt_helper/utils_qt.h
        qt_helper/utils_qt.cpp
        qt_helper/converter_patterns.cpp
        qt_helper/converter_patterns.h)
ENDIF()
SET(PATTERNS_SOURCES
        patterns/command_pattern.hpp
        patterns/crtp_pattern.hpp
        patterns/factory_pattern.hpp
        patterns/factory_method.hpp
        patterns/observer_pattern.hpp
        patterns/singleton_pattern.hpp)
SET(FILE_PARSERS file_parsers/parser_ini.h file_parsers/parser_ini.cpp)
SET(MULTI_THREADING_SOURCES 
        multi_threading/lockers.h
        multi_threading/lockers.cpp
        multi_threading/thread_safe_containers.hpp
        multi_threading/common_headers.hpp
        multi_threading/thread_adapter.hpp
        multi_threading/thread_pool.hpp
        multi_threading/std_thread_pool_impl.hpp
        multi_threading/pthread_thread_pool_impl.hpp
        multi_threading/pthread_wrapper.hpp)
IF(BOOST_ENABLED)
    SET(MULTI_THREADING_SOURCES ${MULTI_THREADING_SOURCES} multi_threading/boost_thread_pool_impl.hpp)
ENDIF(BOOST_ENABLED)

SET(COMMON_SOURCES runtime_bitset.h runtime_bitset.cpp log_levels.hpp logger.h logger.cpp common_config.h
macros.h macros.cpp mpl.hpp utils.h utils.cpp file_system.h file_system.cpp
boost_extension.hpp smart_ptr.hpp url.h url.cpp memory_string.h memory_string.cpp time.h time.cpp
${QT_UTILS_SOURCES} ${PATTERNS_SOURCES} ${FILE_PARSERS} ${DATABASE_SUPPORT_SOURCES} ${SETTINGS_SOURCES} ${MULTI_THREADING_SOURCES})

SET(COMMON_LIBS ${DATABASE_SUPPORT_LIBS})
IF(BOOST_ENABLE)
SET(COMMON_LIBS "${COMMON_LIBS} ${Boost_LIBRARIES}")
ENDIF(BOOST_ENABLE)

IF(MINGW OR CMAKE_COMPILER_IS_GNUCXX )
ADD_APP_LIBRARY (${PROJECT_COMMON_LIB} "${COMMON_SOURCES}" "${COMMON_LIBS}")
ENDIF(MINGW OR CMAKE_COMPILER_IS_GNUCXX )
