cmake_minimum_required(VERSION 3.13.4)

set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules")

# Define project data
set (APPLICATION_VENDOR_NAME "freetuxtv")
set (APPLICATION_VENDOR_DOMAIN "https://github.com/freetuxtv/freetuxtv")
set (APPLICATION_NAME "FreetuxTV")
set (APPLICATION_VERSION_MAJOR 1)
set (APPLICATION_VERSION_MINOR 0)
set (APPLICATION_VERSION_PATCH 0)
if(NOT DEFINED APPLICATION_VERSION_BUILD)
    set(APPLICATION_VERSION_BUILD "")
endif()
if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(APPLICATION_VERSION_BUILD ${APPLICATION_VERSION_BUILD}dbg)
endif()
set (APPLICATION_VERSION ${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}.${APPLICATION_VERSION_PATCH}${APPLICATION_VERSION_BUILD})
set (APPLICATION_CONTACTMAIL "admin@freetuxtv.net")
set (APPLICATION_CONTACT "Contact FreetuxTV <${APPLICATION_CONTACTMAIL}>")
set (APPLICATION_PACKAGE_VENDOR "freetuxtv")
set (APPLICATION_PACKAGE_NAME "freetuxtv")
set (APPLICATION_EXE_NAME freetuxtv)
set (APPLICATION_BUGREPORT "https://github.com/freetuxtv/freetuxtv/issues")

project (freetuxtv C CXX)

##############################
# Project configuration
##############################

# Define common project config
include (CMakeModules/ProjectCommonConfig.cmake)

##################
# Define options
##################

include (CMakeModules/ProjectCompilerChecks.cmake)

####################
# Library checking
####################

# Compiler
set(WITH_COMPILER_LIBRARIES ON CACHE BOOL "Enable build with compiler libraries")
# GUI
set(WITH_QT ON CACHE BOOL "Enable build with qt library")
#set(QT_USE_VERSION 5)
set(QT4_COMPONENTS QtCore QtGui QtNetwork QtXml QtSql)
set(QT5_COMPONENTS Core Gui Widgets Network Xml Sql)
set(QT6_COMPONENTS Core Core5Compat Gui Widgets Network Xml Sql)
if(WIN32)
    set(QT_PLATFORMS_COMPONENTS qwindows qminimal)
elseif(APPLE)
    set(QT_PLATFORMS_COMPONENTS qcocoa)
else()
    set(QT_PLATFORMS_COMPONENTS qxcb qminimal)
endif()
set(QT_IMAGEFORMATS_COMPONENTS qjpeg)
set(QT_SQL_COMPONENTS qsqlite qsqlmysql qsqlpsql)

# Check for pkg-config
find_package(PkgConfig REQUIRED)

# Check for libnotify
pkg_check_modules(LIBNOTIFY libnotify>=0.7.6 REQUIRED)
if(LIBNOTIFY_FOUND)
    message(STATUS "Found libnotify ${LIBNOTIFY_VERSION}")
    include_directories(${LIBNOTIFY_INCLUDE_DIRS})
    link_directories(${LIBNOTIFY_LIBRARY_DIRS})
    string(REPLACE "." ";" LIBNOTIFY_VERSION_LIST ${LIBNOTIFY_VERSION})
    list(GET LIBNOTIFY_VERSION_LIST 0 LIBNOTIFY_VERSION_MAJOR)
    list(GET LIBNOTIFY_VERSION_LIST 1 LIBNOTIFY_VERSION_MINOR)
    list(GET LIBNOTIFY_VERSION_LIST 2 LIBNOTIFY_VERSION_REVISION)
    add_definitions(-DLIBNOTIFY_VERSION_MAJOR=${LIBNOTIFY_VERSION_MAJOR} -DLIBNOTIFY_VERSION_MINOR=${LIBNOTIFY_VERSION_MINOR} -DLIBNOTIFY_VERSION_REVISION=${LIBNOTIFY_VERSION_REVISION})
    list(APPEND PACKAGE_DEPS "libnotify4 (>= ${LIBNOTIFY_VERSION})")
    #set(USE_LIBNOTIFY 1)
endif()

# Check for libvlc
pkg_check_modules(LIBVLC REQUIRED libvlc)
if(LIBVLC_FOUND)
    message(STATUS "Found libvlc ${LIBVLC_VERSION}")
    include_directories(${LIBVLC_INCLUDE_DIRS})
    link_directories(${LIBVLC_LIBRARY_DIRS})
    string(REPLACE "." ";" LIBVLC_VERSION_LIST ${LIBVLC_VERSION})
    list(GET LIBVLC_VERSION_LIST 0 LIBVLC_VERSION_MAJOR)
    list(GET LIBVLC_VERSION_LIST 1 LIBVLC_VERSION_MINOR)
    list(GET LIBVLC_VERSION_LIST 2 LIBVLC_VERSION_REVISION)
    add_definitions(-DLIBVLC_VERSION_MAJOR=${LIBVLC_VERSION_MAJOR} -DLIBVLC_VERSION_MINOR=${LIBVLC_VERSION_MINOR} -DLIBVLC_VERSION_REVISION=${LIBVLC_VERSION_REVISION})
    list(APPEND PACKAGE_DEPS "libvlc5 (>= ${LIBVLC_VERSION})")
endif()

# Check for curl
pkg_check_modules(CURL REQUIRED libcurl)
if(CURL_FOUND)
    message(STATUS "Found curl ${CURL_VERSION}")
    include_directories(${CURL_INCLUDE_DIRS})
    link_directories(${CURL_LIBRARY_DIRS})
    list(APPEND PACKAGE_DEPS "libcurl4 (>= ${CURL_VERSION})")
endif()

# Check for vlc
pkg_check_modules(LIBVLC REQUIRED libvlc)
if(LIBVLC_FOUND)
    message(STATUS "Found libvlc ${LIBVLC_VERSION}")
    include_directories(${LIBVLC_INCLUDE_DIRS})
    link_directories(${LIBVLC_LIBRARY_DIRS})
    list(APPEND PACKAGE_DEPS "libvlc (>= ${LIBVLC_VERSION})")
endif()

# Set packaging options
set (PACKAGE_DEPS)
if(WITH_AUTONOMOUS_PACKAGE_BUILD)
    set(AUTONOMOUS_PACKAGE_BUILD ON CACHE BOOL "Enable build of autonomous package without dependencies")
endif()

include (CMakeModules/ProjectIncludeLibraries.cmake)

####################
# Config file
####################

set(PACKAGE_LOCALE_DIR "${CMAKE_INSTALL_PREFIX}/share/locale")
set(PACKAGE_DATA_DIR "${CMAKE_INSTALL_PREFIX}/share")

# Configure a header file to pass some of the CMake settings
# to the source code
configure_file (
        "${PROJECT_SOURCE_DIR}/config.h.in"
        "${PROJECT_BINARY_DIR}/config.h"
)
add_definitions(-DHAVE_CONFIG_H)
configure_file (
        "${PROJECT_SOURCE_DIR}/version.h.in"
        "${PROJECT_BINARY_DIR}/version.h"
)
add_definitions(-DHAVE_VERSION_H)
include_directories (${PROJECT_BINARY_DIR})

####################
# Build libraries
####################

include_directories (${PROJECT_SOURCE_DIR}/lib)
add_subdirectory(lib)

####################
# Add sources
####################

include_directories (${PROJECT_SOURCE_DIR}/src)

set(FREETUXTV_SRC
    src/Controller/ApplicationController.cpp
    src/Controller/ApplicationController.h
    src/Controller/TVChannelInfosListSynchronizer.cpp
    src/Controller/TVChannelInfosListSynchronizer.h
    src/Controller/TVChannelsController.cpp
    src/Controller/TVChannelsController.h
    src/Database/DatabaseApplication.cpp
    src/Database/DatabaseApplication.h
    src/Database/DatabaseController.cpp
    src/Database/DatabaseController.h
    src/Database/DatabaseControllerFactoryApplication.cpp
    src/Database/DatabaseControllerFactoryApplication.h
    src/Database/DatabaseInstance.cpp
    src/Database/DatabaseInstance.h
    src/Database/DatabaseTables.h
    src/Global/Application.cpp
    src/Global/Application.h
    src/Global/FileUtils.cpp
    src/Global/FileUtils.h
    src/Global/Utils.cpp
    src/Global/Utils.h
    src/Global/M3UParser.cpp
    src/Global/M3UParser.h
    src/Global/QError.cpp
    src/Global/QError.h
    src/Global/QFileDownloader.cpp
    src/Global/QFileDownloader.h
    src/Global/UserPaths.cpp
    src/Global/UserPaths.h
    src/GUI/QAddChannelsGroupDialog.cpp
    src/GUI/QAddChannelsGroupDialog.h
    src/GUI/QApplicationMainWindow.cpp
    src/GUI/QApplicationMainWindow.h
    src/GUI/QCtrlBarView.cpp
    src/GUI/QCtrlBarView.h
    src/GUI/QDateTimePicker.cpp
    src/GUI/QDateTimePicker.h
    src/GUI/QPlayerErrorDialog.cpp
    src/GUI/QPlayerErrorDialog.h
    src/GUI/QPreferencesDialog.cpp
    src/GUI/QPreferencesDialog.h
    src/GUI/QProgressDialog.cpp
    src/GUI/QProgressDialog.h
    src/GUI/QRecordingsList.cpp
    src/GUI/QRecordingsList.h
    src/GUI/QChannelsList.cpp
    src/GUI/QChannelsList.h
    src/GUI/QChannelPropertiesDialog.cpp
    src/GUI/QChannelPropertiesDialog.h
    src/GUIController/QAddChannelsGroupDialogController.cpp
    src/GUIController/QAddChannelsGroupDialogController.h
    src/GUIController/QApplicationMainWindowController.cpp
    src/GUIController/QApplicationMainWindowController.h
    src/GUIController/QChannelsItemDelegate.cpp
    src/GUIController/QChannelsItemDelegate.h
    src/GUIController/QPreferencesDialogController.cpp
    src/GUIController/QPreferencesDialogController.h
    src/GUIModel/QChannelsGroupItem.cpp
    src/GUIModel/QChannelsGroupItem.h
    src/GUIModel/QChannelsGroupListModel.cpp
    src/GUIModel/QChannelsGroupListModel.h
    src/GUIModel/QChannelsListItem.cpp
    src/GUIModel/QChannelsListItem.h
    src/Model/ChannelInfos.cpp
    src/Model/ChannelInfos.h
    src/Model/ChannelsGroupInfos.cpp
    src/Model/ChannelsGroupInfos.h
    src/Model/ChannelsGroupInfosList.cpp
    src/Model/ChannelsGroupInfosList.h
    src/Model/ChannelsGroupSection.cpp
    src/Model/ChannelsGroupSection.h
    src/Model/TVChannelInfos.cpp
    src/Model/TVChannelInfos.h
    src/Model/Preferences.cpp
    src/Model/Preferences.h
    src/Model/Proxy.cpp
    src/Model/Proxy.h
    src/Model/RecordingInfos.cpp
    src/Model/RecordingInfos.h
    src/VideoPlayer/QVLCVideoPlayer.cpp
    src/VideoPlayer/QVLCVideoPlayer.h
    src/XML/ChannelsGroupsParserHandler.cpp
    src/XML/ChannelsGroupsParserHandler.h
    src/XML/ChannelsGroupsStreamParser.cpp
    src/XML/ChannelsGroupsStreamParser.h
    src/XML/TVChannelInfosListParserHandler.cpp
    src/XML/TVChannelInfosListParserHandler.h
    src/XML/TVChannelInfosStreamParser.cpp
    src/XML/TVChannelInfosStreamParser.h
    src/main.cpp
)

# Creating MOC file for sources file using "slots" or Q_OBJECT macro
set(APPLICATION_MOC_HEADERS
    src/Controller/TVChannelInfosListSynchronizer.h
    src/Controller/TVChannelsController.h
    src/Database/DatabaseController.h
    src/Global/QFileDownloader.h
    src/GUI/QDateTimePicker.h
    src/GUI/QPlayerErrorDialog.h
    src/GUI/QProgressDialog.h
    src/GUI/QRecordingsList.h
    src/GUI/QChannelsList.h
    src/GUI/QChannelPropertiesDialog.h
    src/GUIController/QAddChannelsGroupDialogController.h
    src/GUIController/QApplicationMainWindowController.h
    src/GUIController/QPreferencesDialogController.h
    src/GUIController/QChannelsItemDelegate.h
    src/VideoPlayer/QVLCVideoPlayer.h
)
qtx_wrap_cpp(
    APPLICATION_MOC_OUTFILES
    ${APPLICATION_MOC_HEADERS}
)

# Resources
set (QRC_FILES
    data/app.qrc
)
qtx_add_resources(RCS_OUTFILES ${QRC_FILES})

####################
# Translations
####################

set (I18N_FILES
    src/Database/DatabaseController.cpp
    src/Global/FileUtils.cpp
    src/Global/M3UParser.cpp
    src/Global/Utils.cpp
    src/GUI/QAddChannelsGroupDialog.cpp
    src/GUI/QApplicationMainWindow.cpp
    src/GUI/QCtrlBarView.cpp
    src/GUI/QDateTimePicker.cpp
    src/GUI/QPlayerErrorDialog.cpp
    src/GUI/QPreferencesDialog.cpp
    src/GUI/QProgressDialog.cpp
    src/GUIController/QAddChannelsGroupDialogController.cpp
    src/GUIController/QApplicationMainWindowController.cpp
    src/GUIController/QPreferencesDialogController.cpp
    src/GUIModel/QChannelsGroupItem.cpp
    src/GUIModel/QChannelsGroupListModel.cpp
    src/XML/ChannelsGroupsStreamParser.cpp
    src/XML/TVChannelInfosStreamParser.cpp
    src/main.cpp
)

set (TS_FILES
    ts/de.ts
    ts/es.ts
    ts/fr.ts
    ts/hr.ts
    ts/lt.ts
    ts/pl.ts
    ts/pt.ts
    ts/ru.ts
    ts/tr.ts
    ts/zh_CN.ts
    ts/zh_TW.ts
)

#option (UPDATE_TRANSLATIONS "Update source translation ts/*.ts files (WARNING: make clean will delete the source .ts files! Danger!)" ON)
#if (UPDATE_TRANSLATIONS)
#  qt_create_translation(qm_FILES ${TS_FILES} ${I18N_FILES})
#else (UPDATE_TRANSLATIONS)
#  qt_add_translation(qm_FILES ${TS_FILES})
#endif (UPDATE_TRANSLATIONS)

#add_custom_target(update_ts ALL DEPENDS ${qm_FILES})

####################
# RPATH
####################

if(UNIX AND NOT APPLE)
	# Default library path for debian
	set(CMAKE_INSTALL_RPATH "/usr/${INSTALL_PATH_LIB}")
	# Define RPATH instead of RUNPATH
	set(CMAKE_EXE_LINKER_FLAGS "-Wl,--disable-new-dtags")
endif(UNIX AND NOT APPLE)

####################
# Create Executable
####################

#add_executable(${APPLICATION_EXE_NAME} ${LIBDBEVOLUTION_SRC} ${LIBVLCGTK_SRC} ${FREETUXTV_LIB_SRC} ${FREETUXTV_SRC})
#target_link_libraries(${APPLICATION_EXE_NAME} ${QT_LIBRARIES} ${LIBNOTIFY_LIBRARIES} ${LIBVLC_LIBRARIES} ${SQLITE3_LIBRARIES} ${CURL_LIBRARIES})
add_executable(
    ${APPLICATION_EXE_NAME}
    ${FREETUXTV_SRC}
    ${APPLICATION_MOC_OUTFILES}
    ${RCS_OUTFILES}
)
target_link_libraries(
    ${APPLICATION_EXE_NAME}
    common_database
    toolkit
    libqdbevolution
    ${QT_LIBRARIES}
    ${LIBVLC_LIBRARIES}
)
install(TARGETS ${APPLICATION_EXE_NAME} DESTINATION ${INSTALL_PATH_BIN})

############################
# Common files installation
############################

# Documentation file
set(DOCFILES_EXT "")
if(WIN32)
    set(DOCFILES_EXT ".txt")
endif(WIN32)

# README file
configure_file (
        "${PROJECT_SOURCE_DIR}/README"
        "${PROJECT_BINARY_DIR}/README${DOCFILES_EXT}"
        NEWLINE_STYLE ${AUTO_NEWLINE_STYLE}
)
install(FILES "${PROJECT_BINARY_DIR}/README${DOCFILES_EXT}" DESTINATION ${INSTALL_PATH_DOC})

# COPYING file
configure_file (
        "${PROJECT_SOURCE_DIR}/COPYING"
        "${PROJECT_BINARY_DIR}/COPYING${DOCFILES_EXT}"
        NEWLINE_STYLE ${AUTO_NEWLINE_STYLE}
)
install(FILES "${PROJECT_BINARY_DIR}/COPYING${DOCFILES_EXT}" DESTINATION ${INSTALL_PATH_DOC})

# AUTHORS file
configure_file (
        "${PROJECT_SOURCE_DIR}/AUTHORS"
        "${PROJECT_BINARY_DIR}/AUTHORS${DOCFILES_EXT}"
        NEWLINE_STYLE ${AUTO_NEWLINE_STYLE}
)
install(FILES "${PROJECT_BINARY_DIR}/AUTHORS${DOCFILES_EXT}" DESTINATION ${INSTALL_PATH_DOC})

# ChangeLog file
configure_file (
        "${PROJECT_SOURCE_DIR}/ChangeLog"
        "${PROJECT_BINARY_DIR}/ChangeLog${DOCFILES_EXT}"
        NEWLINE_STYLE ${AUTO_NEWLINE_STYLE}
)
install(FILES "${PROJECT_BINARY_DIR}/ChangeLog${DOCFILES_EXT}" DESTINATION ${INSTALL_PATH_DOC})

# INSTALL file
configure_file (
        "${PROJECT_SOURCE_DIR}/INSTALL"
        "${PROJECT_BINARY_DIR}/INSTALL${DOCFILES_EXT}"
        NEWLINE_STYLE ${AUTO_NEWLINE_STYLE}
)
install(FILES "${PROJECT_BINARY_DIR}/INSTALL${DOCFILES_EXT}" DESTINATION ${INSTALL_PATH_DOC})

###############################
# Resources files installation
###############################

# Desktop file
if(UNIX AND NOT WIN32)
    configure_file (
            "${PROJECT_SOURCE_DIR}/data/menu/freetuxtv.desktop.in"
            "${PROJECT_BINARY_DIR}/data/menu/freetuxtv.desktop"
    )
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/data/menu/freetuxtv.desktop DESTINATION ${INSTALL_PATH_APPLICATIONS})
endif(UNIX AND NOT WIN32)

# Splashscreen
install(FILES "${PROJECT_SOURCE_DIR}/data/splashscreen.png" DESTINATION ${INSTALL_PATH_SHARE})

# UI files
install(DIRECTORY "${PROJECT_SOURCE_DIR}/data/ui/" DESTINATION ${INSTALL_PATH_SHARE}/ui FILES_MATCHING PATTERN "*.glade")

# Data files
install(FILES "${PROJECT_SOURCE_DIR}/data/channels_groups.xml" DESTINATION ${INSTALL_PATH_SHARE})
install(FILES "${PROJECT_SOURCE_DIR}/data/tv_channels.xml" DESTINATION ${INSTALL_PATH_SHARE})

# Channels logos
install(DIRECTORY "${PROJECT_SOURCE_DIR}/data/images/channels/" DESTINATION ${INSTALL_PATH_SHARE}/images/channels/ FILES_MATCHING PATTERN "*.png")

# Icons
install(FILES "${PROJECT_SOURCE_DIR}/data/icons/hicolor_apps_16x16_freetuxtv.png" DESTINATION ${INSTALL_PATH_ICONS}/hicolor/16x16/apps/ RENAME freetuxtv.png)
install(FILES "${PROJECT_SOURCE_DIR}/data/icons/hicolor_apps_22x22_freetuxtv.png" DESTINATION ${INSTALL_PATH_ICONS}/hicolor/22x22/apps/ RENAME freetuxtv.png)
install(FILES "${PROJECT_SOURCE_DIR}/data/icons/hicolor_apps_32x32_freetuxtv.png" DESTINATION ${INSTALL_PATH_ICONS}/hicolor/32x32/apps/ RENAME freetuxtv.png)
install(FILES "${PROJECT_SOURCE_DIR}/data/icons/hicolor_apps_48x48_freetuxtv.png" DESTINATION ${INSTALL_PATH_ICONS}/hicolor/48x48/apps/ RENAME freetuxtv.png)
install(FILES "${PROJECT_SOURCE_DIR}/data/icons/hicolor_apps_64x64_freetuxtv.png" DESTINATION ${INSTALL_PATH_ICONS}/hicolor/64x64/apps/ RENAME freetuxtv.png)
install(FILES "${PROJECT_SOURCE_DIR}/data/icons/hicolor_apps_128x128_freetuxtv.png" DESTINATION ${INSTALL_PATH_ICONS}/hicolor/128x128/apps/ RENAME freetuxtv.png)
install(FILES "${PROJECT_SOURCE_DIR}/data/icons/hicolor_apps_scalable_freetuxtv.svg" DESTINATION ${INSTALL_PATH_ICONS}/hicolor/scalable/apps/ RENAME freetuxtv.svg)

#################
# Build package
#################

set(CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION ${INSTALL_PATH_BIN})
INCLUDE(InstallRequiredSystemLibraries)

# CPack confile file
configure_file(${PROJECT_SOURCE_DIR}/CPackOptions.cmake.in
        ${PROJECT_BINARY_DIR}/CPackOptions.cmake
        @ONLY)
set(CPACK_PROJECT_CONFIG_FILE ${CMAKE_BINARY_DIR}/CPackOptions.cmake)

if(WIN32)
    if(CMAKE_BUILD_TYPE MATCHES Debug)
        SET(CPACK_GENERATOR ZIP)
    else()
        SET(CPACK_GENERATOR ZIP NSIS)
    endif()
elseif(APPLE)
    configure_file(${PROJECT_SOURCE_DIR}/platforms/osx/Info.plist.in
            ${PROJECT_BINARY_DIR}/platforms/osx/Info.plist
            @ONLY)
    configure_file(${PROJECT_SOURCE_DIR}/platforms/osx/run.sh.in
            ${PROJECT_BINARY_DIR}/platforms/osx/${APPLICATION_NAME}
            @ONLY)
    SET(CPACK_GENERATOR Bundle)
else()
    SET(CPACK_GENERATOR TGZ DEB)
endif(WIN32)

SET(CPACK_PACKAGE_NAME ${APPLICATION_PACKAGE_NAME})
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${APPLICATION_DESCRIPTION_SUMMARY})
SET(CPACK_PACKAGE_VENDOR ${APPLICATION_VENDOR_NAME})
SET(CPACK_PACKAGE_CONTACT ${APPLICATION_CONTACT})
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README${DOCFILES_EXT}")
SET(CPACK_PACKAGE_VERSION_MAJOR ${APPLICATION_VERSION_MAJOR})
SET(CPACK_PACKAGE_VERSION_MINOR ${APPLICATION_VERSION_MINOR})
SET(CPACK_PACKAGE_VERSION_PATCH ${APPLICATION_VERSION_PATCH}${APPLICATION_VERSION_BUILD})
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING${DOCFILES_EXT}")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY ${APPLICATION_PACKAGE_NAME}) # Useful for registry key and NSIS auto uninstaller

if(CMAKE_BUILD_TYPE MATCHES Debug)
    SET(CPACK_STRIP_FILES FALSE)
    SET(CPACK_SOURCE_STRIP_FILES FALSE)
else()
    SET(CPACK_STRIP_FILES TRUE)
    SET(CPACK_SOURCE_STRIP_FILES TRUE)
endif()

if(WIN32)
    # NSIS
    SET(CPACK_NSIS_DISPLAY_NAME "${APPLICATION_NAME}")
elseif(APPLE)
    if (NOT CPACK_SYSTEM_NAME)
        IF(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
            SET(CPACK_SYSTEM_NAME amd64)
        ELSEIF(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "i686")
            SET(CPACK_SYSTEM_NAME i386)
        ENDIF(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
    endif()

    SET(CPACK_BUNDLE_NAME "${APPLICATION_NAME}")
else()
	# DEB instruction files
	configure_file (
		"${PROJECT_SOURCE_DIR}/platforms/debian/postinst.in"
		"${PROJECT_BINARY_DIR}/platforms/debian/postinst"
	)
	configure_file (
		"${PROJECT_SOURCE_DIR}/platforms/debian/prerm.in"
		"${PROJECT_BINARY_DIR}/platforms/debian/prerm"
	)

	if (NOT CPACK_SYSTEM_NAME)
		IF(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
			SET(CPACK_SYSTEM_NAME amd64)
		ELSEIF(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "i686")
			SET(CPACK_SYSTEM_NAME i386)
		ENDIF(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
	endif()

	JOINLIST(PACKAGE_DEPS ", " DEBIAN_DEPS)

	SET(CPACK_DEBIAN_PACKAGE_NAME ${APPLICATION_PACKAGE_NAME})
	SET(CPACK_DEBIAN_PACKAGE_VERSION ${APPLICATION_VERSION})
	SET(CPACK_DEBIAN_PACKAGE_DEPENDS ${DEBIAN_DEPS})
	SET(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${PROJECT_BINARY_DIR}/platforms/debian/postinst;${PROJECT_BINARY_DIR}/platforms/debian/prerm;")
	SET(CPACK_DEBIAN_PACKAGE_SECTION "misc")
	# SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE ${DEB_ARCH})
ENDIF()
SET(CPACK_PACKAGE_EXECUTABLES "${APPLICATION_EXE_NAME}" "${APPLICATION_NAME}")
include(CPack)
