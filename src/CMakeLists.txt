SET(HEADERS_GLOBAL
    global/global.h
    global/types.h
)
SET(SOURCES_GLOBAL
    global/global.cpp
    global/types.cpp
)

SET(HEADERS_GUI_TO_MOC
    gui/main_window.h
    gui/app_style.h
    gui/widgets/log_tab_widget.h
    gui/widgets/log_widget.h
    gui/widgets/commands_widget.h
    gui/property_table_model.h
    gui/explorer/explorer_tree_view.h
    gui/explorer/explorer_tree_model.h
    gui/dialogs/about_dialog.h
    gui/dialogs/input_dialog.h
    gui/dialogs/info_server_dialog.h
    gui/dialogs/history_server_dialog.h
    gui/dialogs/property_server_dialog.h
    gui/dialogs/preferences_dialog.h
    gui/dialogs/connections_dialog.h
    gui/dialogs/connection_dialog.h
    gui/dialogs/cluster_dialog.h
    gui/dialogs/connection_diagnostic_dialog.h
    gui/dialogs/discovery_dialog.h
    gui/dialogs/python_console_dialog.h
    gui/dialogs/lua_console_dialog.h
    gui/dialogs/base_console_dialog.h
    gui/dialogs/encode_decode_dialog.h
    gui/dialogs/load_contentdb_dialog.h
    gui/dialogs/create_dbkey_dialog.h
    gui/widgets/main_widget.h
    gui/main_tab_bar.h
    gui/fasto_scintilla.h
    gui/fasto_editor.h
    gui/fasto_text_view.h
    gui/widgets/query_widget.h
    gui/widgets/output_widget.h
    gui/fasto_tree_view.h
    gui/fasto_common_model.h
    gui/fasto_table_view.h
    gui/icon_label.h
    gui/glass_widget.h
    gui/base/tree_model.h
    gui/base/table_model.h
    gui/base/graph_widget.h
)
SET(HEADERS_GUI
    gui/dialogs/connection_listwidget_items.h
    gui/gui_factory.h
    gui/shortcuts.h
    gui/fasto_common_item.h
    gui/base/tree_item.h
    gui/base/table_item.h
)
SET(SOURCES_GUI
    gui/main_window.cpp
    gui/app_style.cpp
    gui/gui_factory.cpp
    gui/shortcuts.cpp
    gui/widgets/log_tab_widget.cpp
    gui/widgets/log_widget.cpp
    gui/widgets/commands_widget.cpp
    gui/property_table_model.cpp
    gui/dialogs/about_dialog.cpp
    gui/dialogs/input_dialog.cpp
    gui/dialogs/preferences_dialog.cpp
    gui/dialogs/connections_dialog.cpp
    gui/dialogs/connection_dialog.cpp
    gui/dialogs/cluster_dialog.cpp
    gui/dialogs/connection_diagnostic_dialog.cpp
    gui/dialogs/discovery_dialog.cpp
    gui/dialogs/connection_listwidget_items.cpp
    gui/dialogs/info_server_dialog.cpp
    gui/dialogs/property_server_dialog.cpp
    gui/dialogs/history_server_dialog.cpp
    gui/dialogs/python_console_dialog.cpp
    gui/dialogs/lua_console_dialog.cpp
    gui/dialogs/base_console_dialog.cpp
    gui/dialogs/encode_decode_dialog.cpp
    gui/dialogs/load_contentdb_dialog.cpp
    gui/dialogs/create_dbkey_dialog.cpp
    gui/widgets/main_widget.cpp
    gui/main_tab_bar.cpp
    gui/fasto_scintilla.cpp
    gui/fasto_editor.cpp
    gui/fasto_text_view.cpp
    gui/widgets/query_widget.cpp
    gui/widgets/output_widget.cpp
    gui/explorer/explorer_tree_view.cpp
    gui/explorer/explorer_tree_model.cpp
    gui/fasto_tree_view.cpp
    gui/fasto_common_model.cpp
    gui/fasto_table_view.cpp
    gui/fasto_common_item.cpp
    gui/icon_label.cpp
    gui/glass_widget.cpp
    gui/base/tree_model.cpp
    gui/base/tree_item.cpp
    gui/base/table_model.cpp
    gui/base/table_item.cpp
    gui/base/graph_widget.cpp
)

SET(HEADERS_CORE_TO_MOC
    core/command_logger.h
    core/logger.h
    core/idriver.h
    core/iserver.h
    core/servers_manager.h
    core/python_engine.h
    core/lua_engine.h
)
SET(HEADERS_CORE
    core/connection_types.h
    core/core_fwd.h
    core/icluster.h
    core/idatabase.h
    core/settings_manager.h
    core/connection_settings.h
    core/events/events.h
    core/events/events_info.h
    core/types.h
    core/ssh_info.h
    core/scp_file_transfer.h
    core/iedcoder.h
    core/base64_edcoder.h
    core/hex_edcoder.h
    core/html_edcoder.h
    core/compress_edcoder.h
    core/msgpack_edcoder.h
)
SET(SOURCES_CORE
    core/events/events_info.cpp
    core/command_logger.cpp
    core/connection_types.cpp
    core/settings_manager.cpp
    core/connection_settings.cpp
    core/logger.cpp
    core/idriver.cpp
    core/iserver.cpp
    core/icluster.cpp
    core/idatabase.cpp
    core/servers_manager.cpp
    core/types.cpp
    core/ssh_info.cpp
    core/python_engine.cpp
    core/lua_engine.cpp
    core/scp_file_transfer.cpp
    core/iedcoder.cpp
    core/base64_edcoder.cpp
    core/hex_edcoder.cpp
    core/html_edcoder.cpp
    core/compress_edcoder.cpp
    core/msgpack_edcoder.cpp
    third-party/cmp/cmp.c
)

SET(HEADERS_SHELL_TO_MOC
    shell/shell_widget.h
    shell/redis_shell.h
    shell/redis_lexer.h
    shell/memcached_shell.h
    shell/memcached_lexer.h
    shell/ssdb_lexer.h
    shell/ssdb_shell.h
    shell/python_shell.h
    shell/lua_shell.h
)
SET(HEADERS_SHELL
)
SET(SOURCES_SHELL
    shell/shell_widget.cpp
    shell/redis_shell.cpp
    shell/redis_lexer.cpp
    shell/memcached_shell.cpp
    shell/memcached_lexer.cpp
    shell/ssdb_lexer.cpp
    shell/ssdb_shell.cpp
    shell/python_shell.cpp
    shell/lua_shell.cpp
)

SET(HEADERS_TRANSLATIONS
    translations/translations.h
    translations/global.h
)
SET(SOURCES_TRANSLATIONS
    translations/translations.cpp
    translations/global.cpp
)

SET_DESKTOP_TARGET()

IF(OS_WINDOWS)
    SET(PLATFORM_HDRS)
    SET(PLATFORM_SRCS)
    SET(PLATFORM_LIBRARIES ws2_32)
ELSEIF(OS_MACOSX)
    FIND_LIBRARY(COCOA_LIBRARY Cocoa)
    SET(PLATFORM_LIBRARIES ${COCOA_LIBRARY})
ELSEIF(OS_LINUX)
    SET(PLATFORM_HDRS)
    SET(PLATFORM_SRCS)
    SET(PLATFORM_LIBRARIES dl)
ENDIF(OS_WINDOWS)

ADD_SUBDIRECTORY(common)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_LIST_DIR} ${OPENSSL_INCLUDE_DIR})

ADD_SUBDIRECTORY(third-party/libssh2)
ADD_SUBDIRECTORY(third-party/redis)
ADD_SUBDIRECTORY(third-party/qscintilla/Qt4Qt5)

#redis
SET(HEADERS_REDIS_TO_MOC
    core/redis/redis_cluster.h
    core/redis/redis_server.h
    core/redis/redis_driver.h
)
SET(HEADERS_REDIS
    core/redis/redis_infos.h
    core/redis/redis_config.h
    core/redis/redis_database.h
    core/redis/redis_settings.h
    core/redis/redis_cluster_settings.h
)
SET(SOURCES_REDIS
    core/redis/redis_config.cpp
    core/redis/redis_infos.cpp
    core/redis/redis_cluster.cpp
    core/redis/redis_server.cpp
    core/redis/redis_driver.cpp
    core/redis/redis_database.cpp
    core/redis/redis_settings.cpp
    core/redis/redis_cluster_settings.cpp
)

#memcached
FIND_PACKAGE(Sasl QUIET)
IF(SASL_FOUND)
    ADD_DEFINITIONS(-DHAVE_LIBSASL)
    INCLUDE_DIRECTORIES(${SASL_INCLUDE_DIR})
ENDIF(SASL_FOUND)

ADD_SUBDIRECTORY(third-party/memcached)

SET(HEADERS_MEMCACHED_TO_MOC
    core/memcached/memcached_server.h
    core/memcached/memcached_driver.h
)
SET(HEADERS_MEMCACHED
    core/memcached/memcached_infos.h
    core/memcached/memcached_config.h
    core/memcached/memcached_database.h
    core/memcached/memcached_settings.h
)
SET(SOURCES_MEMCACHED
    core/memcached/memcached_config.cpp
    core/memcached/memcached_infos.cpp
    core/memcached/memcached_server.cpp
    core/memcached/memcached_driver.cpp
    core/memcached/memcached_database.cpp
    core/memcached/memcached_settings.cpp
)

#ssdb

ADD_SUBDIRECTORY(third-party/ssdb)

SET(HEADERS_SSDB_TO_MOC
    core/ssdb/ssdb_server.h
    core/ssdb/ssdb_driver.h
)
SET(HEADERS_SSDB
    core/ssdb/ssdb_infos.h
    core/ssdb/ssdb_config.h
    core/ssdb/ssdb_database.h
    core/ssdb/ssdb_settings.h
)
SET(SOURCES_SSDB
    core/ssdb/ssdb_config.cpp
    core/ssdb/ssdb_infos.cpp
    core/ssdb/ssdb_server.cpp
    core/ssdb/ssdb_driver.cpp
    core/ssdb/ssdb_database.cpp
    core/ssdb/ssdb_settings.cpp
)

ADD_SUBDIRECTORY(third-party/lua)
INCLUDE_DIRECTORIES(third-party/lua/lua-5.3.0/src)

IF(PYTHON_ENABLED)
    ADD_SUBDIRECTORY(third-party/pytonqt)
    INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIR} third-party/pytonqt/src)
ENDIF(PYTHON_ENABLED)

SET(HEADERS ${HEADERS} ${HEADERS_GLOBAL} ${HEADERS_GUI} ${HEADERS_CORE} ${HEADERS_SHELL} ${HEADERS_REDIS} ${HEADERS_MEMCACHED} ${HEADERS_SSDB} ${HEADERS_TRANSLATIONS})
SET(SOURCES ${SOURCES} ${SOURCES_GLOBAL} ${SOURCES_GUI} ${SOURCES_CORE} ${SOURCES_SHELL} ${SOURCES_REDIS} ${SOURCES_MEMCACHED} ${SOURCES_SSDB} ${SOURCES_TRANSLATIONS})
SET(HEADERS_TOMOC ${HEADERS_GUI_TO_MOC} ${HEADERS_CORE_TO_MOC} ${HEADERS_SHELL_TO_MOC} ${HEADERS_REDIS_TO_MOC} ${HEADERS_MEMCACHED_TO_MOC} ${HEADERS_SSDB_TO_MOC})

QTX_WRAP_CPP(MOC_FILES ${HEADERS_TOMOC})
CONFIGURE_FILE("resources/gui.qrc.in" "${CMAKE_CURRENT_LIST_DIR}/resources/gui.qrc" @ONLY IMMEDIATE)
SET(RESOURCES ${CMAKE_CURRENT_LIST_DIR}/resources/gui.qrc)
QTX_ADD_RESOURCES(RES_SRC ${RESOURCES})
SET(INCLUDE_DIRS ${INCLUDE_DIRS} third-party/redis/redis/src third-party/redis/redis/deps third-party/qscintilla/Qt4Qt5 third-party/memcached/src third-party/ssdb/src)
INCLUDE_DIRECTORIES(${INCLUDE_DIRS})

SOURCE_GROUP("Header Files" FILES ${HEADERS} ${HEADERS_TOMOC})
SOURCE_GROUP("Source Files" FILES ${SOURCES})
SOURCE_GROUP("Generated" FILES ${MOC_FILES})
SOURCE_GROUP("Platform Files" FILES ${PLATFORM_HDRS} ${PLATFORM_SRCS})
SOURCE_GROUP("Resources" FILES ${RES_SRC} ${RESOURCE_OS})

SET(TRANSLATIONS
    translations/${PROJECT_NAME_LOWERCASE}_ru.ts
    translations/${PROJECT_NAME_LOWERCASE}_fr.ts
)

FIND_PACKAGE(Qt5LinguistTools)

IF(GENERATE_TRANSLATIONS)
    QTX_CREATE_TRANSLATION(QM_FILES ${HEADERS} ${SOURCES} ${TRANSLATIONS} OPTIONS -no-obsolete)
ELSE(GENERATE_TRANSLATIONS)
    QTX_ADD_TRANSLATION(QM_FILES ${TRANSLATIONS})
ENDIF(GENERATE_TRANSLATIONS)

SET(ALL_SOURCES ${ALL_SOURCES} ${HEADERS} ${HEADERS_TOMOC} ${SOURCES} ${MOC_FILES} ${PLATFORM_HDRS} ${PLATFORM_SRCS})
SET(ALL_LIBS ${ALL_LIBS} qscintilla2 common ${QT_LIBRARIES} ${PLATFORM_LIBRARIES} ${OPENSSL_LIBRARIES} ${PYTHON_LIBRARY} ${ZLIB_LIBRARY})
IF(SASL_FOUND)
    SET(ALL_LIBS ${ALL_LIBS} ${SASL_LIBRARIES})
ENDIF(SASL_FOUND)

SET(OBJECT_LIBS ${OBJECT_LIBS} $<TARGET_OBJECTS:redis_cli> $<TARGET_OBJECTS:libssh2> $<TARGET_OBJECTS:libmemcached> $<TARGET_OBJECTS:ssd_cli> $<TARGET_OBJECTS:liblua>)

IF(OS_LINUX)
    SET(ALL_LIBS ${ALL_LIBS} pthread)
ENDIF(OS_LINUX)

IF(PYTHON_ENABLED)
    SET(OBJECT_LIBS ${OBJECT_LIBS} $<TARGET_OBJECTS:pythonqt>)
    IF(OS_LINUX)
        SET(ALL_LIBS ${ALL_LIBS} dl util)
    ENDIF(OS_LINUX)
ENDIF(PYTHON_ENABLED)

SET(EXE_SOURCES main.cpp ${RES_SRC} ${ICON_FILE} ${RESOURCE_OS} ${QM_FILES})

IF(MINGW OR CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANGCXX)
    ADD_APP_LIBRARY_OBJECT(${PROJECT_CORE_LIBRARY} "${ALL_SOURCES}" "${ALL_LIBS}" "${OBJECT_LIBS}")
    IF(OS_ANDROID)
        ADD_APP_SHARED_LIBRARY(${PROJECT_NAME} "${EXE_SOURCES}" "${PROJECT_CORE_LIBRARY}")
    ELSE()
        ADD_APP_EXECUTABLE(${PROJECT_NAME} "${EXE_SOURCES}" "${PROJECT_CORE_LIBRARY}")
    ENDIF(OS_ANDROID)
ELSE()
    MESSAGE(FATAL_ERROR "NOT SUPPORTED COMPILER!!!")
ENDIF(MINGW OR CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANGCXX)

IF(OS_WINDOWS)
    SET_SOURCE_FILES_PROPERTIES(${RESOURCE_OS} PROPERTIES LANGUAGE RC)
ELSEIF(OS_MACOSX)
    SET_SOURCE_FILES_PROPERTIES(${RESOURCE_OS} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    SET_SOURCE_FILES_PROPERTIES(${ICON_FILE} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
ENDIF(OS_WINDOWS)

GET_FILENAME_COMPONENT(ICON_FILE_NAME ${ICON_FILE} NAME)
IF(OS_MACOSX)
    SET(MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME})
    SET(MACOSX_BUNDLE_GUI_IDENTIFIER ${PROJECT_NAME})
    SET(MACOSX_BUNDLE_INFO_STRING "${PROJECT_VERSION},${PROJECT_COPYRIGHT}" )
    SET(MACOSX_BUNDLE_SHORT_VERSION_STRING "${SHORT_VERSION}" )
    SET(MACOSX_BUNDLE_LONG_VERSION_STRING ${PROJECT_VERSION})
    SET(MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION})
    SET(MACOSX_BUNDLE_COPYRIGHT ${PROJECT_COPYRIGHT})
    SET(MACOSX_BUNDLE_ICON_FILE ${ICON_FILE_NAME})
ENDIF(OS_MACOSX)

# Start to install

VersionConf(${PROJECT_NAME} ${RESOURCE_OS_IN} ${RESOURCE_OS} ${ICON_FILE_NAME})
INSTALL(TARGETS ${PROJECT_NAME} DESTINATION ${TARGET_INSTALL_DESTINATION} COMPONENT APPLICATIONS)

INSTALL(FILES ${CMAKE_SOURCE_DIR}/LICENSE DESTINATION . COMPONENT LICENSE)
INSTALL(FILES ${CMAKE_SOURCE_DIR}/COPYRIGHT DESTINATION . COMPONENT LICENSE)
INSTALL(FILES ${CMAKE_SOURCE_DIR}/changelog.txt DESTINATION . COMPONENT LICENSE RENAME CHANGELOG)
INSTALL_QT(${PROJECT_NAME} ${LIB_INSTALL_DESTINATION})

IF(OS_WINDOWS)
    DEPLOY_QT_PLUGIN("platforms/qwindows" ${QT_PLUGINS_DIR} ${LIB_INSTALL_DESTINATION})
    DEPLOY_QT_PLUGIN("imageformats/qgif" ${QT_PLUGINS_DIR} ${LIB_INSTALL_DESTINATION})
    SET(MISC_LIBS icuuc49 icuin49 icudt49 icuuc51 icuin51 icudt51 D3DCompiler_43 libEGL libGLESv2)
    SET(PLACE_WHERE_SEARCH ${QT_BINS_DIR})

    #find runtime zlib
    SET(SHARED_ZLIB_NAMES z.dll zlib.dll zdll.dll zlib1.dll zlibd.dll zlibd1.dll)
    FIND_RUNTIME_LIBRARY(SHARED_ZLIB_LIBRARY SHARED_ZLIB_NAMES)
    INSTALL(FILES ${SHARED_ZLIB_LIBRARY} DESTINATION ${LIB_INSTALL_DESTINATION} COMPONENT RUNTIME)

    #find runtime msys
    SET(SHARED_MSYS_NAMES msys-2.0.dll)
    FIND_RUNTIME_LIBRARY(SHARED_MSYS_LIBRARY SHARED_MSYS_NAMES)
    INSTALL(FILES ${SHARED_MSYS_LIBRARY} DESTINATION ${LIB_INSTALL_DESTINATION} COMPONENT RUNTIME)

    #find runtime sasl
    SET(SHARED_SASL_NAMES msys-sasl2-3.dll)
    FIND_RUNTIME_LIBRARY(SHARED_SASL_LIBRARY SHARED_SASL_NAMES)
    INSTALL(FILES ${SHARED_SASL_LIBRARY} DESTINATION ${LIB_INSTALL_DESTINATION} COMPONENT RUNTIME)

IF(PYTHON_ENABLED)
    #find runtime python
    SET(SHARED_PYTHON_NAMES libpython2.7.dll)
    FIND_RUNTIME_LIBRARY(SHARED_PYTHON_LIBRARY SHARED_PYTHON_NAMES)
    INSTALL(FILES ${SHARED_PYTHON_LIBRARY} DESTINATION ${LIB_INSTALL_DESTINATION} COMPONENT RUNTIME)
ENDIF(PYTHON_ENABLED)

ELSEIF(OS_MACOSX)
    SET(BUNDLE_NAME ${MACOSX_BUNDLE_BUNDLE_NAME}.app)
    DEPLOY_QT_PLUGIN("platforms/qcocoa" ${QT_PLUGINS_DIR} ${BUNDLE_NAME}/Contents/plugins)
    DEPLOY_QT_PLUGIN("accessible/qtaccessiblewidgets" ${QT_PLUGINS_DIR} ${BUNDLE_NAME}/Contents/plugins)
    DEPLOY_QT_PLUGIN("imageformats/qgif" ${QT_PLUGINS_DIR} ${BUNDLE_NAME}/Contents/plugins)
    INSTALL(FILES ${CMAKE_SOURCE_DIR}/install/qt.conf DESTINATION ${BUNDLE_NAME}/Contents/Resources COMPONENT RESOURCES)
ELSEIF(OS_LINUX)
    CONFIGURE_FILE("${CMAKE_SOURCE_DIR}/install/linux/${PROJECT_NAME_LOWERCASE}.sh.in" "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME_LOWERCASE}.sh" @ONLY IMMEDIATE)
    INSTALL(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME_LOWERCASE}.sh DESTINATION ${TARGET_INSTALL_DESTINATION})
    INSTALL(FILES ${RESOURCE_OS} DESTINATION share/applications COMPONENT RESOURCES)
    INSTALL(FILES ${ICON_FILE} DESTINATION share/icons COMPONENT RESOURCES)
    DEPLOY_QT_PLUGIN("platforms/qxcb" ${QT_PLUGINS_DIR} ${LIB_INSTALL_DESTINATION})
    DEPLOY_QT_PLUGIN("platformthemes/qgtk2" ${QT_PLUGINS_DIR} ${LIB_INSTALL_DESTINATION})
    DEPLOY_QT_PLUGIN("imageformats/qgif" ${QT_PLUGINS_DIR} ${LIB_INSTALL_DESTINATION})
    SET(MISC_LIBS icuuc49 icudata49 icui18n49 icuuc51 icudata51 icui18n51)
    SET(PLACE_WHERE_SEARCH ${QT_LIBS_DIR})
ENDIF(OS_WINDOWS)

IF(PYTHON_ENABLED)
    GET_FILENAME_COMPONENT(PYTHON_LIBRARY_PATH ${PYTHON_LIBRARY} PATH)
	
    IF(APPLE)
        SET(PYTHON_INSTALL_DIR ${BUNDLE_NAME}/Contents/MacOS/lib)
    ELSE()
        SET(PYTHON_INSTALL_DIR ${TARGET_INSTALL_DESTINATION}/lib)
    ENDIF(APPLE)

    INSTALL(DIRECTORY ${PYTHON_LIBRARY_PATH}/python2.7 DESTINATION ${PYTHON_INSTALL_DIR} COMPONENT PYTHON_MODULES
        FILES_MATCHING
        PATTERN "*.py"
        PATTERN "lib-dynload/*.so"
        PATTERN "lib-dynload/*.pyd"
        PATTERN ".pyc" EXCLUDE
        PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ)
ENDIF(PYTHON_ENABLED)

IF(OS_MACOSX)
    INSTALL(FILES ${QM_FILES} DESTINATION ${BUNDLE_NAME}/Contents/MacOS/translations COMPONENT TRANSLATIONS)
    INSTALL(DIRECTORY DESTINATION ${BUNDLE_NAME}/Contents/MacOS/lua DIRECTORY_PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_WRITE GROUP_READ GROUP_EXECUTE)
ELSE()
    INSTALL(FILES ${QM_FILES} DESTINATION ${TARGET_INSTALL_DESTINATION}/translations COMPONENT TRANSLATIONS)
    INSTALL(DIRECTORY DESTINATION ${TARGET_INSTALL_DESTINATION}/lua DIRECTORY_PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_WRITE GROUP_READ GROUP_EXECUTE)
ENDIF(OS_MACOSX)

FOREACH(miscLib ${MISC_LIBS})
    DEPLOY_QT_PLUGIN(${miscLib} ${PLACE_WHERE_SEARCH} ${LIB_INSTALL_DESTINATION})
ENDFOREACH(miscLib ${MISC_LIBS})

INSTALL_RUNTIME_LIBRARIES()
TARGET_BUNDLEFIX(${PROJECT_NAME} ${LIB_INSTALL_DESTINATION})

IF(DEVELOPER_ENABLE_TESTS)
########## PREPARE GTEST LIBRARY ##########
    IF(MSVC)
        SET(gtest_force_shared_crt ON CACHE BOOL "Build shared gtest library")
        ADD_DEFINITIONS(-D_VARIADIC_MAX=10)
    ENDIF(MSVC)
    ADD_SUBDIRECTORY(${CMAKE_SOURCE_DIR}/tests/gtest-1.7.0 gtest)
    INCLUDE_DIRECTORIES(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
########## PREPARE GTEST LIBRARY ##########

    ADD_EXECUTABLE(unit_tests
        ${CMAKE_SOURCE_DIR}/tests/test_fasto_objects.cpp
        ${CMAKE_SOURCE_DIR}/tests/unit_test_common_net.cpp
        ${CMAKE_SOURCE_DIR}/tests/unit_test_common_strings.cpp
        global/global.cpp
    )

    TARGET_LINK_LIBRARIES(unit_tests gtest gtest_main common)

    ADD_TEST(NAME unit_tests COMMAND tests)
    SET_PROPERTY(TARGET unit_tests PROPERTY FOLDER "Unit tests")
ENDIF(DEVELOPER_ENABLE_TESTS)
