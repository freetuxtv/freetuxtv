cmake_minimum_required(VERSION 2.8.9)

# Define project data
set (APPLICATION_VENDOR_NAME "freetuxtv")
set (APPLICATION_VENDOR_DOMAIN "https://github.com/freetuxtv/freetuxtv")
set (APPLICATION_NAME "FreetuxTV")
set (APPLICATION_VERSION_MAJOR 0)
set (APPLICATION_VERSION_MINOR 7)
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

project (freetuxtv)

# Define install paths
if(WIN32)
    set(INSTALL_PATH_BIN ".")
    set(INSTALL_PATH_SHARE ".")
    set(INSTALL_PATH_DOC ".")
    set(INSTALL_PATH_APPLICATIONS ".")
    set(INSTALL_PATH_ICONS ".")
elseif(APPLE)
    set(INSTALL_PATH_BIN ".")
    set(INSTALL_PATH_SHARE ".")
    set(INSTALL_PATH_FRAMEWORKS "../Frameworks")
    set(INSTALL_PATH_DOC ".")
    set(INSTALL_PATH_APPLICATIONS ".")
    set(INSTALL_PATH_ICONS ".")
else(WIN32)
    set(INSTALL_PATH_BIN "bin/")
    set(INSTALL_PATH_SHARE "share/${APPLICATION_PACKAGE_NAME}")
    set(INSTALL_PATH_DOC "share/${APPLICATION_PACKAGE_NAME}/doc")
    set(INSTALL_PATH_APPLICATIONS "share/applications")
    set(INSTALL_PATH_ICONS "share/icons/")
endif(WIN32)

####################
# Library checking
####################

# Check for pkg-config
find_package(PkgConfig REQUIRED)

# Check for GTK 3
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
if(GTK3_FOUND)
    message(STATUS "Found gtk3 ${GTK3_VERSION}")
    include_directories(${GTK3_INCLUDE_DIRS})
    link_directories(${GTK3_LIBRARY_DIRS})
    add_definitions(${GTK3_CFLAGS_OTHER} -DGTK_API_VERSION=3)
endif()

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
endif()

# Check for sqlite3
pkg_check_modules(SQLITE3 REQUIRED sqlite3)
if(SQLITE3_FOUND)
    message(STATUS "Found sqlite3 ${SQLITE3_VERSION}")
    include_directories(${SQLITE3_INCLUDE_DIRS})
    link_directories(${SQLITE3_LIBRARY_DIRS})
endif()

# Check for curl
pkg_check_modules(CURL REQUIRED libcurl)
if(CURL_FOUND)
    message(STATUS "Found curl ${CURL_VERSION}")
    include_directories(${CURL_INCLUDE_DIRS})
    link_directories(${CURL_LIBRARY_DIRS})
endif()

####################
# Config file
####################

set(PACKAGE_LOCALE_DIR "${CMAKE_INSTALL_PREFIX}/share/locale")
set(PACKAGE_DATA_DIR "${CMAKE_INSTALL_PREFIX}/${INSTALL_PATH_SHARE}")

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
# Add sources
####################

include_directories (${PROJECT_SOURCE_DIR}/src)

# Compatiblity with autotools
add_definitions(-DPACKAGE_LOCALE_DIR="${PACKAGE_LOCALE_DIR}")
add_definitions(-DPACKAGE_DATA_DIR="${PACKAGE_DATA_DIR}")
add_definitions(-DVERSION="${APPLICATION_VERSION}")
add_definitions(-DPACKAGE="${APPLICATION_PACKAGE_NAME}")
add_definitions(-DPACKAGE_BUGREPORT="${APPLICATION_BUGREPORT}")

set(LIBDBEVOLUTION_SRC
    lib/libdbevolution/db-evolution-instance.h
    lib/libdbevolution/db-evolution-instance.c
)

set(LIBVLCGTK_SRC
    lib/libvlc-gtk/gtk-libvlc-include.h
    lib/libvlc-gtk/gtk-libvlc-instance.h
    lib/libvlc-gtk/gtk-libvlc-instance.c
    lib/libvlc-gtk/gtk-libvlc-media.h
    lib/libvlc-gtk/gtk-libvlc-media.c
    lib/libvlc-gtk/gtk-libvlc-media-player.h
    lib/libvlc-gtk/gtk-libvlc-media-player.c
    lib/libvlc-gtk/gtk-libvlc-private.h
)

set(FREETUXTV_LIB_SRC
    src/gtk-builder-dialog.c
    src/gtk-builder-dialog.h
    src/gtk-builder-object.c
    src/gtk-builder-object.h
    src/gtk-builder-widget.c
    src/gtk-builder-widget.h
    src/gtk-builder-window.c
    src/gtk-builder-window.h
    src/gtk-date-time-picker.c
    src/gtk-date-time-picker.h
    src/gtk-progress-dialog.c
    src/gtk-progress-dialog.h
    src/lib-gmmkeys.c
    src/lib-gmmkeys.h
    src/lib-m3uparser.c
    src/lib-m3uparser.h
)

set(FREETUXTV_SRC
    src/freetuxtv-app.h
    src/freetuxtv-cclosure-marshal.c
    src/freetuxtv-cclosure-marshal.h
    src/freetuxtv-cellrenderer-channelslist.c
    src/freetuxtv-cellrenderer-channelslist.h
    src/freetuxtv-cellrenderer-recordingslist.c
    src/freetuxtv-cellrenderer-recordingslist.h
    src/freetuxtv-channel-infos.c
    src/freetuxtv-channel-infos.h
    src/freetuxtv-channels-group-infos.c
    src/freetuxtv-channels-group-infos.h
    src/freetuxtv-channels-list.c
    src/freetuxtv-channels-list.h
    src/freetuxtv-db-sync.c
    src/freetuxtv-db-sync.h
    src/freetuxtv-fileutils.c
    src/freetuxtv-fileutils.h
    src/freetuxtv-gladexml.h
    src/freetuxtv-i18n.h
    src/freetuxtv-models.c
    src/freetuxtv-models.h
    src/freetuxtv-player-error-dialog.c
    src/freetuxtv-player-error-dialog.h
    src/freetuxtv-recording-infos.c
    src/freetuxtv-recording-infos.h
    src/freetuxtv-recordings-list.c
    src/freetuxtv-recordings-list.h
    src/freetuxtv-tv-channel-infos.c
    src/freetuxtv-tv-channel-infos.h
    src/freetuxtv-tv-channels-list.c
    src/freetuxtv-tv-channels-list.h
    src/freetuxtv-utils.c
    src/freetuxtv-utils.h
    src/freetuxtv-window-add-channels-group.c
    src/freetuxtv-window-add-channels-group.h
    src/freetuxtv-window-add-recording.c
    src/freetuxtv-window-add-recording.h
    src/freetuxtv-window-channel-properties.c
    src/freetuxtv-window-channel-properties.h
    src/freetuxtv-window-main.c
    src/freetuxtv-window-main.h
    src/freetuxtv-window-tv-channels-database.c
    src/freetuxtv-window-tv-channels-database.h
    src/main.c
)

####################
# Create Executable
####################

add_executable(freetuxtv ${LIBDBEVOLUTION_SRC} ${LIBVLCGTK_SRC} ${FREETUXTV_LIB_SRC} ${FREETUXTV_SRC})
target_link_libraries(freetuxtv ${GTK3_LIBRARIES} ${LIBNOTIFY_LIBRARIES} ${LIBVLC_LIBRARIES} ${SQLITE3_LIBRARIES} ${CURL_LIBRARIES})