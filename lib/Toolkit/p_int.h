/*
 * p_int.h
 *
 *  Created on: 25 oct. 2012
 *      Author: ebeuque
 */

#ifndef TOOLKIT_CORE_PINT_H_
#define TOOLKIT_CORE_PINT_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <Toolkit/Core/p_arch.h>

#ifndef MSVC
// GCC, Mingw32...
#define __STDC_FORMAT_MACROS
#ifdef __cplusplus
extern "C" {
#endif
#include <inttypes.h>
#ifdef __cplusplus
}
#endif
#else
// MSVC compiler
#include <stdint.h>

#if _MSC_VER>=1920 // From VS 2019
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#else
#include "p_intmacros.h"
#endif

#endif /* MSVC */

#ifdef WIN32
#define PRI0xPTR     "0x%p"
#else
#define PRI0xPTR     "%p"
#endif

// Align the size to nearest nb_pixels alignment
// NOTE: this macro must be use when the pixels are coded with 8 bits (aka YUV sample)
#define p_size_align(size, nb_pixels) (((size)+(nb_pixels)-1)&~((nb_pixels)-1))

// Pad to the nearest 32bits alignment
#define p_pad32(width) (((width)+3)&(~3))

#ifdef USE_64BITS
#define PRIuPrintMAX 	"llu"
typedef unsigned long long   uintprintmax_t;
#else
#define PRIuPrintMAX 	"lu"
typedef unsigned long  uintprintmax_t;
#endif

#endif /* TOOLKIT_CORE_PINT_H_ */
