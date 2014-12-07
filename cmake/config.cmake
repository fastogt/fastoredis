IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    ADD_DEFINITIONS(-DOS_LINUX -DOS_POSIX)
ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    ADD_DEFINITIONS(-DOS_MACOSX -DOS_POSIX)
ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    ADD_DEFINITIONS(-D_WIN32 -DOS_WIN)
ELSE()
    MESSAGE(FATAL_ERROR "Not supported OS: ${CMAKE_SYSTEM_NAME}")
ENDIF()

IF(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
    SET(CMAKE_COMPILER_IS_CLANGCXX 1)
ENDIF()

IF(CMAKE_COMPILER_IS_CLANGCXX)
    ADD_DEFINITIONS(-DCOMPILER_CLANG)
ELSEIF (CMAKE_COMPILER_IS_GNUCXX)
    ADD_DEFINITIONS(-DCOMPILER_GCC)
ELSEIF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
    ADD_DEFINITIONS(-DCOMPILER_INTEL)
ELSEIF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    ADD_DEFINITIONS(-DCOMPILER_MSVC)
ELSE()
    MESSAGE(FATAL_ERROR "Not supported compiler id: ${CMAKE_CXX_COMPILER_ID}")
ENDIF()

MESSAGE(STATUS "CMAKE_SYSTEM_PROCESSOR: ${CMAKE_SYSTEM_PROCESSOR}")
MESSAGE(STATUS "CMAKE_SYSTEM: ${CMAKE_SYSTEM}")
MESSAGE(STATUS "CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")

IF (CMAKE_BUILD_TYPE)
    STRING(TOUPPER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE_STABLE) 
    MESSAGE(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE_STABLE}")
ENDIF(CMAKE_BUILD_TYPE)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/")
IF(NOT CMAKE_DEBUG_POSTFIX)
    SET(CMAKE_DEBUG_POSTFIX d)
ENDIF()
INCLUDE(cmake/projecthelper.cmake)
INCLUDE(cmake/utils.cmake)

IF(SSH_ENABLED)
    ADD_DEFINITIONS(-DSSH_SUPPORT_ENABLED)
    IF(NOT OPENSSL_FOUND)
        MESSAGE(FATAL_ERROR "OPENSSH_ENABLED but openssl not founded, please specify OPENSSL_ROOT_DIR variable and check the OPENSSL_ENABLED variable must be ON.")
    ENDIF(NOT OPENSSL_FOUND)
ENDIF(SSH_ENABLED)

IF(BOOST_ENABLED)
    ADD_DEFINITIONS(-DBOOST_ENABLED)
    INCLUDE(${CMAKE_CURRENT_LIST_DIR}/integrate-boost.cmake)
    SET(Boost_USE_MULTITHREADED ON)
    SET(Boost_USE_STATIC_LIBS ON)
ENDIF(BOOST_ENABLED)

IF(PYTHON_ENABLED)
    ADD_DEFINITIONS(-DPYTHON_ENABLED)
ENDIF(PYTHON_ENABLED)

IF(QT_ENABLED)
    INCLUDE(${CMAKE_CURRENT_LIST_DIR}/integrate-qt.cmake)
    ADD_DEFINITIONS(-DQT_SUPPORT_ENABLED)
ENDIF(QT_ENABLED)

IF(DEVELOPER_ENABLE_TESTS)
    INCLUDE(cmake/testing.cmake)
    SETUP_TESTING()
ENDIF(DEVELOPER_ENABLE_TESTS)

MACRO(ADD_APP_EXECUTABLE_MSVC PROJECT_NAME SOURCES LIBS)
	ADD_EXECUTABLE(${PROJECT_NAME} ${DESKTOP_TARGET} ${SOURCES})
	TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${LIBS})
ENDMACRO()

MACRO(ADD_APP_LIBRARY_MSVC PROJECT_NAME SOURCES LIBS)
	ADD_LIBRARY(${PROJECT_NAME} STATIC ${SOURCES})
	TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${LIBS})
ENDMACRO()

MACRO(ADD_APP_EXECUTABLE PROJ_NAME SOURCES LIBS)
    SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${BUILD_TYPE}/build)

    SET(TARGET ${PROJ_NAME})
    ADD_EXECUTABLE(${TARGET} ${DESKTOP_TARGET} ${SOURCES})
    TARGET_LINK_LIBRARIES(${TARGET} ${LIBS})
    IF ("${CMAKE_BUILD_TYPE_STABLE}" STREQUAL "RELEASE")
        SET_TARGET_PROPERTIES(${TARGET} PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS_RELEASE}")
    ELSE()
        SET_TARGET_PROPERTIES(${TARGET} PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS_DEBUG}")
    ENDIF()
ENDMACRO()

MACRO(ADD_APP_LIBRARY LIB_NAME SOURCES LIBS)
    SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${BUILD_TYPE}/build)

    SET(TARGET_LIB ${LIB_NAME})
    ADD_LIBRARY(${TARGET_LIB} STATIC ${SOURCES})
    TARGET_LINK_LIBRARIES(${TARGET_LIB} ${LIBS})
    IF ("${CMAKE_BUILD_TYPE_STABLE}" STREQUAL "RELEASE")
        SET_TARGET_PROPERTIES(${TARGET_LIB} PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS_RELEASE}")
    ELSE()
        SET_TARGET_PROPERTIES(${TARGET_LIB} PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS_DEBUG}")
    ENDIF()
ENDMACRO()

MACRO(ADD_APP_LIBRARY_OBJECT LIB_NAME SOURCES LIBS LIB_OBJECTS)
    SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${BUILD_TYPE}/build)

    SET(TARGET_LIB ${LIB_NAME})
    ADD_LIBRARY(${TARGET_LIB} STATIC ${SOURCES} ${LIB_OBJECTS})
    TARGET_LINK_LIBRARIES(${TARGET_LIB} ${LIBS})
    IF ("${CMAKE_BUILD_TYPE_STABLE}" STREQUAL "RELEASE")
        SET_TARGET_PROPERTIES(${TARGET_LIB} PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS_RELEASE}")
    ELSE()
        SET_TARGET_PROPERTIES(${TARGET_LIB} PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS_DEBUG}")
    ENDIF()
ENDMACRO()
