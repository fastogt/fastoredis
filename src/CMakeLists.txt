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
    gui/dialogs/info_server_dialog.h
    gui/dialogs/property_server_dialog.h
    gui/dialogs/preferences_dialog.h
    gui/dialogs/connections_dialog.h
    gui/dialogs/connection_dialog.h
    gui/widgets/main_widget.h
    gui/main_tab_bar.h
    gui/fasto_editor.h
    gui/widgets/query_widget.h
    gui/widgets/output_widget.h
    gui/fasto_tree_view.h
    gui/fasto_tree_model.h
    gui/fasto_table_view.h
    gui/fasto_table_model.h
    gui/icon_label.h
    gui/glass_widget.h
    gui/base/tree_model.h
    gui/base/table_model.h
)
SET(HEADERS_GUI
    gui/gui_factory.h
    gui/fasto_tree_item.h
    gui/base/tree_item.h
    gui/base/table_item.h
)
SET(SOURCES_GUI
    gui/main_window.cpp
    gui/app_style.cpp
    gui/gui_factory.cpp
    gui/widgets/log_tab_widget.cpp
    gui/widgets/log_widget.cpp
    gui/widgets/commands_widget.cpp
    gui/property_table_model.cpp
    gui/dialogs/about_dialog.cpp
    gui/dialogs/preferences_dialog.cpp
    gui/dialogs/connections_dialog.cpp
    gui/dialogs/connection_dialog.cpp
    gui/dialogs/info_server_dialog.cpp
    gui/dialogs/property_server_dialog.cpp
    gui/widgets/main_widget.cpp
    gui/main_tab_bar.cpp
    gui/fasto_editor.cpp
    gui/widgets/query_widget.cpp
    gui/widgets/output_widget.cpp
    gui/explorer/explorer_tree_view.cpp
    gui/explorer/explorer_tree_model.cpp
    gui/fasto_tree_view.cpp
    gui/fasto_tree_model.cpp
    gui/fasto_table_view.cpp
    gui/fasto_tree_item.cpp
    gui/fasto_table_model.cpp
    gui/icon_label.cpp
    gui/glass_widget.cpp
    gui/base/tree_model.cpp
    gui/base/tree_item.cpp
    gui/base/table_model.cpp
    gui/base/table_item.cpp
)

SET(HEADERS_CORE_TO_MOC
    core/command_logger.h
    core/logger.h
    core/idriver.h
    core/iserver.h
    core/redis/redis_server.h
    core/redis/redis_driver.h
)
SET(HEADERS_CORE
    core/connection_types.h
    core/settings_manager.h
    core/connection_settings.h
    core/events/infos.h
    core/events/events.h
    core/events/events_info.h
    core/redis/redis_config.h
    core/servers_manager.h
)
SET(SOURCES_CORE
    core/events/events_info.cpp
    core/events/infos.cpp
    core/command_logger.cpp
    core/connection_types.cpp
    core/settings_manager.cpp
    core/connection_settings.cpp
    core/logger.cpp
    core/redis/redis_driver.cpp
    core/idriver.cpp
    core/iserver.cpp
    core/redis/redis_server.cpp
    core/servers_manager.cpp
    core/redis/redis_config.cpp
)
SET(HEADERS_SHELL_TO_MOC
    shell/shell_widget.h
    shell/redis_shell.h
    shell/redis_lexer.h
)
SET(HEADERS_SHELL
)
SET(SOURCES_SHELL
    shell/shell_widget.cpp
    shell/redis_shell.cpp
    shell/redis_lexer.cpp
)

IF(WIN32)
    ADD_DEFINITIONS()
ELSEIF(UNIX)
    ADD_DEFINITIONS()
ENDIF()

SET_DESKTOP_TARGET()

IF(WIN32)
    SET(PLATFORM_HDRS)
    SET(PLATFORM_SRCS)
    SET(PLATFORM_LIBRARIES)
ELSEIF(APPLE)
    SET(PLATFORM_HDRS gui/utils/mac_utils.h)
    SET(PLATFORM_SRCS gui/utils/mac_utils.mm)
    FIND_LIBRARY(COCOA_LIBRARY Cocoa)
    SET(PLATFORM_LIBRARIES ${COCOA_LIBRARY})
ELSEIF(UNIX)
    SET(PLATFORM_HDRS)
    SET(PLATFORM_SRCS)
    SET(PLATFORM_LIBRARIES)
ENDIF()

add_subdirectory(common)
add_subdirectory(third-party/redis)
add_subdirectory(third-party/qscintilla/Qt4Qt5)

IF(BOOST_ENABLED)
    INTEGRATE_BOOST(system)
ENDIF(BOOST_ENABLED)

IF(OPENSSL_ENABLED)
ENDIF(OPENSSL_ENABLED)

SET(HEADERS ${HEADERS} ${HEADERS_GLOBAL} ${HEADERS_GUI} ${HEADERS_CORE} ${HEADERS_SHELL})
SET(SOURCES ${SOURCES} ${SOURCES_GLOBAL} ${SOURCES_GUI} ${SOURCES_CORE} ${SOURCES_SHELL})
SET(HEADERS_TOMOC ${HEADERS_GUI_TO_MOC} ${HEADERS_CORE_TO_MOC} ${HEADERS_SHELL_TO_MOC})

QTX_WRAP_CPP(MOC_FILES ${HEADERS_TOMOC})
CONFIGURE_FILE("resources/gui.qrc.in" "${CMAKE_CURRENT_LIST_DIR}/resources/gui.qrc" @ONLY IMMEDIATE)
SET(RESOURCES ${CMAKE_CURRENT_LIST_DIR}/resources/gui.qrc)
QTX_ADD_RESOURCES(RES_SRC ${RESOURCES})
SET(INCLUDE_DIRS ${INCLUDE_DIRS} ${CMAKE_CURRENT_LIST_DIR} third-party/redis/redis/src third-party/redis/redis/deps third-party/qscintilla/Qt4Qt5)
INCLUDE_DIRECTORIES(${INCLUDE_DIRS})
SOURCE_GROUP("Header Files" FILES ${HEADERS} ${HEADERS_TOMOC})
SOURCE_GROUP("Source Files" FILES ${SOURCES})
SOURCE_GROUP("Generated" FILES ${MOC_FILES})
SOURCE_GROUP("Platform Files" FILES ${PLATFORM_HDRS} ${PLATFORM_SRCS})
SOURCE_GROUP("Resources" FILES ${RES_SRC} ${RESOURCE_OS})

SET(TRANSLATIONS
    translations/${PROJECT_NAME}_ru.ts
)
FIND_PACKAGE(Qt5LinguistTools)
IF(GENERATE_TRANSLATIONS)
    QTX_CREATE_TRANSLATION(QM_FILES ${SOURCES} ${TRANSLATIONS} OPTIONS -no-obsolete)
ELSE()
    QTX_ADD_TRANSLATION(QM_FILES ${TRANSLATIONS})
ENDIF(GENERATE_TRANSLATIONS)

INSTALL(FILES ${QM_FILES} DESTINATION translations COMPONENT TRANSLATIONS)

SET(HEADERS_TRANSLATIONS
    translations/translations.h
)
SET(SOURCES_TRANSLATIONS
    translations/translations.cpp
)
SET(ALL_SOURCES ${ALL_SOURCES} ${HEADERS_TRANSLATIONS} ${SOURCES_TRANSLATIONS})

SET(ALL_SOURCES ${ALL_SOURCES} ${HEADERS} ${HEADERS_TOMOC} ${SOURCES} ${MOC_FILES} ${PLATFORM_HDRS} ${PLATFORM_SRCS})
SET(ALL_LIBS ${ALL_LIBS} qscintilla2 ${QT_LIBRARIES} ${PLATFORM_LIBRARIES} common redis-cli)
SET(EXE_SOURCES main.cpp ${RES_SRC} ${ICON_FILE} ${RESOURCE_OS} ${QM_FILES})

IF(MINGW OR CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANGCXX)
    ADD_APP_LIBRARY(${PROJECT_NAME_LIBRARY} "${ALL_SOURCES}" "${ALL_LIBS}")
    ADD_APP_EXECUTABLE(${PROJECT_NAME} "${EXE_SOURCES}" "${PROJECT_NAME_LIBRARY}")
ELSEIF(MSVC)
    ADD_APP_LIBRARY_MSVC(${PROJECT_NAME_LIBRARY} "${ALL_SOURCES}" "${ALL_LIBS}")
    ADD_APP_EXECUTABLE_MSVC(${PROJECT_NAME} "${EXE_SOURCES}" "${PROJECT_NAME_LIBRARY}")
ELSE()
    MESSAGE(FATAL_ERROR "NOT SUPPORTED COMPILER!!!")
ENDIF(MINGW OR CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANGCXX)

IF(MANIFEST_ENABLE)
    GET_TARGET_PROPERTY(BINARY_FILE ${PROJECT_NAME} LOCATION)
    ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD COMMAND mt -manifest "${CMAKE_SOURCE_DIR}/install/windows/win7.manifest" -outputresource:"${BINARY_FILE}")
ENDIF(MANIFEST_ENABLE)

IF(WIN32)
    SET_SOURCE_FILES_PROPERTIES(${RESOURCE_OS} PROPERTIES LANGUAGE RC)
ELSEIF(APPLE)
    SET_SOURCE_FILES_PROPERTIES(${RESOURCE_OS} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    SET_SOURCE_FILES_PROPERTIES(${ICON_FILE} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    #SET_SOURCE_FILES_PROPERTIES(${CMAKE_SOURCE_DIR}/install/qt.conf PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
ENDIF(WIN32)

GET_FILENAME_COMPONENT(ICON_FILE_NAME ${ICON_FILE} NAME)
IF(APPLE)
    SET(MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME} )
    SET(MACOSX_BUNDLE_GUI_IDENTIFIER ${PROJECT_NAME})
    SET(MACOSX_BUNDLE_INFO_STRING "${PROJECT_VERSION},${PROJECT_COPYRIGHT}" )
    SET(MACOSX_BUNDLE_SHORT_VERSION_STRING "${SHORT_VERSION}" )
    SET(MACOSX_BUNDLE_LONG_VERSION_STRING ${PROJECT_VERSION})
    SET(MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION})
    SET(MACOSX_BUNDLE_COPYRIGHT ${PROJECT_COPYRIGHT})
    SET(MACOSX_BUNDLE_ICON_FILE ${ICON_FILE_NAME})
ENDIF(APPLE)

# Start to install
VersionConf(${PROJECT_NAME} ${RESOURCE_OS_IN} ${RESOURCE_OS} ${ICON_FILE_NAME})
#IF(WIN32)
#	SET(CMAKE_EXE_LINKER_FLAGS "/VERSION:\"${MAJOR}.${MINOR}\"" CACHE STRING "Resetting the default linker options." FORCE)
#	STRING(REPLACE "." "," FILEVERSION ${PROJECT_VERSION})
#	SET(VERSION_STR ${PROJECT_VERSION})
#ENDIF()
INSTALL(TARGETS ${PROJECT_NAME} DESTINATION ${TARGET_INSTALL_DESTINATION} COMPONENT APPLICATIONS)
INSTALL(FILES ${CMAKE_SOURCE_DIR}/LICENSE DESTINATION . COMPONENT LICENSE)
INSTALL(FILES ${CMAKE_SOURCE_DIR}/COPYRIGHT DESTINATION . COMPONENT LICENSE)
INSTALL(FILES ${CMAKE_SOURCE_DIR}/changelog.txt DESTINATION . COMPONENT LICENSE RENAME CHANGELOG)
INSTALL_QT(${PROJECT_NAME} ${LIB_INSTALL_DESTINATION})

IF(WIN32)
    DEPLOY_QT_PLUGIN("platforms/qwindows" ${QT_PLUGINS_DIR} ${LIB_INSTALL_DESTINATION})
    DEPLOY_QT_PLUGIN("imageformats/qgif" ${QT_PLUGINS_DIR} ${LIB_INSTALL_DESTINATION})
    SET(MISC_LIBS icuuc49 icuin49 icudt49 icuuc51 icuin51 icudt51 D3DCompiler_43 libEGL libGLESv2)
    SET(PLACE_WHERE_SEARCH ${QT_BINS_DIR})
ELSEIF(APPLE)
    SET(BUNDLE_NAME ${MACOSX_BUNDLE_BUNDLE_NAME}.app)
    DEPLOY_QT_PLUGIN("platforms/qcocoa" ${QT_PLUGINS_DIR} ${BUNDLE_NAME}/Contents/plugins)
    DEPLOY_QT_PLUGIN("accessible/qtaccessiblewidgets" ${QT_PLUGINS_DIR} ${BUNDLE_NAME}/Contents/plugins)
    DEPLOY_QT_PLUGIN("imageformats/qgif" ${QT_PLUGINS_DIR} ${BUNDLE_NAME}/Contents/plugins)
    INSTALL(FILES ${CMAKE_SOURCE_DIR}/install/qt.conf DESTINATION ${BUNDLE_NAME}/Contents/Resources COMPONENT RESOURCES)
ELSEIF(UNIX)
    INSTALL(FILES ${CMAKE_SOURCE_DIR}/install/linux/README DESTINATION . COMPONENT LICENSE)
    INSTALL(PROGRAMS ${CMAKE_SOURCE_DIR}/install/linux/${PROJECT_NAME}.sh DESTINATION ${TARGET_INSTALL_DESTINATION})
    INSTALL(FILES ${RESOURCE_OS} DESTINATION share/applications COMPONENT RESOURCES)
    INSTALL(FILES ${ICON_FILE} DESTINATION share/icons COMPONENT RESOURCES)
    DEPLOY_QT_PLUGIN("platforms/qxcb" ${QT_PLUGINS_DIR} ${LIB_INSTALL_DESTINATION})
    DEPLOY_QT_PLUGIN("platformthemes/qgtk2" ${QT_PLUGINS_DIR} ${LIB_INSTALL_DESTINATION})
    DEPLOY_QT_PLUGIN("imageformats/qgif" ${QT_PLUGINS_DIR} ${LIB_INSTALL_DESTINATION})
    SET(MISC_LIBS icuuc49 icudata49 icui18n49 icuuc51 icudata51 icui18n51)
    SET(PLACE_WHERE_SEARCH ${QT_LIBS_DIR})
ENDIF()

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
        ${CMAKE_SOURCE_DIR}/tests/test_fasto_objects.cpp ${CMAKE_SOURCE_DIR}/tests/unit_test_common_net.cpp global/global.cpp)

    TARGET_LINK_LIBRARIES(unit_tests gtest gtest_main common)

    ADD_TEST(NAME unit_tests COMMAND tests)
    SET_PROPERTY(TARGET unit_tests PROPERTY FOLDER "Unit tests")

ENDIF(DEVELOPER_ENABLE_TESTS)
