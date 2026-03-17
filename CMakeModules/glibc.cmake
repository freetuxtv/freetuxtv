# - Check glibc version
# CHECK_GLIBC_VERSION()
#
# Once done this will define
#
#   GLIBC_VERSION - glibc version
#
MACRO (CHECK_GLIBC_VERSION)
    execute_process(
            COMMAND ldd --version
            OUTPUT_VARIABLE LDD_VERSION_OUTPUT
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    string(REGEX MATCH "([0-9]+\\.[0-9]+)" GLIBC_VERSION "${LDD_VERSION_OUTPUT}")
    if(NOT GLIBC_VERSION)
        message(FATAL_ERROR "Unable to find GLIBC version")
    endif()
ENDMACRO (CHECK_GLIBC_VERSION)