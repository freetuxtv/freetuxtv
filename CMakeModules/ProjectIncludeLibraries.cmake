# Do include for requested modules

include (CheckFunctionExists)

include (ProjectUsefulMacros)

option(SINGLE_UNIT_COMPILATION "Enable single unit compilation" OFF)
option(AUTONOMOUS_PACKAGE_BUILD "Enable build of autonomous package without dependencies" OFF)

#####################
### Compiler library
#####################

option(WITH_COMPILER_LIBRARIES "Enable build with compiler libraries" OFF)
if(WITH_COMPILER_LIBRARIES)
	if(WIN32)
		if(CMAKE_COMPILER_IS_GNUCXX)
			set(TMP_DLL_PATH "")
			# If libgmp is present we just need it
			if(HOST_WIN32)
				execute_process(COMMAND cmd.exe /c "${CMAKE_CURRENT_LIST_DIR}/find_file_in_path.bat libgmp-10.dll" OUTPUT_VARIABLE TMP_DLL_PATH OUTPUT_STRIP_TRAILING_WHITESPACE)
			endif()
			if(TMP_DLL_PATH STREQUAL "")
				# Otherwise we take libstdc++
				execute_process(COMMAND ${CMAKE_CXX_COMPILER} -print-file-name=libstdc++-6.dll OUTPUT_VARIABLE TMP_DLL_PATH OUTPUT_STRIP_TRAILING_WHITESPACE)
			else()
				install(FILES ${TMP_DLL_PATH} DESTINATION ${INSTALL_PATH_BIN})
			endif()
			if(NOT TMP_DLL_PATH STREQUAL "")
				install(FILES ${TMP_DLL_PATH} DESTINATION ${INSTALL_PATH_BIN})
			endif()
			execute_process(COMMAND ${CMAKE_CXX_COMPILER} -print-file-name=libgcc_s_sjlj-1.dll OUTPUT_VARIABLE TMP_DLL_PATH OUTPUT_STRIP_TRAILING_WHITESPACE)
			if(NOT TMP_DLL_PATH STREQUAL "" AND NOT TMP_DLL_PATH STREQUAL "libgcc_s_sjlj-1.dll")
				install(FILES ${TMP_DLL_PATH} DESTINATION ${INSTALL_PATH_BIN})
			endif()
			execute_process(COMMAND ${CMAKE_CXX_COMPILER} -print-file-name=libgcc_s_seh-1.dll OUTPUT_VARIABLE TMP_DLL_PATH OUTPUT_STRIP_TRAILING_WHITESPACE)
			if(NOT TMP_DLL_PATH STREQUAL "" AND NOT TMP_DLL_PATH STREQUAL "libgcc_s_seh-1.dll")
				install(FILES ${TMP_DLL_PATH} DESTINATION ${INSTALL_PATH_BIN})
			endif()
		endif(CMAKE_COMPILER_IS_GNUCXX)

		if(MSVC)
			install(FILES C:/Windows/System32/msvcp100.dll DESTINATION ${INSTALL_PATH_BIN})
			install(FILES C:/Windows/System32/msvcr100.dll DESTINATION ${INSTALL_PATH_BIN})
			install(FILES C:/Windows/System32/vcomp140.dll DESTINATION ${INSTALL_PATH_BIN})
		endif(MSVC)

		if(CMAKE_BUILD_TYPE STREQUAL "Debug")
			install(FILES C:/Windows/System32/concrt140d.dll DESTINATION ${INSTALL_PATH_BIN})
			install(FILES C:/Windows/System32/msvcp140_1d.dll DESTINATION ${INSTALL_PATH_BIN})
			install(FILES C:/Windows/System32/msvcp140_2d.dll DESTINATION ${INSTALL_PATH_BIN})
			install(FILES C:/Windows/System32/msvcp140d.dll DESTINATION ${INSTALL_PATH_BIN})
			install(FILES C:/Windows/System32/ucrtbased.dll DESTINATION ${INSTALL_PATH_BIN})
			install(FILES C:/Windows/System32/vcruntime140_1d.dll DESTINATION ${INSTALL_PATH_BIN})
			install(FILES C:/Windows/System32/vcruntime140d.dll DESTINATION ${INSTALL_PATH_BIN})
			install(FILES C:/Windows/System32/vcomp140d.dll DESTINATION ${INSTALL_PATH_BIN})
		endif ()
	endif(WIN32)
endif(WITH_COMPILER_LIBRARIES)

# Add gdb
if(CMAKE_BUILD_TYPE MATCHES Debug)
	if(NOT AUTONOMOUS_PACKAGE_BUILD)
		list(APPEND PACKAGE_DEPS "gdb")
	endif()
endif()

# Get libc6 dependencies on linux to check OS version
if(UNIX AND NOT APPLE)
	if(AUTONOMOUS_PACKAGE_BUILD)
		include(${CMAKE_CURRENT_LIST_DIR}/glibc.cmake)
		CHECK_GLIBC_VERSION()
		if(GLIBC_VERSION)
			message(STATUS "Found glibc/libc6 version: ${GLIBC_VERSION}")
			# Package glibc is not present in Ubuntu, ask for libc6 instead
			list(APPEND PACKAGE_DEPS "libc6 (>=${GLIBC_VERSION})")
		endif()
	endif()
endif()

#####################
### Security
#####################

# Add OpenSSL
option(WITH_OPENSSL "Enable build with openssl library" OFF)
if(WITH_OPENSSL)
	find_package (OpenSSL)
	include_directories (${OPENSSL_INCLUDE_DIR})
	if(OPENSSL_FOUND)
		message(STATUS "OpenSSL found: ${OPENSSL_VERSION}")
		set(USE_OPENSSL 1)

		if(WIN32)
			if(MSVC)
				find_install_dll("libcrypto" ${INSTALL_PATH_BIN} TRUE)
				find_install_dll("libssl" ${INSTALL_PATH_BIN} TRUE)
			else()
				find_install_dll("crypto" ${INSTALL_PATH_BIN} TRUE)
				find_install_dll("ssl" ${INSTALL_PATH_BIN} TRUE)
			endif()
		elseif (APPLE)
			list(GET OPENSSL_LIBRARIES 0 OPENSSL_LIBRARY_PATH_0)
			get_filename_component(OPENSSL_LIBRARY_PATH ${OPENSSL_LIBRARY_PATH_0} DIRECTORY)
			get_filename_component(OPENSSL_LIBRARY_ROOT_PATH ${OPENSSL_LIBRARY_PATH}/../.. REALPATH)
			file(GLOB OPENSSL_LIBRARIES_INSTALL
					${OPENSSL_LIBRARY_PATH}/libssl*.dylib
					${OPENSSL_LIBRARY_PATH}/libcrypto*.dylib
			)
			install(FILES ${OPENSSL_LIBRARIES_INSTALL} DESTINATION ${INSTALL_PATH_LIB})
		else()
			if(AUTONOMOUS_PACKAGE_BUILD)
				install_libs_so("${OPENSSL_LIBRARIES}" ${INSTALL_PATH_LIB})
			endif(AUTONOMOUS_PACKAGE_BUILD)
		endif(WIN32)

	endif(OPENSSL_FOUND)
endif(WITH_OPENSSL)

if(QT_OPENSSL_USE)
	if(MSVC)
		# Workaround to get Qt Openssl v1 when vcpkg use v3
		message(STATUS "Using Qt Open SSL version from ${QT_OPENSSL_PATH}")
		find_install_dll_in_path("libcrypto" ${QT_OPENSSL_PATH}/bin ${INSTALL_PATH_BIN} TRUE)
		find_install_dll_in_path("libssl" ${QT_OPENSSL_PATH}/bin ${INSTALL_PATH_BIN} TRUE)
	endif()
endif()

#########################
### GUI modules
#########################

# Add QT
option(WITH_QT "Enable build with qt library" OFF)
if(WITH_QT)
	include (ProjectIncludeLibrariesQt)

	# Add dependencies libraries
	if (UNIX AND NOT APPLE)
		if(AUTONOMOUS_PACKAGE_BUILD)
			find_install_lib_so("icui18n" ${INSTALL_PATH_LIB} FALSE)
			find_install_lib_so("icuuc" ${INSTALL_PATH_LIB} FALSE)
			find_install_lib_so("icudata" ${INSTALL_PATH_LIB} FALSE)
			find_install_lib_so("double-conversion" ${INSTALL_PATH_LIB} FALSE)
			list(APPEND PACKAGE_DEPS "libpcre2-16-0")
		endif()
	endif()

	# Add dependencies libraries
	set(HAS_QTGUI ((NOT Qt${QT_VERSION_MAJOR}_FOUND AND NOT QT_DONT_USE_QTGUI) OR Qt${QT_VERSION_MAJOR}Gui_FOUND))
	if(HAS_QTGUI)
		if(WIN32)
			if(QT_CUSTOM_PATH AND QT_USE_VERSION EQUAL 5)
				install(FILES ${QT_CUSTOM_PATH}/bin/libEGL.dll DESTINATION ${INSTALL_PATH_BIN} CONFIGURATIONS Release)
				install(FILES ${QT_CUSTOM_PATH}/bin/libGLESv2.dll DESTINATION ${INSTALL_PATH_BIN} CONFIGURATIONS Release)
				install(FILES ${QT_CUSTOM_PATH}/bin/libEGLd.dll DESTINATION ${INSTALL_PATH_BIN} CONFIGURATIONS Debug)
				install(FILES ${QT_CUSTOM_PATH}/bin/libGLESv2d.dll DESTINATION ${INSTALL_PATH_BIN} CONFIGURATIONS Debug)
			endif()
			if(QT_CUSTOM_PATH AND QT_USE_VERSION EQUAL 6)
				install(FILES ${QT_CUSTOM_PATH}/bin/opengl32sw.dll DESTINATION ${INSTALL_PATH_BIN})
			endif()
		endif()
		if(AUTONOMOUS_PACKAGE_BUILD)
			list(APPEND PACKAGE_DEPS "libx11-6")
			list(APPEND PACKAGE_DEPS "libxcb-cursor0")
			list(APPEND PACKAGE_DEPS "libxcb-xinput0")
			list(APPEND PACKAGE_DEPS "libxcb-icccm4")
			list(APPEND PACKAGE_DEPS "libxcb-image0")
			list(APPEND PACKAGE_DEPS "libxcb-keysyms1")
			list(APPEND PACKAGE_DEPS "libxcb-randr0")
			list(APPEND PACKAGE_DEPS "libxcb-render-util0")
			list(APPEND PACKAGE_DEPS "libxcb-xinerama0")
			list(APPEND PACKAGE_DEPS "libxcb-xkb1")
			list(APPEND PACKAGE_DEPS "libxkbcommon-x11-0")
			list(APPEND PACKAGE_DEPS "libgl1")
			list(APPEND PACKAGE_DEPS "libegl1")
			list(APPEND PACKAGE_DEPS "libopengl0")
			list(APPEND PACKAGE_DEPS "libvdpau1")
			list(APPEND PACKAGE_DEPS "libharfbuzz0b")
			# Qt modules without cmake files
			find_install_lib_so("Qt${QT_VERSION_MAJOR}XcbQpa" ${INSTALL_PATH_LIB} FALSE)
			find_install_lib_so("jpeg" ${INSTALL_PATH_LIB} FALSE)
		endif()
	endif()

endif()

# Add QT Mobility (Linux only)
option(WITH_QTMOBILITY "Enable build with qtmobility library" OFF)
if(WITH_QT AND NOT QT_USE_VERSION EQUAL 4)
	# Disable qt mobility if using qt>=5
	set(WITH_QTMOBILITY OFF)
endif()
if(WITH_QTMOBILITY)
	find_package (QtMobility 1.2.0 REQUIRED ${QT_MOBILITY_COMPONENTS})
	if(QTMOBILITY_FOUND)
		set(USE_QTMOBILITY 1)
		set(QTMOBILITY_DEPS_VERSION "1.2")
		include_directories (${QTMOBILITY_INCLUDE_DIR} ${QTMOBILITY_INCLUDE_DIRS})

		# Add component
		foreach(TMP_COMPONENT ${QT_MOBILITY_COMPONENTS})
			string(TOLOWER ${TMP_COMPONENT} TMP_MODULE)
			string(TOUPPER ${TMP_COMPONENT} TMP_MODULEU)

			if(TMP_MODULE STREQUAL "multimediakit")
				set(USE_QTMULTIMEDIA 1)
			else()
				set(USE_QT${TMP_MODULEU} 1)
			endif()

			if (NOT WIN32)
				list(APPEND PACKAGE_DEPS "libqt${TMP_MODULE}1 (>= ${QTMOBILITY_DEPS_VERSION})")
			endif(NOT WIN32)
		endforeach(TMP_COMPONENT)

		if (WIN32)
			if(CMAKE_HOST_SYSTEM MATCHES Linux)
				set(QTMOBILITY_DLL_DIR ${QTMOBILITY_LIBRARY_DIR}) # When cross compiling DLL are here
			else()
				set(QTMOBILITY_DLL_DIR ${QTMOBILITY_LIBRARY_DIR})
			endif()
		endif(WIN32)
	endif(QTMOBILITY_FOUND)
endif(WITH_QTMOBILITY)

# zlib
if(WITH_CURL)
	if(WIN32)
		find_install_dll("zlib" ${INSTALL_PATH_BIN} FALSE)
		find_install_dll("z" ${INSTALL_PATH_BIN} FALSE)
	elseif(APPLE)
		# Todo
	else()
		if(AUTONOMOUS_PACKAGE_BUILD)
			if(WITH_ZLIB_SYSTEM)
				list(APPEND PACKAGE_DEPS "zlib1g")
			else()
				find_install_lib_so("z" ${INSTALL_PATH_LIB} FALSE)
			endif()
		endif()
	endif()
endif()

# freetype
if((WITH_QT AND HAS_QTGUI))
	if(WIN32)
		find_install_dll("freetype" ${INSTALL_PATH_BIN} FALSE)
		find_install_dll("libfreetype" ${INSTALL_PATH_BIN} FALSE)
	elseif(APPLE)
		# Todo
	else()
		if(AUTONOMOUS_PACKAGE_BUILD)
			if(WITH_FREETYPE_SYSTEM)
				list(APPEND PACKAGE_DEPS "libfreetype6")
			else()
				find_install_lib_so("freetype" ${INSTALL_PATH_LIB} FALSE)
			endif()
		endif()
	endif()
endif()

# fontconfig
if(WITH_QT AND HAS_QTGUI)
	if(AUTONOMOUS_PACKAGE_BUILD)
		if(WITH_FONTCONFIG_SYSTEM)
			list(APPEND PACKAGE_DEPS "fontconfig")
		else()
			find_package(Fontconfig REQUIRED)
			if(Fontconfig_FOUND)
				get_filename_component(FONTCONFIG_ROOT ${Fontconfig_INCLUDE_DIRS}/.. ABSOLUTE)
				if(EXISTS ${FONTCONFIG_ROOT}/etc)
					set(FONTCONFIG_CONFIG_PATH ${FONTCONFIG_ROOT}/etc)
				else()
					set(FONTCONFIG_CONFIG_PATH /etc)
				endif()
				install_libs_so(${Fontconfig_LIBRARIES} ${INSTALL_PATH_LIB} FALSE)
				list(APPEND PACKAGE_DEPS "libexpat1")
        
				install(FILES ${FONTCONFIG_CONFIG_PATH}/fonts/fonts.conf DESTINATION ${INSTALL_PATH_ETC}/fontconfig/)
				install(DIRECTORY ${FONTCONFIG_ROOT}/share/fontconfig/conf.avail/ DESTINATION ${INSTALL_PATH_ETC}/fontconfig/conf.d)
			endif()
		endif()
	endif()
endif()

# libpng
if(WITH_AVCODEC OR WITH_AVFILTER OR (WITH_QT AND HAS_QTGUI))
	if(WIN32)
		if(MSVC)
			find_install_dll("libpng" ${INSTALL_PATH_BIN} FALSE)
		else()
			find_install_dll("png" ${INSTALL_PATH_BIN} FALSE)
		endif()
	elseif(APPLE)
		# Todo
	else()
		if(AUTONOMOUS_PACKAGE_BUILD)
			if(WITH_LIBPNG_SYSTEM)
				list(APPEND PACKAGE_DEPS "libpng16-16")
			else()
				find_install_lib_so("png" ${INSTALL_PATH_LIB} FALSE)
			endif()
		endif()
	endif()
endif()
