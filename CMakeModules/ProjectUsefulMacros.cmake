# Define some useful macro or function

# Here the list of macro defined
# - JOINLIST : join a list of item into an unique string with a separator
# - SUFFIXLIST : add a suffix to all list of item
# - install_if_not_exists : install a file without override if file exists
# - check_has_module : check if a cmake module exists

macro(JOINLIST list_name sep output)
	# create empty list - necessary?
	# prefix and suffix elements
	foreach(l ${${list_name}})
		if(NOT DEFINED list_name_TMP)
			SET(list_name_TMP ${l})
		else()
			SET(list_name_TMP ${list_name_TMP}${sep}${l})
		endif()
	endforeach()
	# replace list by tmp list
	SET(${output} ${list_name_TMP})
	UNSET(list_name_TMP)
endmacro(JOINLIST)

macro(SUFFIXLIST list_name suffix output)
	# create empty list - necessary?
	# prefix and suffix elements
	foreach(l ${${list_name}})
		SET(list_name_TMP ${list_name_TMP} ${l}${suffix})
	endforeach()
	# replace list by tmp list
	SET(${output} ${list_name_TMP})
	UNSET(list_name_TMP)
endmacro(SUFFIXLIST)

function(install_if_not_exists src dest)
  if(NOT IS_ABSOLUTE "${src}")
    set(src "${CMAKE_CURRENT_SOURCE_DIR}/${src}")
  endif()
  get_filename_component(src_name "${src}" NAME)
  if (NOT IS_ABSOLUTE "${dest}")
    set(dest "${CMAKE_INSTALL_PREFIX}/${dest}")
  endif()
  install(CODE "
    if(NOT EXISTS \"\$ENV{DESTDIR}${dest}/${src_name}\")
      #file(INSTALL \"${src}\" DESTINATION \"${dest}\")
      message(STATUS \"Installing: \$ENV{DESTDIR}${dest}/${src_name}\")
      execute_process(COMMAND \${CMAKE_COMMAND} -E copy \"${src}\"
                      \"\$ENV{DESTDIR}${dest}/${src_name}\"
                      RESULT_VARIABLE copy_result
                      ERROR_VARIABLE error_output)
      if(copy_result)
        message(FATAL_ERROR \${error_output})
      endif()
    else()
      message(STATUS \"Skipping  : \$ENV{DESTDIR}${dest}/${src_name}\")
    endif()
  ")
endfunction(install_if_not_exists)

function(check_has_module Module)
  find_package(${Module} QUIET)
  if(NOT DEFINED ${Module}_DIR)
    set(HAS_MODULE_${Module} TRUE PARENT_SCOPE)
  elseif(${Module}_DIR)
    set(HAS_MODULE_${Module} TRUE PARENT_SCOPE)
  else()
    set(HAS_MODULE_${Module} FALSE PARENT_SCOPE)
  endif()
  if(${Module}_VERSION)
	  set(MODULE_${Module}_VERSION ${${Module}_VERSION} PARENT_SCOPE)
  endif()
  if(${Module}_VERSION_MAJOR)
	  set(MODULE_${Module}_VERSION_MAJOR ${${Module}_VERSION_MAJOR} PARENT_SCOPE)
  endif()
  if(${Module}_VERSION_MINOR)
	  set(MODULE_${Module}_VERSION_MINOR ${${Module}_VERSION_MINOR} PARENT_SCOPE)
  endif()
endfunction()

function(list_include_directories)
    get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
    foreach(dir ${dirs})
      message(STATUS "Include dir = '${dir}'")
    endforeach()
endfunction()

macro(get_list_directories retval curdir return_relative)
    file(GLOB sub-dir RELATIVE ${curdir} *)
    set(list_of_dirs "")
    foreach(dir ${sub-dir})
      if(IS_DIRECTORY ${curdir}/${dir})
        if (${return_relative})
          set(list_of_dirs ${list_of_dirs} ${dir})
        else()
          set(list_of_dirs ${list_of_dirs} ${curdir}/${dir})
        endif()
      endif()
    endforeach()
    set(${retval} ${list_of_dirs})
endmacro()


macro(get_msvc_arch_dir VAR)
	if("${CMAKE_HOST_SYSTEM_PROCESSOR}" STREQUAL "AMD64")
		set(${VAR} "x64")
	else()
		set(${VAR} "unknown")
	endif()
endmacro()

function(install_file_or_symlink file_path install_path_lib)

	if(EXISTS "${file_path}")

		# get file info
		get_filename_component(FILE_DIR "${file_path}" DIRECTORY)
		get_filename_component(FILE_NAME "${file_path}" NAME)

		set(NEW_FILE_DIR ${CMAKE_CURRENT_BINARY_DIR}/tmp_install)
		set(NEW_FILE_PATH ${NEW_FILE_DIR}/${FILE_NAME})

		if(NOT EXISTS "${NEW_FILE_DIR}")
			message(STATUS "Creating file directory ${NEW_FILE_DIR}")
			file(MAKE_DIRECTORY ${NEW_FILE_DIR})
		endif()

		if(IS_SYMLINK "${file_path}")

			# get target info
			file(READ_SYMLINK "${file_path}" LINK_TARGET)
			if(NOT IS_ABSOLUTE "${LINK_TARGET}")
				set(LINK_TARGET "${FILE_DIR}/${LINK_TARGET}")
			endif()
			get_filename_component(LINK_TARGET_DIR "${LINK_TARGET}" DIRECTORY)
			get_filename_component(LINK_TARGET_NAME "${LINK_TARGET}" NAME)

			# install target
			if(EXISTS "${LINK_TARGET}")
				install_file_or_symlink(${LINK_TARGET} ${install_path_lib})
			endif()

			# create a new link in lib and install it
			if(NOT ${FILE_NAME} STREQUAL ${LINK_TARGET_NAME})
				message(STATUS "Creating symbolic link ${FILE_NAME} on ${LINK_TARGET_NAME}")
				file(CREATE_LINK ${LINK_TARGET_NAME} ${NEW_FILE_PATH} SYMBOLIC)
				install(FILES ${NEW_FILE_PATH} DESTINATION ${install_path_lib})
			endif()
		else()
			message(STATUS "Copying library ${file_path}")
			file(COPY ${file_path} DESTINATION ${NEW_FILE_DIR})
			install(FILES ${NEW_FILE_PATH} DESTINATION ${install_path_lib}
				PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
				GROUP_EXECUTE GROUP_READ WORLD_EXECUTE WORLD_READ)
		endif()
	else()
		message(FATAL_ERROR "file not exists: ${file_path}")
	endif()
endfunction(install_file_or_symlink)

function(install_libs_so libs install_path_lib)
	foreach(lib ${libs})
		get_filename_component(LIBRARY_PATH ${lib} DIRECTORY)
		get_filename_component(LIBRARY_NAME ${lib} NAME)
		file(GLOB LIBRARIES_INSTALL ${LIBRARY_PATH}/${LIBRARY_NAME}*)
		# Install each lib found and it's pointed link if any
		foreach(LIBRARY_INSTALL ${LIBRARIES_INSTALL})
			install_file_or_symlink(${LIBRARY_INSTALL} ${install_path_lib})
		endforeach()
	endforeach()
endfunction(install_libs_so)

function(find_install_lib_so libname install_path_lib required)
	find_library(FIND_INSTALL_LIB_SO_${libname}_PATH ${libname})
	if(FIND_INSTALL_LIB_SO_${libname}_PATH)
		install_libs_so("${FIND_INSTALL_LIB_SO_${libname}_PATH}" ${install_path_lib})
	else()
		if(required)
			message(FATAL_ERROR "Unable to find library: ${libname}")
		endif()
	endif()
endfunction(find_install_lib_so)

function(find_install_lib_so_in_subdir libname install_path_lib path_suffixes required)
	find_library(FIND_INSTALL_LIB_SO_${libname}_PATH ${libname} PATH_SUFFIXES ${path_suffixes})
	if(FIND_INSTALL_LIB_SO_${libname}_PATH)
		install_libs_so("${FIND_INSTALL_LIB_SO_${libname}_PATH}" ${install_path_lib})
	else()
		if(required)
			message(FATAL_ERROR "Unable to find library: ${libname}")
		endif()
	endif()
endfunction(find_install_lib_so_in_subdir)

function(find_install_libs_so libs install_path_lib required)
	foreach(lib ${libs})
		if(EXISTS "${lib}")
			install_libs_so(${lib} ${install_path_lib})
		else()
			find_install_lib_so(${lib} ${install_path_lib} ${required})
		endif()
	endforeach()
endfunction(find_install_libs_so)

function(find_install_dll libname install_path_dll required)
	find_library(FIND_INSTALL_DLL_${libname}_LIB_FILEPATH ${libname})
	if(FIND_INSTALL_DLL_${libname}_LIB_FILEPATH)
		# cmake 2.8.9 don't support DIRECTORY
		#get_filename_component(tmp_lib_path ${FIND_INSTALL_DLL_${libname}_LIB_FILEPATH} DIRECTORY)
		#get_filename_component(tmp_bin_path ${tmp_lib_path}/../bin ABSOLUTE)
		# Add debug for MSVC
		get_filename_component(tmp_bin_path ${FIND_INSTALL_DLL_${libname}_LIB_FILEPATH}/../../bin ABSOLUTE)
		file(GLOB FIND_INSTALL_DLL_${libname}_FILES ${tmp_bin_path}/*${libname}*.dll)
		foreach(file_install ${FIND_INSTALL_DLL_${libname}_FILES})
				message(STATUS "Copying library ${file_install}")
				install(FILES ${file_install} DESTINATION ${install_path_dll})

				# Install also PDB file in debug
				if (CMAKE_BUILD_TYPE STREQUAL "Debug")
					string(REGEX REPLACE "\\.dll$" ".pdb" pdb_file ${file_install})
					if (EXISTS "${pdb_file}")
						message(STATUS "Copying PDB file ${pdb_file}")
						install(FILES ${pdb_file} DESTINATION ${install_path_dll})
					endif ()
				endif ()

		endforeach()
	else()
		if(required)
			message(FATAL_ERROR "Unable to find dll: ${libname}")
		endif()
	endif()
endfunction(find_install_dll)

function(find_install_dll_in_path libname search_path install_path_dll required)
	file(GLOB FIND_INSTALL_DLL_IN_PATH_${libname}_FILES ${search_path}/*${libname}*.dll)
	foreach(file_install ${FIND_INSTALL_DLL_IN_PATH_${libname}_FILES})
			message(STATUS "Copying library ${file_install}")
			install(FILES ${file_install} DESTINATION ${install_path_dll})
	endforeach()
endfunction(find_install_dll_in_path)

# Only working in cmake 3.16
function(patch_program_interpreter program_path interpreter_path)
	install(CODE "set(PROGRAM_PATH \"${program_path}\")")
	install(CODE "set(INTERPRETER_PATH \"${interpreter_path}\")")

	install(CODE [[
		message(STATUS "Patching program interpreter")
		if(NOT "$ENV{DESTDIR}" STREQUAL "")
			set(INSTALL_TMP_PATH "$ENV{DESTDIR}/${CMAKE_INSTALL_PREFIX}")
		else()
			set(INSTALL_TMP_PATH "${CMAKE_INSTALL_PREFIX}")
		endif()
		execute_process(COMMAND patchelf --set-interpreter ${CMAKE_INSTALL_PREFIX}/${INTERPRETER_PATH} ${INSTALL_TMP_PATH}/${PROGRAM_PATH})
	]])
endfunction(patch_program_interpreter)

macro(install_exe_deps exename install_path_lib)
	install(CODE "set(TARGET_NAME \"${exename}\")")
	install(CODE "set(DESTINATION_PATH \"${install_path_lib}\")")

	install(CODE [[
	  file(GET_RUNTIME_DEPENDENCIES
		EXECUTABLES ${TARGET_NAME}
		RESOLVED_DEPENDENCIES_VAR RESOLVED_DEPS
		UNRESOLVED_DEPENDENCIES_VAR UNRESOLVED_DEPS
	  )
	  foreach(_file ${RESOLVED_DEPS})
		file(INSTALL
		  DESTINATION "${DESTINATION_PATH}"
		  TYPE SHARED_LIBRARY
		  FOLLOW_SYMLINK_CHAIN
		  FILES "${_file}"
		)
	  endforeach()
	  list(LENGTH UNRESOLVED_DEPS UNRESOLVED_DEPS_COUNT)
	  if("${UNRESOLVED_DEPS_COUNT}" GREATER 0)
		message(WARNING "Unresolved dependencies detected!")
	  endif()
	]])
endmacro(install_exe_deps)

function(generate_single_unit_sources TARGET_NAME SOURCE_FILES SINGLE_UNIT_FILES)
	set(SINGLE_UNIT_C_FILE "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}-generated-single-unit.c")
	set(SINGLE_UNIT_CXX_FILE "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}-generated-single-unit.cpp")

	file(REMOVE "${SINGLE_UNIT_C_FILE}" "${SINGLE_UNIT_CXX_FILE}")

	# On Windows, seek for winsock2.h to include it in first
	if(WIN32)
		set(WINSOCK2_FOUND false)
		foreach(SOURCE_FILE ${SOURCE_FILES})
			file(READ "${SOURCE_FILE}" SOURCE_CONTENT)
			if(SOURCE_CONTENT MATCHES "winsock2.h|curl.h")
				set(WINSOCK2_FOUND true)
				break()
			endif()
		endforeach()

		if (WINSOCK2_FOUND)
			file(APPEND "${SINGLE_UNIT_CXX_FILE}" "#ifdef HAVE_CONFIG_H\n")
			file(APPEND "${SINGLE_UNIT_CXX_FILE}" "#include <config.h>\n")
			file(APPEND "${SINGLE_UNIT_CXX_FILE}" "#endif\n\n")
			file(APPEND "${SINGLE_UNIT_CXX_FILE}" "#include <winsock2.h>\n\n")
		endif()
	endif()

	foreach(SOURCE_FILE ${SOURCE_FILES})
		# Include only .c and .cpp file
		if(SOURCE_FILE MATCHES ".+\\.c$")
			set(SINGLE_UNIT_C_FILE_CREATED TRUE)
			file(APPEND "${SINGLE_UNIT_C_FILE}" "#include \"${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE_FILE}\"\n")
		elseif(SOURCE_FILE MATCHES ".+\\.cpp$")
			file(APPEND "${SINGLE_UNIT_CXX_FILE}" "#include \"${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE_FILE}\"\n")
		endif()
	endforeach()

	# Allways add cxx file but check if c file was created
	if(SINGLE_UNIT_C_FILE_CREATED)
		set(SINGLE_UNIT_FILES "${SINGLE_UNIT_CXX_FILE}" "${SINGLE_UNIT_C_FILE}" PARENT_SCOPE)
	else()
		set(SINGLE_UNIT_FILES "${SINGLE_UNIT_CXX_FILE}" PARENT_SCOPE)
	endif()
endfunction()
