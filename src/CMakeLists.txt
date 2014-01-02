SET(HEADERS_GLOBAL
    global/global.h
)

SET(SOURCES_GLOBAL
    global/global.cpp
)

SET(HEADERS_GUI_TO_MOC
    gui/MainWindow.h
    gui/AppStyle.h
    gui/widgets/LogWidget.h
    gui/explorer/ExplorerTreeView.h
    gui/explorer/ExplorerTreeModel.h
    gui/dialogs/AboutDialog.h
    gui/dialogs/PreferencesDialog.h
    gui/dialogs/ConnectionsDialog.h
    gui/dialogs/ConnectionDialog.h
    gui/widgets/MainWidget.h
    gui/MainTabBar.h
    gui/FastoEditor.h
    gui/widgets/QueryWidget.h
    gui/widgets/OutputWidget.h
    gui/FastoTreeView.h
    gui/FastoTreeModel.h
    gui/FastoTableView.h
    gui/FastoTableModel.h
    gui/IconLabel.h
    gui/base/TreeModel.h
)
SET(HEADERS_GUI
    gui/GuiFactory.h
    gui/FastoTreeItem.h
    gui/base/TreeItem.h
)
SET(SOURCES_GUI
    gui/MainWindow.cpp
    gui/AppStyle.cpp
    gui/GuiFactory.cpp
    gui/widgets/LogWidget.cpp
    gui/dialogs/AboutDialog.cpp
    gui/dialogs/PreferencesDialog.cpp
    gui/dialogs/ConnectionsDialog.cpp
    gui/dialogs/ConnectionDialog.cpp
    gui/widgets/MainWidget.cpp
    gui/MainTabBar.cpp
    gui/FastoEditor.cpp
    gui/widgets/QueryWidget.cpp
    gui/widgets/OutputWidget.cpp
    gui/explorer/ExplorerTreeView.cpp
    gui/explorer/ExplorerTreeModel.cpp
    gui/FastoTreeView.cpp
    gui/FastoTreeModel.cpp
    gui/FastoTableView.cpp
    gui/FastoTreeItem.cpp
    gui/FastoTableModel.cpp
    gui/IconLabel.cpp
    gui/base/TreeModel.cpp
    gui/base/TreeItem.cpp
)

SET(HEADERS_CORE_TO_MOC
    core/Logger.h
    core/IDriver.h
    core/IServer.h
    core/redis/RedisServer.h
    core/redis/RedisDriver.h
)
SET(HEADERS_CORE
    core/IDataBase.h
    core/redis/RedisDataBase.h
    core/ConnectionTypes.h
    core/SettingsManager.h
    core/ConnectionSettings.h
    core/events/Events.hpp
    core/events/EventsInfo.hpp
    core/redis/RedisConfig.h
    core/ServersManager.h
)
SET(SOURCES_CORE
    core/ConnectionTypes.cpp
    core/SettingsManager.cpp
    core/ConnectionSettings.cpp
    core/Logger.cpp
    core/redis/RedisDriver.cpp
    core/IDriver.cpp
    core/IDataBase.cpp
    core/IServer.cpp
    core/redis/RedisServer.cpp
    core/ServersManager.cpp
    core/redis/RedisConfig.cpp
    core/redis/RedisDataBase.cpp
)
SET(HEADERS_SHELL_TO_MOC
    shell/ShellWidget.h
    shell/RedisShell.h
    shell/RedisLexer.h
)
SET(HEADERS_SHELL
)
SET(SOURCES_SHELL
    shell/ShellWidget.cpp
    shell/RedisShell.cpp
    shell/RedisLexer.cpp
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
    SET(PLATFORM_HDRS)
    SET(PLATFORM_SRCS)
    SET(PLATFORM_LIBRARIES)
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
SET(ALL_LIBS ${ALL_LIBS} ${Boost_LIBRARIES})
ENDIF()

IF(OPENSSL_ENABLED)

ENDIF(OPENSSL_ENABLED)

SET(HEADERS ${HEADERS} ${HEADERS_GLOBAL} ${HEADERS_GUI} ${HEADERS_CORE} ${HEADERS_SHELL})
SET(SOURCES ${SOURCES} main.cpp ${SOURCES_GLOBAL} ${SOURCES_GUI} ${SOURCES_CORE} ${SOURCES_SHELL})
SET(HEADERS_TOMOC ${HEADERS_GUI_TO_MOC} ${HEADERS_CORE_TO_MOC} ${HEADERS_SHELL_TO_MOC})

QTX_WRAP_CPP(MOC_FILES ${HEADERS_TOMOC})
CONFIGURE_FILE("resources/gui.qrc.in" "${CMAKE_CURRENT_LIST_DIR}/resources/gui.qrc" @ONLY IMMEDIATE)
SET(RESOURCES ${CMAKE_CURRENT_LIST_DIR}/resources/gui.qrc)
QTX_ADD_RESOURCES(RES_SRC ${RESOURCES})
SET(INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR} third-party/redis/redis/src third-party/redis/redis/deps third-party/qscintilla/Qt4Qt5)
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
    translations/Translations.h
)
SET(SOURCES_TRANSLATIONS
    translations/Translations.cpp
)
SET(ALL_SOURCES ${ALL_SOURCES} ${HEADERS_TRANSLATIONS} ${SOURCES_TRANSLATIONS})

SET(ALL_SOURCES ${ALL_SOURCES} ${HEADERS} ${HEADERS_TOMOC} ${SOURCES} ${MOC_FILES} ${PLATFORM_HDRS} ${PLATFORM_SRCS} ${RES_SRC} ${ICON_FILE} ${RESOURCE_OS} ${BOOST_SOURCES} ${QM_FILES})
SET(ALL_LIBS ${ALL_LIBS} qscintilla2 ${QT_LIBRARIES} ${QT_QTMAIN_LIBRARY} ${Qt5Core_QTMAIN_LIBRARIES} ${PLATFORM_LIBRARIES} common redis-cli)

IF(MINGW OR CMAKE_COMPILER_IS_GNUCXX OR APPLE)
    ADD_APP_EXECUTABLE(${PROJECT_NAME} "${ALL_SOURCES}" "${ALL_LIBS}")
ELSEIF(MSVC)
    ADD_APP_EXECUTABLE_MSVC(${PROJECT_NAME} "${ALL_SOURCES}" "${ALL_LIBS}")
ELSE()
    MESSAGE(FATAL_ERROR "NOT SUPPORTED COMPILER!!!")
ENDIF(MINGW OR CMAKE_COMPILER_IS_GNUCXX OR APPLE)

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

ENDIF(DEVELOPER_ENABLE_TESTS)
