/*
 * p_string.h : This file contains portable declaration for string function
 *
 *  Created on: 6 mars 2012
 *      Author: ebeuque
 */

#ifndef TOOLKIT_CORE_PSTRING_H_
#define TOOLKIT_CORE_PSTRING_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#ifdef __cplusplus
	extern "C" {
#endif

#define p_strlen strlen
#define p_strcmp strcmp
#define p_strncmp strncmp
#define p_strchr strchr

#define p_strtol strtol
#define p_strtoul strtoul
#define p_strtod strtod

#ifdef MSVC
#define p_strtoll _strtoi64
#define p_strtoull _strtoui64
#define p_sscanf sscanf_s
#define p_strdup _strdup
#define p_strcpy strcpy_s
#define p_strncpy(d, s, c) strncpy_s(d, c+1, s, _TRUNCATE)
#define p_strcasecmp _stricmp
#define p_strncasecmp _strnicmp
#define p_sprintf_s(b, s, f, ...) sprintf_s(b, s, f, __VA_ARGS__)
#define p_snprintf(b, n, f, ...) _snprintf_s(b, n, _TRUNCATE, f, __VA_ARGS__)
#define p_vsnprintf(b, n, f, ...) _vsnprintf_s(b, n, _TRUNCATE, f, __VA_ARGS__)
#define p_vscprintf(format, args) _vscprintf(format, args)
#define p_strerror_r(errnum, buf, buflen) strerror_s(buf, buflen, errnum)
#else
#define p_strtoll strtoll
#define p_strtoull strtoull
#define p_sscanf sscanf
#define p_strdup strdup
#define p_strcpy(dst, max_size, src) strcpy(dst, src)
#define p_strncpy strncpy
#define p_strcasecmp strcasecmp
#define p_strncasecmp strncasecmp
#define p_sprintf_s(b, s, f, ...) sprintf(b, f, __VA_ARGS__)
#define p_snprintf snprintf
#define p_vsnprintf vsnprintf
#define p_vscprintf(format, args) vsnprintf(NULL, 0, format, args)
#define p_strerror_r(errnum, buf, buflen) strncpy(buf, strerror(errnum), buflen)
#endif

#define p_strftime strftime

#define p_streq(str1, str2) (str1 == NULL ? (str2 == NULL) : (str2 == NULL ? 0 : strcmp(str1, str2) == 0))

#define p_strstr strstr
#define p_strcspn strcspn

#ifdef __cplusplus
	}
#endif

#endif /* TOOLKIT_CORE_PSTRING_H_ */
