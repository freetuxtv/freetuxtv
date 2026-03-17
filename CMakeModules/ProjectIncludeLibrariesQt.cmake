include (${CMAKE_CURRENT_LIST_DIR}/ProjectQtUsefulMacros.cmake)

# Mandatory options
if(NOT QT4_COMPONENTS AND NOT QT5_COMPONENTS AND NOT QT6_COMPONENTS)
	message(FATAL_ERROR "Please define the QT4_COMPONENTS, QT5_COMPONENTS or QT6_COMPONENTS variables")
endif(NOT QT4_COMPONENTS AND NOT QT5_COMPONENTS AND NOT QT6_COMPONENTS)

# Component checks from version
set(QT_PRE_FOUND FALSE)
if(NOT QT_PRECHECK_FOUND AND (NOT QT_USE_VERSION OR QT_USE_VERSION EQUAL 6))
	message(STATUS "Checking presence for Qt major version: 6")
	check_has_module(Qt6)
	if(HAS_MODULE_Qt6)
		set(QT_PRECHECK_FOUND TRUE)
		set(QT_PRECHECK_VERSION ${MODULE_Qt6_VERSION})
		set(QT_PRECHECK_VERSION_MAJOR ${MODULE_Qt6_VERSION_MAJOR})
		set(QT_PRECHECK_VERSION_MINOR ${MODULE_Qt6_VERSION_MINOR})
		set(QT_PRECHECK_COMPONENTS ${QT6_COMPONENTS})
		set(QT_USE_VERSION 6)
	endif()
endif()
if(NOT QT_PRECHECK_FOUND AND (NOT QT_USE_VERSION OR QT_USE_VERSION EQUAL 5))
	message(STATUS "Checking presence for Qt major version: 5")
	check_has_module(Qt5)
	if(HAS_MODULE_Qt5)
		set(QT_PRECHECK_FOUND TRUE)
		set(QT_PRECHECK_VERSION ${MODULE_Qt5_VERSION})
		set(QT_PRECHECK_VERSION_MAJOR ${MODULE_Qt5_VERSION_MAJOR})
		set(QT_PRECHECK_VERSION_MINOR ${MODULE_Qt5_VERSION_MINOR})
		set(QT_PRECHECK_COMPONENTS ${QT5_COMPONENTS})
		set(QT_USE_VERSION 5)
	endif()
endif()
if(NOT QT_PRECHECK_FOUND AND (NOT QT_USE_VERSION OR QT_USE_VERSION EQUAL 4))
	message(STATUS "Checking presence for Qt major version: 4")
	check_has_module(Qt4)
	if(HAS_MODULE_Qt4)
		set(QT_PRECHECK_FOUND TRUE)
		set(QT_PRECHECK_VERSION ${MODULE_Qt4_VERSION})
		set(QT_PRECHECK_VERSION_MAJOR ${MODULE_Qt4_VERSION_MAJOR})
		set(QT_PRECHECK_VERSION_MINOR ${MODULE_Qt4_VERSION_MINOR})
		set(QT_PRECHECK_COMPONENTS ${QT4_COMPONENTS})
		set(QT_USE_VERSION 4)
	endif()
endif()
if(NOT QT_PRECHECK_FOUND)
	if(QT_USE_VERSION)
		message(FATAL_ERROR "Unsupported Qt major version requested: ${QT_USE_VERSION}")
	else()
		message(FATAL_ERROR "Unable to find any Qt package: ${QT_USE_VERSION}")
	endif()
endif()

message(STATUS "Found Qt version: ${QT_PRECHECK_VERSION_MAJOR}.${QT_PRECHECK_VERSION_MINOR}")

qtx_filter_components("${QT_PRECHECK_COMPONENTS}" "${QT_PRECHECK_VERSION}" QT_COMPONENTS)

# Look for correct QT version and components
if(QT_PRECHECK_VERSION_MAJOR EQUAL 6)
	message(STATUS "Looking for Qt major version: 6")
	find_package (Qt6 6.4.0 COMPONENTS ${QT_COMPONENTS})
	qt_standard_project_setup(REQUIRES 6.4)
elseif(QT_PRECHECK_VERSION_MAJOR EQUAL 5)
	message(STATUS "Looking for Qt major version: 5")
	find_package (Qt5 5.1.0 COMPONENTS ${QT_COMPONENTS})
elseif(QT_PRECHECK_VERSION_MAJOR EQUAL 4)
	message(STATUS "Looking for Qt major version: 4")
	find_package (Qt4 4.7.0 COMPONENTS ${QT_COMPONENTS})
else()
	message(FATAL_ERROR "Unsupported Qt major version requested: ${QT_USE_VERSION}")
endif()

if(Qt6_FOUND AND NOT QT_FOUND)
	set(QT_FOUND ${Qt6_FOUND})
	# Qt version may not be defined (probably because there is no FindQt6.cmake)
	# If not defined we require at least qt 6.0. Find a better solution for qt6.
	if(NOT QT_VERSION_MAJOR)
		set(QT_VERSION_MAJOR ${Qt6_VERSION_MAJOR})
	endif()
	if(NOT QT_VERSION_MINOR)
		set(QT_VERSION_MINOR ${Qt6_VERSION_MINOR})
	endif()
	if(NOT QT_VERSION_PATCH)
		set(QT_VERSION_PATCH ${Qt6_VERSION_PATCH})
	endif()
endif()
if(Qt5_FOUND AND NOT QT_FOUND)
	set(QT_FOUND ${Qt5_FOUND})
	# Qt version may not be defined (probably because there is no FindQt5.cmake)
	# If not defined we require at least qt 5.0. Find a better solution for qt6.
	if(NOT QT_VERSION_MAJOR)
		set(QT_VERSION_MAJOR ${Qt5_VERSION_MAJOR})
	endif()
	if(NOT QT_VERSION_MINOR)
		set(QT_VERSION_MINOR ${Qt5_VERSION_MINOR})
	endif()
	if(NOT QT_VERSION_PATCH)
		set(QT_VERSION_PATCH ${Qt5_VERSION_PATCH})
	endif()
endif()
set(QT_VERSION "${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}.${QT_VERSION_PATCH}")

if(QT_FOUND)
	set(USE_QT 1)

	if(QT_USE_VERSION GREATER_EQUAL 6)
		# option for Qt 6
		if (QT_FEATURE_reduce_relocations)
			# Qt wants position independent code in certain circumstances - specifically "You must build your code with position
			# independent code if Qt was built with -reduce-relocations. Compile your code with -fPIC (and not with -fPIE)."
			set(CMAKE_POSITION_INDEPENDENT_CODE ON)
		endif()

		if (SINGLE_UNIT_COMPILATION)
			set(CMAKE_AUTOMOC ON)
		endif ()

		# override function to generate moc
		function(qtx_wrap_cpp arg1)
			if (NOT SINGLE_UNIT_COMPILATION)
				qt_wrap_cpp(${arg1} ${ARGN})
				set(${arg1} ${${arg1}} PARENT_SCOPE)
			endif ()
		endfunction(qtx_wrap_cpp)

		# override function to generate ts
		find_package(Qt${QT_VERSION_MAJOR}LinguistTools)
		if(Qt${QT_VERSION_MAJOR}LinguistTools_FOUND)
			function(qtx_create_translation arg1)
				qt_create_translation(arg1 ${ARGN})
				set(${arg1} ${${arg1}} PARENT_SCOPE)
			endfunction(qtx_create_translation)
			function(qtx_add_translation arg1)
				qt_add_translation(arg1 ${ARGN})
				set(${arg1} ${${arg1}} PARENT_SCOPE)
			endfunction(qtx_add_translation)
		else()
			message(FATAL_ERROR "Qt${QT_VERSION_MAJOR}LinguistTools package is required")
		endif()

		# override function to generate res
		function(qtx_add_resources arg1)
			qt_add_resources(${arg1} ${ARGN})
			set(${arg1} ${${arg1}} PARENT_SCOPE)
		endfunction(qtx_add_resources)
	elseif(QT_USE_VERSION EQUAL 5)
		# option for Qt 5
		if (Qt5_POSITION_INDEPENDENT_CODE)
			set(CMAKE_POSITION_INDEPENDENT_CODE ON)
		endif()

		if (SINGLE_UNIT_COMPILATION)
			set(CMAKE_AUTOMOC ON)
		endif ()

		# override function to generate moc
		function(qtx_wrap_cpp arg1)
			if (NOT SINGLE_UNIT_COMPILATION)
				qt5_wrap_cpp(${arg1} ${ARGN})
				set(${arg1} ${${arg1}} PARENT_SCOPE)
			endif ()
		endfunction(qtx_wrap_cpp)

		# override function to generate ts
		find_package(Qt${QT_VERSION_MAJOR}LinguistTools)
		if(Qt${QT_VERSION_MAJOR}LinguistTools_FOUND)
			function(qtx_create_translation arg1)
				qt5_create_translation(arg1 ${ARGN})
				set(${arg1} ${${arg1}} PARENT_SCOPE)
			endfunction(qtx_create_translation)
			function(qtx_add_translation arg1)
				qt5_add_translation(arg1 ${ARGN})
				set(${arg1} ${${arg1}} PARENT_SCOPE)
			endfunction(qtx_add_translation)
		else()
			message(FATAL_ERROR "Qt${QT_VERSION_MAJOR}LinguistTools package is required")
		endif()

		# override function to generate res
		function(qtx_add_resources arg1)
			qt5_add_resources(${arg1} ${ARGN})
			set(${arg1} ${${arg1}} PARENT_SCOPE)
		endfunction(qtx_add_resources)
	else()
		include (${QT_USE_FILE})
		add_definitions (${QT_DEFINITIONS})

		if (SINGLE_UNIT_COMPILATION)
			set(CMAKE_AUTOMOC ON)
		endif ()

		# override function to generate moc
		function(qtx_wrap_cpp arg1)
			if (NOT SINGLE_UNIT_COMPILATION)
				qt4_wrap_cpp(${arg1} ${ARGN})
				set(${arg1} ${${arg1}} PARENT_SCOPE)
			endif ()
		endfunction(qtx_wrap_cpp)

		# override function to generate ts
		function(qtx_create_translation arg1)
			qt4_create_translation(arg1 ${ARGN})
			set(${arg1} ${${arg1}} PARENT_SCOPE)
		endfunction(qtx_create_translation)
		function(qtx_add_translation arg1)
			qt4_add_translation(arg1 ${ARGN})
			set(${arg1} ${${arg1}} PARENT_SCOPE)
		endfunction(qtx_add_translation)

		# override function to generate res
		function(qtx_add_resources arg1)
			qt4_add_resources(${arg1} ${ARGN})
			set(${arg1} ${${arg1}} PARENT_SCOPE)
		endfunction(qtx_add_resources)
	endif()

	function(qtx_get_main_include_path list_includes_path main_include_path)
		set(best_include_path "")
		set(min_length -1)

		foreach(include_path ${list_includes_path})
			if(include_path MATCHES "^\\$")
				# Exclude special path in the form of "$<$<BOOL:$<TARGET_PROPERTY:Qt6::ZlibPrivate,_qt_module_has_private_headers>>:>"
				continue()
			endif()

			# Get string size
			string(LENGTH "${include_path}" length)

			# If best path, we keep it
			if(min_length EQUAL -1 OR length LESS min_length)
				set(min_length ${length})
				set(best_include_path ${include_path})
			endif()

		endforeach()

		set(${main_include_path} ${best_include_path} PARENT_SCOPE)
	endfunction(qtx_get_main_include_path)

	# Compute path on QT5
	if(QT_USE_VERSION GREATER_EQUAL 5)
		set(QT_DIR ${Qt${QT_VERSION_MAJOR}_DIR})

		# Get include main path
		qtx_get_main_include_path("${Qt${QT_VERSION_MAJOR}Core_INCLUDE_DIRS}" QT_INCLUDE_DIR)
		if(QT_INCLUDE_DIR)
			get_filename_component(QT_INCLUDE_DIR "${QT_INCLUDE_DIR}" REALPATH)
		else()
			message(FATAL_ERROR "Qt include path not found (qt path: ${QT_DIR})")
		endif()

		# Get library dir
		if(EXISTS ${QT_INCLUDE_DIR}/../lib)
			set(QT_LIBRARY_DIR ${QT_INCLUDE_DIR}/../lib/)
		elseif(EXISTS ${QT_INCLUDE_DIR}/../../lib)
			set(QT_LIBRARY_DIR ${QT_INCLUDE_DIR}/../../lib)
		else()
			find_library(QtCore_LIBRARY_PATH libQt${QT_VERSION_MAJOR}Core.so)
			if(QtCore_LIBRARY_PATH)
				get_filename_component(QT_LIBRARY_DIR ${QtCore_LIBRARY_PATH} DIRECTORY)
			endif()
		endif()
		if(QT_LIBRARY_DIR)
			get_filename_component(QT_LIBRARY_DIR "${QT_LIBRARY_DIR}" REALPATH)
		else()
			message(FATAL_ERROR "Qt library path not found (include path: ${QT_INCLUDE_DIR} from ${Qt${QT_VERSION_MAJOR}Core_INCLUDE_DIRS})")
		endif()

		# Get root dir
		if(EXISTS ${QT_LIBRARY_DIR}../qt.conf)
			set(QT_ROOT_DIR ${QT_LIBRARY_DIR}/..)
		elseif(EXISTS ${QT_LIBRARY_DIR}/../bin/qt.conf)
			set(QT_ROOT_DIR ${QT_LIBRARY_DIR}/..)
		elseif(EXISTS ${QT_LIBRARY_DIR}/../bin/qmake)
			set(QT_ROOT_DIR ${QT_LIBRARY_DIR}/..)
		elseif(EXISTS ${QT_LIBRARY_DIR}/qt${QT_VERSION_MAJOR})
			set(QT_ROOT_DIR ${QT_LIBRARY_DIR}/qt${QT_VERSION_MAJOR})
		endif()
		if(QT_ROOT_DIR)
			get_filename_component(QT_ROOT_DIR "${QT_ROOT_DIR}" REALPATH)
		else()
			message(FATAL_ERROR "Qt root path not found (include path: ${QT_INCLUDE_DIR})")
		endif()
		set(QT_BINARY_DIR ${QT_ROOT_DIR}/bin)
		set(QT_PLUGINS_DIR ${QT_ROOT_DIR}/plugins)
		set(QT_QML_DIR ${QT_ROOT_DIR}/qml)
		if(QT_USE_VERSION GREATER_EQUAL 6)
			set(QT_TRANSLATIONS_DIR ${QT_ROOT_DIR}/translations)
 		endif()
	endif()
	if(NOT QT_TRANSLATIONS_DIR)
		if(QT_USE_VERSION EQUAL 5)
			# If Qt5+, we get QT_TRANSLATIONS_DIR from the executable path
			get_target_property(QT${QT_VERSION_MAJOR}_QMAKE_EXECUTABLE Qt${QT_VERSION_MAJOR}::qmake IMPORTED_LOCATION)
			execute_process(COMMAND ${QT${QT_VERSION_MAJOR}_QMAKE_EXECUTABLE} -query QT_INSTALL_TRANSLATIONS
				OUTPUT_VARIABLE qt_translations_dir OUTPUT_STRIP_TRAILING_WHITESPACE)
			file(TO_CMAKE_PATH "${qt_translations_dir}" qt_translations_dir)
			set(QT_TRANSLATIONS_DIR ${qt_translations_dir} CACHE PATH "The location of the Qt translations" FORCE)
		endif()
	endif()
	message(STATUS "QT root path: ${QT_ROOT_DIR}")
	message(STATUS "QT include path: ${QT_INCLUDE_DIR}")
	message(STATUS "QT library path: ${QT_LIBRARY_DIR}")
	message(STATUS "QT binary path: ${QT_BINARY_DIR}")
	message(STATUS "QT plugins path: ${QT_PLUGINS_DIR}")
	message(STATUS "QT translations path: ${QT_TRANSLATIONS_DIR}")
	message(STATUS "QT qml path: ${QT_QML_DIR}")

	set(QT_USE_DEBUG_DLL FALSE)

	if (WIN32)
		# Find Qt dll directory
		find_path(QT_DLL_DIR NAMES QtCore4.dll QtCored4.dll Qt${QT_VERSION_MAJOR}Core.dll Qt${QT_VERSION_MAJOR}Cored.dll PATHS ${QT_BINARY_DIR} {QT_LIBRARY_DIR})
		if(NOT QT_DLL_DIR)
			message(FATAL "Qt dll directory not found")
		endif()
		find_path(QT_DLL_DEBUG_DIR NAMES QtCored4.dll Qt${QT_VERSION_MAJOR}Cored.dll Qt6Cored.dll PATHS ${QT_BINARY_DIR} {QT_LIBRARY_DIR})
		if(QT_DLL_DEBUG_DIR)
			set(QT_USE_DEBUG_DLL TRUE)
		endif()
		message(STATUS "Using Qt debug dll: ${QT_USE_DEBUG_DLL}")

		set(QT_TRANSLATIONS_DIR ${QT_DLL_DIR}/../translations)

		# Common library
		if(CMAKE_COMPILER_IS_GNUCXX)
			if(EXISTS "${QT_DLL_DIR}/libgcc_s_dw2-1.dll")
				INSTALL(FILES
					"${QT_DLL_DIR}/libgcc_s_dw2-1.dll"
					DESTINATION ${INSTALL_PATH_BIN}
				)
			endif()
			# Since 4.8.6 use libwinpthread
			if(EXISTS "${QT_DLL_DIR}/libwinpthread-1.dll")
				INSTALL(FILES
					"${QT_DLL_DIR}/libwinpthread-1.dll"
					DESTINATION ${INSTALL_PATH_BIN}
				)
			endif()
			if(EXISTS "${QT_DLL_DIR}/mingwm10.dll")
				INSTALL(FILES
					"${QT_DLL_DIR}/mingwm10.dll"
					DESTINATION ${INSTALL_PATH_BIN}
				)
			endif()
			# libgomp
			execute_process(COMMAND ${CMAKE_CXX_COMPILER} -print-file-name=libgomp-1.dll OUTPUT_VARIABLE TMP_DLL_PATH OUTPUT_STRIP_TRAILING_WHITESPACE)
			if(NOT TMP_DLL_PATH STREQUAL "" AND NOT TMP_DLL_PATH STREQUAL "libgomp-1.dll")
				install(FILES ${TMP_DLL_PATH} DESTINATION ${INSTALL_PATH_BIN})
			endif()
			# libwinpthread
			execute_process(COMMAND ${CMAKE_CXX_COMPILER} -print-file-name=libwinpthread-1.dll OUTPUT_VARIABLE TMP_DLL_PATH OUTPUT_STRIP_TRAILING_WHITESPACE)
			if(NOT TMP_DLL_PATH STREQUAL "" AND NOT TMP_DLL_PATH STREQUAL "libwinpthread-1.dll")
				install(FILES ${TMP_DLL_PATH} DESTINATION ${INSTALL_PATH_BIN})
			endif()
			# zlib
			execute_process(COMMAND ${CMAKE_CXX_COMPILER} -print-file-name=zlib1.dll OUTPUT_VARIABLE TMP_DLL_PATH OUTPUT_STRIP_TRAILING_WHITESPACE)
			if(NOT TMP_DLL_PATH STREQUAL "" AND NOT TMP_DLL_PATH STREQUAL "zlib1.dll")
				install(FILES ${TMP_DLL_PATH} DESTINATION ${INSTALL_PATH_BIN})
			endif()
		endif(CMAKE_COMPILER_IS_GNUCXX)

		# Add platform specific informations
		if(QT_USE_VERSION LESS 5)

		else()
			set(QT_PLATFORMS_COMPONENTS ${QT_PLATFORMS_COMPONENTS} qwindows)
		endif()

	endif(WIN32)

	set(QT_DEPS_VERSION "${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}")

	# Qt Components
	foreach(TMP_COMPONENT ${QT_COMPONENTS})
		if(QT_USE_VERSION LESS 5)
			string(SUBSTRING ${TMP_COMPONENT} 2 -1 TMP_MODULE)
			string(TOLOWER ${TMP_MODULE} TMP_MODULE)
		else()
			string(TOLOWER ${TMP_COMPONENT} TMP_MODULE)
			# Qt adds a "-" each time a module's name ends with a digit
			if(TMP_MODULE MATCHES "[(+0-9)]$")
				set(TMP_MODULE "${TMP_MODULE}-")
			endif()
		endif()

		if(NOT (QT_USE_VERSION LESS 5))
			include_directories(${Qt${QT_VERSION_MAJOR}${TMP_COMPONENT}_INCLUDE_DIRS})
			add_definitions (${Qt${QT_VERSION_MAJOR}${TMP_COMPONENT}_DEFINITIONS})

			set(QT_LIBRARIES ${QT_LIBRARIES} ${Qt${QT_VERSION_MAJOR}${TMP_COMPONENT}_LIBRARIES})
		endif()

		# Fix potential name change between component name and libname
		qtx_get_lib_name(${TMP_COMPONENT} TMP_LIBNAME)

		if(WIN32)
			install_qt_dll(${QT_DLL_DIR} ${TMP_LIBNAME} ${QT_VERSION_MAJOR} ${INSTALL_PATH_BIN})
		elseif(APPLE)
			if(INSTALL_PATH_FRAMEWORKS)
				IF(CMAKE_BUILD_TYPE MATCHES Release)
				  INSTALL(DIRECTORY "${QT_LIBRARY_DIR}/Qt${TMP_LIBNAME}.framework"
					  DESTINATION ${INSTALL_PATH_FRAMEWORKS}
					  PATTERN "Qt${TMP_LIBNAME}_debug" EXCLUDE
					  PATTERN "*.prl" EXCLUDE
				  )
				else()
				  INSTALL(DIRECTORY "${QT_LIBRARY_DIR}/Qt${TMP_LIBNAME}.framework"
					  DESTINATION ${INSTALL_PATH_FRAMEWORKS})
				endif()
			else()
				INSTALL(FILES "${QT_LIBRARY_DIR}/Qt${TMP_LIBNAME}.framework/Versions/${QT_VERSION_MAJOR}/Qt${TMP_LIBNAME}"
					DESTINATION ${INSTALL_PATH_LIB})
			endif()

			file(GLOB DBUS_LIBRARIES_INSTALL /usr/local/opt/dbus/lib/*.dylib)
			install(FILES ${DBUS_LIBRARIES_INSTALL} DESTINATION ${INSTALL_PATH_LIB})
		else()
			if(AUTONOMOUS_PACKAGE_BUILD)
				set(QT_LIBSO_NAME libQt${QT_VERSION_MAJOR}${TMP_LIBNAME}.so)
				find_install_lib_so(${QT_LIBSO_NAME} ${INSTALL_PATH_LIB} TRUE)
				if("${TMP_LIBNAME}" STREQUAL "Gui")
					set(QT_LIBSO_NAME libQt${QT_VERSION_MAJOR}XcbQpa.so.${QT_VERSION_MAJOR})
					find_install_lib_so(${QT_LIBSO_NAME} ${INSTALL_PATH_LIB} TRUE)
				elseif("${TMP_LIBNAME}" STREQUAL "QuickDialogs2")
					set(QT_LIBSO_NAME libQt${QT_VERSION_MAJOR}LabsFolderListModel.so.${QT_VERSION_MAJOR})
					find_install_lib_so(${QT_LIBSO_NAME} ${INSTALL_PATH_LIB} TRUE)
				endif()
			else()
				if(QT_USE_VERSION LESS 5)
					set(TMP_PACKAGE_NAME "libqt${QT_VERSION_MAJOR}-${TMP_MODULE}")
				else()
					if(${TMP_MODULE} STREQUAL "core")
						set(TMP_PACKAGE_NAME "libqt${QT_VERSION_MAJOR}${TMP_MODULE}${QT_VERSION_MAJOR}a")
					else()
						set(TMP_PACKAGE_NAME "libqt${QT_VERSION_MAJOR}${TMP_MODULE}${QT_VERSION_MAJOR}")
					endif()
				endif()
				list(APPEND PACKAGE_DEPS "${TMP_PACKAGE_NAME} (>= ${QT_DEPS_VERSION})")
			endif()
		endif()
	endforeach(TMP_COMPONENT)

	if(INSTALL_PATH_PLUGIN)
		set(INSTALL_PATH_QTPLUGINS ${INSTALL_PATH_PLUGIN})
	else()
		set(INSTALL_PATH_QTPLUGINS ${INSTALL_PATH_BIN})
	endif()

	if(INSTALL_PATH_QML)
		set(INSTALL_PATH_QTQML ${INSTALL_PATH_QML})
	else()
		set(INSTALL_PATH_QTQML ${INSTALL_PATH_BIN})
	endif()

	# Qt Languages
	if(QT_USE_VERSION LESS 5)
		list(APPEND PACKAGE_DEPS "qtcore4-l10n")
	else()
		if(AUTONOMOUS_PACKAGE_BUILD)
			if(QT_USE_VERSION GREATER_EQUAL 6)
				set(QT_USED_MODULES "qt;qtbase;qtmultimedia")
			else()
				set(QT_USED_MODULES "qt;qtbase;qtscript;qtmultimedia;qtxmlpatterns")
			endif()
			set(QT_USED_LANG "fr;sv")
			install_qm_files(${QT_TRANSLATIONS_DIR} "${QT_USED_MODULES}" "${QT_USED_LANG}" ${INSTALL_PATH_SHARE}/translations)
		else()
			if(QT_USE_VERSION EQUAL 6)
				list(APPEND PACKAGE_DEPS "qt6-translations-l10")
			endif()
			if(QT_USE_VERSION EQUAL 5)
				list(APPEND PACKAGE_DEPS "qttranslations5-l10n")
			endif()
		endif()
	endif()

	# Qt QML modules
	foreach(TMP_QML_MODULE ${QT_QML_MODULES})
		qt_qml_check_module_available(${TMP_QML_MODULE} ${QT_DEPS_VERSION} TMP_QML_MODULE_AVAILABLE)
		if(NOT ${TMP_QML_MODULE_AVAILABLE})
			message(STATUS "Excluded Qt Qml module ${TMP_QML_MODULE} for Qt version: ${QT_DEPS_VERSION}")
			continue()
		else()
			message(STATUS "Using Qt Qml module ${TMP_QML_MODULE} for Qt version: ${QT_DEPS_VERSION}")
		endif()

		qt_qml_module_directory(${TMP_QML_MODULE} QML_MODULE_${TMP_QML_MODULE}_DIRECTORY)
		qt_qml_module_plugin_name(${TMP_QML_MODULE} QML_MODULE_${TMP_QML_MODULE}_PLUGIN_NAME)
		qt_qml_module_has_package(${TMP_QML_MODULE} QML_MODULE_${TMP_QML_MODULE}_HAS_PACKAGE)
		qt_qml_module_has_plugins_qmltypes(${TMP_QML_MODULE} QML_MODULE_${TMP_QML_MODULE}_HAS_PLUGINS_QMLTYPES)

		set(TMP_QML_MODULE_DIRECTORY ${QML_MODULE_${TMP_QML_MODULE}_DIRECTORY})
		set(TMP_QML_MODULE_PLUGIN_NAME ${QML_MODULE_${TMP_QML_MODULE}_PLUGIN_NAME})
		set(TMP_QML_MODULE_HAS_PACKAGE ${QML_MODULE_${TMP_QML_MODULE}_HAS_PACKAGE})
		set(TMP_QML_MODULE_HAS_PLUGINS_QMLTYPES ${QML_MODULE_${TMP_QML_MODULE}_HAS_PLUGINS_QMLTYPES})
		set(TMP_QML_MODULE_INSTALL_FILES FALSE)

		if(NOT ${QML_MODULE_${TMP_QML_MODULE}_PLUGIN_NAME} STREQUAL "")
			if(WIN32)
				# Install DLL files
				if(QT_USE_DEBUG_DLL)
					install(FILES "${QT_QML_DIR}/${TMP_QML_MODULE_DIRECTORY}/${TMP_QML_MODULE_PLUGIN_NAME}.dll"
						DESTINATION ${INSTALL_PATH_QTQML}/${TMP_QML_MODULE_DIRECTORY} CONFIGURATIONS Release
					)
					install(FILES "${QT_QML_DIR}/${TMP_QML_MODULE_DIRECTORY}/${TMP_QML_MODULE_PLUGIN_NAME}d.dll"
						DESTINATION ${INSTALL_PATH_QTQML}/${TMP_QML_MODULE_DIRECTORY} CONFIGURATIONS Debug
					)
				else()
					install(FILES "${QT_QML_DIR}/${TMP_QML_MODULE_DIRECTORY}/${TMP_QML_MODULE_PLUGIN_NAME}.dll"
						DESTINATION ${INSTALL_PATH_QTQML}/${TMP_QML_MODULE_DIRECTORY}
					)
				endif()
				set(TMP_QML_MODULE_INSTALL_FILES TRUE)
			elseif(APPLE)
				# Install dylib files
				install(FILES "${QT_QML_DIR}/${TMP_QML_MODULE_DIRECTORY}/lib${TMP_QML_MODULE_PLUGIN_NAME}.dylib"
					DESTINATION ${INSTALL_PATH_QTQML}/${TMP_QML_MODULE_DIRECTORY}
				)
				if(EXISTS "${QT_QML_DIR}/${TMP_QML_MODULE_DIRECTORY}/lib${TMP_QML_MODULE_PLUGIN_NAME}_debug.dylib")
					install(FILES "${QT_QML_DIR}/${TMP_QML_MODULE_DIRECTORY}/lib${TMP_QML_MODULE_PLUGIN_NAME}_debug.dylib"
						DESTINATION ${INSTALL_PATH_QTQML}/${TMP_QML_MODULE_DIRECTORY} CONFIGURATIONS Debug
					)
				endif()
				set(TMP_QML_MODULE_INSTALL_FILES TRUE)
			else()
				if(AUTONOMOUS_PACKAGE_BUILD)
					# Install SO files
					set(TMP_SO_FILE "${QT_QML_DIR}/${TMP_QML_MODULE_DIRECTORY}/lib${TMP_QML_MODULE_PLUGIN_NAME}.so")
					message("-- Installing SO file: ${TMP_SO_FILE}")
					install(FILES "${TMP_SO_FILE}" DESTINATION ${INSTALL_PATH_QTQML}/${TMP_QML_MODULE_DIRECTORY})
					set(TMP_QML_MODULE_INSTALL_FILES TRUE)
				else()
					if(TMP_QML_MODULE_HAS_PACKAGE)
						list(APPEND PACKAGE_DEPS "qml-module-${TMP_QML_MODULE}")
					endif()
				endif()
			endif()
		endif()

		if(TMP_QML_MODULE_INSTALL_FILES)
			# Install qmldir file
			install(FILES "${QT_QML_DIR}/${TMP_QML_MODULE_DIRECTORY}/qmldir"
				DESTINATION ${INSTALL_PATH_QTQML}/${TMP_QML_MODULE_DIRECTORY}
			)
			# Install plugins.qmltypes file
			if(TMP_QML_MODULE_HAS_PLUGINS_QMLTYPES)
				if(QT_USE_VERSION EQUAL 6 AND ${TMP_QML_MODULE_DIRECTORY} STREQUAL "QtQuick/Window")
					install(FILES "${QT_QML_DIR}/${TMP_QML_MODULE_DIRECTORY}/quickwindow.qmltypes"
							DESTINATION ${INSTALL_PATH_QTQML}/${TMP_QML_MODULE_DIRECTORY}
					)
				elseif(QT_USE_VERSION EQUAL 6 AND ${TMP_QML_MODULE_DIRECTORY} STREQUAL "QtQml/WorkerScript")
					install(FILES "${QT_QML_DIR}/${TMP_QML_MODULE_DIRECTORY}/plugins.qmltypes"
							DESTINATION ${INSTALL_PATH_QTQML}
					)
				else()
					install(FILES "${QT_QML_DIR}/${TMP_QML_MODULE_DIRECTORY}/plugins.qmltypes"
							DESTINATION ${INSTALL_PATH_QTQML}/${TMP_QML_MODULE_DIRECTORY}
					)
				endif()
			endif ()
		endif()
	endforeach(TMP_QML_MODULE)

	foreach(TMP_COMPONENT ${QT_QML_MODULES_FILES})
		set(TMP_COMPONENT_INSTALL_FILES TRUE)
		if(QT_USE_VERSION EQUAL 6)
			if(${TMP_COMPONENT} STREQUAL "QtQml/Base/")
				set(TMP_COMPONENT_INSTALL_FILES FALSE)
			elseif(${TMP_COMPONENT} STREQUAL "QtQuick/Controls.2/")
				set(TMP_COMPONENT_INSTALL_FILES FALSE)
			elseif(${TMP_COMPONENT} STREQUAL "QtQuick/PrivateWidgets/")
				set(TMP_COMPONENT_INSTALL_FILES FALSE)
			elseif(${TMP_COMPONENT} STREQUAL "QtGraphicalEffects/")
				set(TMP_COMPONENT_INSTALL_FILES FALSE)
			endif()
		endif()

		if(TMP_COMPONENT_INSTALL_FILES)
			if(WIN32 OR APPLE OR AUTONOMOUS_PACKAGE_BUILD)
				set(TMP_QML_SRC_DIR ${QT_QML_DIR}/${TMP_COMPONENT})
				# Compute directory name
				string(FIND ${TMP_COMPONENT} "/" POSITION REVERSE)
				string(SUBSTRING ${TMP_COMPONENT} 0 ${POSITION} DIR)
				if(IS_DIRECTORY "${QT_QML_DIR}/${TMP_COMPONENT}")
					install_qml_directory("${TMP_QML_SRC_DIR}" "${INSTALL_PATH_QTQML}/${DIR}")
				else()
					install(FILES "${TMP_QML_SRC_DIR}" DESTINATION "${INSTALL_PATH_QTQML}/${DIR}")
				endif()
			endif()
		endif()
	endforeach(TMP_COMPONENT)

	foreach(TMP_COMPONENT ${QT_QML_MODULES_PLUGINS})
		set(TMP_COMPONENT_INSTALL_FILES FALSE)
		if(QT_USE_VERSION EQUAL 6)
			# For now only plugins supported since qt6
			set(TMP_COMPONENT_INSTALL_FILES TRUE)
		endif()

		if(TMP_COMPONENT_INSTALL_FILES)
			if(WIN32 OR APPLE OR AUTONOMOUS_PACKAGE_BUILD)
				# Get plugins dir
				get_filename_component(TMP_PLUGIN_BASE_DIRECTORY "${TMP_COMPONENT}" DIRECTORY)
				set(TMP_PLUGIN_DIRECTORY "${QT_QML_DIR}/${TMP_PLUGIN_BASE_DIRECTORY}")
				set(TMP_TARGET_DIR ${INSTALL_PATH_QTQML}/${TMP_PLUGIN_BASE_DIRECTORY})
				# Get plugin name
				get_filename_component(TMP_PLUGIN_NAME "${TMP_COMPONENT}" NAME)
				install_qml_plugin("${TMP_PLUGIN_DIRECTORY}" ${TMP_PLUGIN_NAME} "${TMP_TARGET_DIR}")
			endif()
		endif()
	endforeach(TMP_COMPONENT)

	# Qt tls plugins
	if(QT_USE_VERSION GREATER_EQUAL 6)
		install_qt_plugins("tls" "${QT_TLS_COMPONENTS}")
	endif()

	# Qt Platforms plugins (Qt5+ only)
	if(NOT QT_USE_VERSION LESS 5)
		install_qt_plugins("platforms" "${QT_PLATFORMS_COMPONENTS}")
	endif()

	if("${QT_PLATFORMS_COMPONENTS}" MATCHES "qxcb")
		install_qt_plugins_directory("xcbglintegrations")
	endif()

	if("${QT_PLATFORMS_COMPONENTS}" MATCHES "wayland")
		install_qt_plugins_directory("wayland-shell-integration")
		install_qt_plugins_directory("wayland-graphics-integration-client")
		install_qt_plugins_directory("wayland-decoration-client")
	endif()

	# Qt Styles plugins (Qt5+ only)
	if(QT_USE_VERSION GREATER_EQUAL 5)
		install_qt_plugins("styles" "${QT_STYLES_COMPONENTS}")
		if(${QT${QT_USE_VERSION}_STYLES_COMPONENTS})
			install_qt_plugins("styles" "${QT${QT_USE_VERSION}_STYLES_COMPONENTS}")
		endif()
	endif()

	# Qt ImageFormats plugins
	install_qt_plugins("imageformats" "${QT_IMAGEFORMATS_COMPONENTS}")

	# Qt GeoServices plugins
	if(NOT QT_USE_VERSION LESS 5)
		install_qt_plugins("geoservices" "${QT_GEOSERVICES_COMPONENTS}")
	endif()

	# Qt platform themes plugins
	if(QT_USE_VERSION GREATER_EQUAL 5)
		if(UNIX)
			install_qt_plugins("platformthemes" "${QT_PLATFORMTHEMES_COMPONENTS}")
		endif ()
	endif()

	# Qt Sql plugins
	foreach(TMP_COMPONENT ${QT_SQL_COMPONENTS})

		if(TMP_COMPONENT STREQUAL "qsqlmysql")
			set(TMP_MODULE "mysql")
		else()
			string(SUBSTRING ${TMP_COMPONENT} 1 -1 TMP_MODULE)
			string(TOLOWER ${TMP_MODULE} TMP_MODULE)
		endif()

		if (WIN32)
			install_qt_plugins_dll("${QT_PLUGINS_DIR}/sqldrivers" ${TMP_COMPONENT} ${QT_VERSION_MAJOR} "${INSTALL_PATH_QTPLUGINS}/sqldrivers")
		elseif(APPLE)
			INSTALL(FILES "${QT_PLUGINS_DIR}/sqldrivers/lib${TMP_COMPONENT}.dylib"
				DESTINATION ${INSTALL_PATH_QTPLUGINS}/sqldrivers
			)
			if(EXISTS "${QT_PLUGINS_DIR}/sqldrivers/lib${TMP_COMPONENT}_debug.dylib")
				INSTALL(FILES "${QT_PLUGINS_DIR}/sqldrivers/lib${TMP_COMPONENT}_debug.dylib"
					DESTINATION ${INSTALL_PATH_QTPLUGINS}/sqldrivers CONFIGURATIONS Debug
				)
			endif()
		else()
			if(AUTONOMOUS_PACKAGE_BUILD)
				install(FILES "${QT_PLUGINS_DIR}/sqldrivers/lib${TMP_COMPONENT}.so"
					DESTINATION ${INSTALL_PATH_QTPLUGINS}/sqldrivers
				)
			else()
				if(QT_USE_VERSION LESS 5)
					set(TMP_PACKAGE_NAME "libqt${QT_VERSION_MAJOR}-sql-${TMP_MODULE}")
				else()
					set(TMP_PACKAGE_NAME "libqt${QT_VERSION_MAJOR}sql${QT_VERSION_MAJOR}-${TMP_MODULE}")
				endif()
				list(APPEND PACKAGE_DEPS "${TMP_PACKAGE_NAME} (>= ${QT_DEPS_VERSION})")
			endif()
		endif()
	endforeach(TMP_COMPONENT)

	# Qt Audio plugins (Only in Qt5)
	if(QT_USE_VERSION EQUAL 5)
		install_qt_plugins("audio" "${QT_AUDIO_COMPONENTS}")
	endif()

	# Qt Multimedia plugins
	if(QT_USE_VERSION GREATER_EQUAL 6)
		install_qt_plugins("multimedia" "${QT_MULTIMEDIA_COMPONENTS}")
	endif()

	if(NOT QT_USE_VERSION LESS 5)
		set(QT_QTMAIN_LIBRARY ${Qt${QT_VERSION_MAJOR}Core_QTMAIN_LIBRARIES})
	endif()

endif(QT_FOUND)
