MACRO(SET_DESKTOP_TARGET)
	# Set default target to build (under win32 its allows console debug)
    IF(WIN32)
        OPTION(DEVELOPER_WINCONSOLE "Enable windows console windows for debug" OFF)
        IF(DEVELOPER_FEATURES AND DEVELOPER_WINCONSOLE)
            SET(DESKTOP_TARGET "")
        ELSE(DEVELOPER_FEATURES AND DEVELOPER_WINCONSOLE)
            SET(DESKTOP_TARGET WIN32)
        ENDIF(DEVELOPER_FEATURES AND DEVELOPER_WINCONSOLE)
    ELSEIF(APPLE)
        SET(DESKTOP_TARGET MACOSX_BUNDLE)
    ENDIF(WIN32)
ENDMACRO(SET_DESKTOP_TARGET)

FUNCTION(PROJECT_GET_GIT_VERSION PROJECT_GIT_VER)
    EXECUTE_PROCESS(COMMAND git show-ref --head --hash=8 2
                    COMMAND sed s/^v//
                    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
                    OUTPUT_VARIABLE PROJECT_GIT_TEMP_VERSION)
    STRING(REGEX REPLACE "(\r?\n)+$" "" PROJECT_GIT_TEMP_VERSION "${PROJECT_GIT_TEMP_VERSION}")
    MESSAGE(STATUS "${PROJECT_NAME} git version: ${PROJECT_GIT_TEMP_VERSION}")
    SET(${PROJECT_GIT_VER} ${PROJECT_GIT_TEMP_VERSION} PARENT_SCOPE)
ENDFUNCTION()

MACRO(DEFINE_DEFAULT_DEFINITIONS)
        ADD_DEFINITIONS(-D__STDC_FORMAT_MACROS)
    IF(WIN32)
        ADD_DEFINITIONS(
        -DNOMINMAX # do not define min() and max()
        -D_CRT_SECURE_NO_WARNINGS 
        -D_CRT_NONSTDC_NO_WARNINGS 
        -D_CRT_SECURE_NO_WARNINGS
        #-DWIN32_LEAN_AND_MEAN # remove obsolete things from windows headers
        )
    ENDIF(WIN32)
    PROJECT_GET_GIT_VERSION(PROJECT_GIT_VERSION)
    ADD_DEFINITIONS(
        -DPROJECT_NAME="${PROJECT_NAME}"
        -DWCHAR_PROJECT_NAME=L"${PROJECT_NAME}"
        -DPROJECT_NAME_TITLE="${PROJECT_NAME_TITLE}"
        -DPROJECT_COPYRIGHT="${PROJECT_COPYRIGHT}"
        -DPROJECT_DOMAIN="${PROJECT_DOMAIN}" 
        -DPROJECT_COMPANYNAME="${PROJECT_COMPANYNAME}"
        -DPROJECT_COMPANYNAME_DOMAIN="${PROJECT_COMPANYNAME_DOMAIN}"
        -DPROJECT_GITHUB_FORK="${PROJECT_GITHUB_FORK}"
        -DPROJECT_GITHUB_ISSUES="${PROJECT_GITHUB_ISSUES}"
        -DPROJECT_VERSION="${PROJECT_VERSION}"
        -DPROJECT_NAME_LOWERCASE="${PROJECT_NAME_LOWERCASE}"
        -DPROJECT_NAME_UPPERRCASE="${PROJECT_NAME_UPPERRCASE}"
        -DPROJECT_GIT_VERSION="${PROJECT_GIT_VERSION}"
        -D${PROJECT_NAME_UPPERRCASE}
        -DPROJECT_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
        -DPROJECT_VERSION_MINOR=${PROJECT_VERSION_MINOR}
        -DPROJECT_VERSION_PATCH=${PROJECT_VERSION_PATCH}
        -DPROJECT_VERSION_TWEAK=${PROJECT_VERSION_TWEAK}
        )
    IF(DEVELOPER_FEATURES)
        ADD_DEFINITIONS(-DDEVELOPER_FEATURES)
        IF(WIN32)
            SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG")
        ENDIF(WIN32)
    ENDIF(DEVELOPER_FEATURES)
    IF(MSVC)
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Zi")
    ENDIF(MSVC)
ENDMACRO(DEFINE_DEFAULT_DEFINITIONS)

MACRO(SETUP_COMPILER_SETTINGS IS_DYNAMIC)
    IF(MSVC)
        SET(ADDITIONAL_CL_OPTIMIZATION_OPTIONS
        #	/Gr # fastcall
            /Gy # function level linking
            /GF # string pooling
            /GL # whole program optimization
            #/FR # disable browserable info
            /Oi # intrinsic functions
            /Ot # fast code
            /Ob2 # inline expansion
            /Ox  # full optimization
            #/arch:SSE2
            /fp:except- /fp:fast
        )

        SET(ADDITIONAL_CL_OPTIMIZATION_OPTIONS_PROJECTNAME
        #	/Gd  # cdecl
            /Os  # small code
            /Og  # Turn on loop, common subexpression and register optimizations
            /Ob0 # do not inline
            /O2  # maximize speed
            /fp:precise
        )

        SET(ADDITIONAL_LINKER_OPTIMIZATION_OPTIONS
            /INCREMENTAL:NO
            /LTCG
            /DEBUG
        )
    ENDIF()
    
    SET(IS_DYNAMIC ${IS_DYNAMIC})
    STRING(REPLACE ";" " " cmake_cl_release_init_str "${ADDITIONAL_CL_OPTIMIZATION_OPTIONS} /D NDEBUG")
    STRING(REPLACE ";" " " cmake_linker_release_init_str "${ADDITIONAL_LINKER_OPTIMIZATION_OPTIONS}")
    IF(IS_DYNAMIC)
        SET(makeRulesOwerrideContent "
            IF(MSVC)
                SET(CMAKE_C_FLAGS_DEBUG_INIT 			\"/D_DEBUG /MDd /Zi /Ob0 /Od /RTC1\")
                SET(CMAKE_C_FLAGS_MINSIZEREL_INIT     	\"/MD /O1 /Ob1 /D NDEBUG\")
                SET(CMAKE_C_FLAGS_RELEASE_INIT       	\"/MD /O2 /Ob2 /D NDEBUG\")
                SET(CMAKE_C_FLAGS_RELWITHDEBINFO_INIT 	\"/MD /Zi /O2 /Ob1 /D NDEBUG\")
                
                SET(CMAKE_CXX_FLAGS_DEBUG_INIT 			\"/D_DEBUG /MDd /Zi /Ob0 /Od /EHsc /RTC1\")
                SET(CMAKE_CXX_FLAGS_MINSIZEREL_INIT     \"/MD /O1 ${cmake_cl_release_init_str}\")
                SET(CMAKE_CXX_FLAGS_RELEASE_INIT        \"/MD /O2 ${cmake_cl_release_init_str}\")
                SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO_INIT \"/MD /Zi /O2 /Ob1 /D NDEBUG /EHsc\")

                SET(CMAKE_EXE_LINKER_FLAGS_MINSIZEREL_INIT		\"${cmake_linker_release_init_str}\")
                SET(CMAKE_EXE_LINKER_FLAGS_RELEASE_INIT			\"${cmake_linker_release_init_str}\")
                SET(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO_INIT	\"${cmake_linker_release_init_str}\")
            
            ENDIF(MSVC)
        "		)

        IF(MSVC)
            SET(CMAKE_CXX_FLAGS_DEBUG "/MDd /D_DEBUG /Zi /Ob0 /Od /RTC1")
            SET(CMAKE_CXX_FLAGS_RELEASE "/MD /O2 ${cmake_cl_release_init_str}")
            SET(CMAKE_CXX_FLAGS_MINSIZEREL "/MD /O1 ${cmake_cl_release_init_str}")
            SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/MD /O2 /Zi ${cmake_cl_release_init_str}")
            SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${cmake_linker_release_init_str}")
            SET(CMAKE_EXE_LINKER_FLAGS_MINSIZEREL "${cmake_linker_release_init_str}")
            SET(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "${cmake_linker_release_init_str}")
        ENDIF(MSVC)
        
        MESSAGE(STATUS "Using setting for DYNAMIC run-time")
    else(IS_DYNAMIC)
        SET(makeRulesOwerrideContent "
        IF(MSVC)
            SET(CMAKE_C_FLAGS_INIT \"/MT\")
            SET(CMAKE_CXX_FLAGS_INIT \"/MT /EHsc\")

            SET(CMAKE_C_FLAGS_DEBUG_INIT 			\"/D_DEBUG /MTd /Zi /Ob0 /Od /RTC1\")
            SET(CMAKE_C_FLAGS_MINSIZEREL_INIT     	\"/MT /O1 /Ob1 /D NDEBUG\")
            SET(CMAKE_C_FLAGS_RELEASE_INIT       	\"/MT /O2 /Ob2 /D NDEBUG\")
            SET(CMAKE_C_FLAGS_RELWITHDEBINFO_INIT 	\"/MT /Zi /O2 /Ob1 /D NDEBUG\")

            SET(CMAKE_CXX_FLAGS_DEBUG_INIT 			\"/D_DEBUG /MTd /Zi /Ob0 /Od /EHsc /RTC1\")
            SET(CMAKE_CXX_FLAGS_MINSIZEREL_INIT     \"/MT /O1 ${cmake_cl_release_init_str}\")
            SET(CMAKE_CXX_FLAGS_RELEASE_INIT        \"/MT /O2 ${cmake_cl_release_init_str}\")
            SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO_INIT \"/MT /Zi /O2 /Ob1 /D NDEBUG /EHsc\")

            SET(CMAKE_EXE_LINKER_FLAGS_MINSIZEREL_INIT		\"${cmake_linker_release_init_str}\")
            SET(CMAKE_EXE_LINKER_FLAGS_RELEASE_INIT			\"${cmake_linker_release_init_str}\")
            SET(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO_INIT	\"${cmake_linker_release_init_str}\")
        ENDIF(MSVC)
        "		)

        IF(MSVC)
            SET(CMAKE_CXX_FLAGS_DEBUG "/MTd /D_DEBUG /Zi /Ob0 /Od /RTC1")
            SET(CMAKE_CXX_FLAGS_RELEASE "/MT /O2 ${cmake_cl_release_init_str}")
            SET(CMAKE_CXX_FLAGS_MINSIZEREL "/MT /O1 ${cmake_cl_release_init_str}")
            SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/MT /O2 /Zi ${cmake_cl_release_init_str}")
            SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${cmake_linker_release_init_str}")
            SET(CMAKE_EXE_LINKER_FLAGS_MINSIZEREL "${cmake_linker_release_init_str}")
            SET(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "${cmake_linker_release_init_str}")
        ENDIF(MSVC)

        MESSAGE(STATUS "Using setting for STATIC run-time")

        ADD_DEFINITIONS(-DQT_STATICPLUGINS)
    ENDIF(IS_DYNAMIC)

    FILE(WRITE "${CMAKE_CURRENT_BINARY_DIR}/makeRulesOwerride.cmake" "${makeRulesOwerrideContent}")
    SET(CMAKE_USER_MAKE_RULES_OVERRIDE ${CMAKE_CURRENT_BINARY_DIR}/makeRulesOwerride.cmake)
ENDMACRO(SETUP_COMPILER_SETTINGS IS_DYNAMIC)

MACRO(INSTALL_RUNTIME_LIBRARIES)
    # Install CRT
    SET(CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION .)
    IF(MINGW)
        GET_FILENAME_COMPONENT(Mingw_Path ${CMAKE_CXX_COMPILER} PATH)
        SET(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS ${Mingw_Path}/libgcc_s_seh-1.dll ${Mingw_Path}/libstdc++-6.dll ${Mingw_Path}/libwinpthread-1.dll)
    ENDIF(MINGW)
    INCLUDE(InstallRequiredSystemLibraries)
ENDMACRO(INSTALL_RUNTIME_LIBRARIES)

FUNCTION(FIND_RUNTIME_LIBRARY OUT_VAR DLIB_NAMES)
    IF(MINGW)
        GET_FILENAME_COMPONENT(Mingw_Path ${CMAKE_CXX_COMPILER} PATH)
        FIND_LIBRARY(OUT_VAR_FIND_${OUT_VAR} NAMES ${${DLIB_NAMES}} HINTS ${Mingw_Path} $ENV{MSYS_ROOT}/usr/bin)
        IF(NOT OUT_VAR_FIND_${OUT_VAR})
            MESSAGE(WARNING "FIND_RUNTIME_LIBRARY not found ${OUT_VAR}")
        ELSE()
            MESSAGE(STATUS "FIND_RUNTIME_LIBRARY found ${OUT_VAR_FIND_${OUT_VAR}}")
        ENDIF()
        SET(${OUT_VAR} ${OUT_VAR_FIND_${OUT_VAR}} PARENT_SCOPE)
    ELSE()
        MESSAGE(FATAL_ERROR "Not implemented")
    ENDIF(MINGW)
ENDFUNCTION()

MACRO(ADD_DLIB_TO_POSTBUILD_STEP TARGET_NAME DLIB_NAME)
IF(MSVC OR APPLE)
        GET_TARGET_PROPERTY(dlib_location ${DLIB_NAME} LOCATION)
        GET_FILENAME_COMPONENT(dlib_targetdir ${dlib_location} PATH)
        SET(DLIB_DEBUG ${dlib_targetdir}/${CMAKE_SHARED_LIBRARY_PREFIX}${DLIB_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX})
        SET(DLIB_RELEASE ${dlib_targetdir}/${CMAKE_SHARED_LIBRARY_PREFIX}${DLIB_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX})
        ADD_CUSTOM_COMMAND(TARGET ${TARGET_NAME} POST_BUILD COMMAND
		 ${CMAKE_COMMAND} -E copy $<$<CONFIG:Debug>:${DLIB_DEBUG}> $<$<NOT:$<CONFIG:Debug>>:${DLIB_RELEASE}>  $<TARGET_FILE_DIR:${TARGET_NAME}>
        )
ENDIF(MSVC OR APPLE)
ENDMACRO(ADD_DLIB_TO_POSTBUILD_STEP)

MACRO(INSTALL_DEBUG_INFO_FILE)
    IF(WIN32)
        #install pbd files
            FOREACH(buildCfg ${CMAKE_CONFIGURATION_TYPES})
                INSTALL(
                FILES ${CMAKE_CURRENT_BINARY_DIR}/${buildCfg}/${PROJECT_NAME}.pdb
                DESTINATION .
                CONFIGURATIONS ${buildCfg})
            ENDFOREACH(buildCfg ${CMAKE_CONFIGURATION_TYPES})
    ENDIF(WIN32)
# TODO: add code for other platforms here
ENDMACRO(INSTALL_DEBUG_INFO_FILE)


MACRO(TARGET_BUNDLEFIX TARGET_NAME LIB_DIST)
    IF(APPLE)
        INSTALL(CODE "
        FILE(GLOB_RECURSE QTPLUGINS_FIXUP
        \"\${CMAKE_INSTALL_PREFIX}/*${CMAKE_SHARED_LIBRARY_SUFFIX}\")
        include(BundleUtilities)
        fixup_bundle(\"\${CMAKE_INSTALL_PREFIX}/${BUNDLE_NAME}\" \"\${QTPLUGINS_FIXUP}\" \"\${CMAKE_INSTALL_PREFIX}/${LIB_DIST}\")
        " COMPONENT Runtime)
    ENDIF(APPLE)
ENDMACRO(TARGET_BUNDLEFIX TARGET_NAME)

IF(APPLE)
    WRITE_FILE(${CMAKE_CURRENT_BINARY_DIR}/src/bundle_config.cmake "") # Stub for file
    MACRO(APPEND_DYNAMIC_LIB LIBRARY)
        GET_FILENAME_COMPONENT(LIBRARY_PATH ${LIBRARY} PATH)
        GET_FILENAME_COMPONENT(LIBRARY_NAME ${LIBRARY} NAME)

        GET_TARGET_PROPERTY(projLocation ${PROJECT_NAME} LOCATION)
        STRING(REPLACE "/Contents/MacOS/${PROJECT_NAME}" "" MACOSX_BUNDLE_LOCATION ${projLocation})
        STRING(REPLACE "$(CONFIGURATION)$(EFFECTIVE_PLATFORM_NAME)" "$(CONFIGURATION)" APPS ${MACOSX_BUNDLE_LOCATION})

        ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD COMMAND cp \"${LIBRARY}\" \"${APPS}/Contents/MacOS/\")
        ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD COMMAND install_name_tool -change \"${LIBRARY}\" \"@executable_path/../MacOS/${LIBRARY_NAME}\" \"${projLocation}\")
        SET(BUNDLE_LIBRARIES_MOVE "${BUNDLE_LIBRARIES_MOVE};${APPS}/Contents/MacOS/${LIBRARY_NAME}")
        SET(BUNDLE_DIRECTORY_MOVE "${BUNDLE_DIRECTORY_MOVE};${LIBRARY_PATH}")
        # override fix_bundle config
        WRITE_FILE(${CMAKE_CURRENT_BINARY_DIR}/bundle_config.cmake "
            set(BUNDLE_LIBRARIES_MOVE \"${BUNDLE_LIBRARIES_MOVE}\")
            set(BUNDLE_DIRECTORY_MOVE \"${BUNDLE_DIRECTORY_MOVE}\")
        ")
    ENDMACRO(APPEND_DYNAMIC_LIB)
ENDIF(APPLE)
