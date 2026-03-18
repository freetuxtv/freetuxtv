############################
# Add some compiler options
############################

#set(STACKSIZE 64000)
if(NOT MSVC)
	add_definitions (-Wall)
	#if(WIN32 AND CMAKE_COMPILER_IS_GNUCXX)
	#	set(CMAKE_EXE_LINKER_FLAGS "-Wl,--stack,${STACKSIZE}")
	#endif()
	#Uncomment to use ASAN debug
	#set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
#else()
	#set(CMAKE_EXE_LINKER_FLAGS "/STACK:${STACKSIZE}")
endif(NOT MSVC)

if(APPLE)
	message(STATUS "Using MacOS SDK at: ${CMAKE_OSX_SYSROOT}")
endif()

############################
# Check of some C files
############################

include (CheckIncludeFiles)
CHECK_INCLUDE_FILES (stdlib.h HAVE_STDLIB_H)
CHECK_INCLUDE_FILES (stdio.h HAVE_STDIO_H)
CHECK_INCLUDE_FILES (stddef.h HAVE_STDDEF_H)
CHECK_INCLUDE_FILES (string.h HAVE_STRING_H)
CHECK_INCLUDE_FILES (strings.h HAVE_STRINGS_H)
CHECK_INCLUDE_FILES (math.h HAVE_MATH_H)
CHECK_INCLUDE_FILES (regex.h HAVE_REGEX_H)
CHECK_INCLUDE_FILES (byteswap.h HAVE_BYTESWAP_H)
CHECK_INCLUDE_FILES (sys/time.h HAVE_SYS_TIME_H)

if (HAVE_STRING_H AND HAVE_STDLIB_H AND HAVE_STDIO_H AND HAVE_STDDEF_H AND HAVE_MATH_H)
	message(STATUS "All required headers are found")
else()
	message(FATAL_ERROR "Some required headers are not found")
endif()

############################
# Check of some C functions
############################

include (CheckFunctionExists)
CHECK_FUNCTION_EXISTS (strdup HAVE_STRDUP)
CHECK_FUNCTION_EXISTS (strcasestr HAVE_STRCASESTR)
CHECK_FUNCTION_EXISTS (strcasecmp HAVE_STRCASECMP)
CHECK_FUNCTION_EXISTS (strncasecmp HAVE_STRNCASECMP)
CHECK_FUNCTION_EXISTS (_stricmp HAVE_STRICMP)
CHECK_FUNCTION_EXISTS (_strnicmp HAVE_STRNICMP)
CHECK_FUNCTION_EXISTS (strndup HAVE_STRNDUP)
CHECK_FUNCTION_EXISTS (strtok_r HAVE_STRTOK_R)
CHECK_FUNCTION_EXISTS (abs HAVE_ABS)
#CHECK_FUNCTION_EXISTS (fabs HAVE_FABS)

if (HAVE_STRDUP AND (HAVE_STRCASESTR OR HAVE_STRICMP) AND (HAVE_STRNCASECMP OR HAVE_STRNICMP) AND HAVE_ABS)
	message(STATUS "All required functions are found")
else()
	message(FATAL_ERROR "Some required functions are not found")
endif()
