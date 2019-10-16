/*
 * Copyright 2006 The Android Open Source Project
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */
#ifndef SkTypes_DEFINED
#  define SkTypes_DEFINED
#  include "include/core/SkPreConfig.h"
#  if  defined (SK_USER_CONFIG_HEADER)
#    include SK_USER_CONFIG_HEADER
#  else 
#    include "include/config/SkUserConfig.h"
#  endif
#  include "include/core/SkPostConfig.h"
#  include <stddef.h>
#  include <stdint.h>
/** \file SkTypes.h
*/

/** Called internally if we hit an unrecoverable error.
    The platform implementation must not return, but should either throw
    an exception or otherwise exit.
*/
SK_API extern void sk_abort_no_print(void);
#  ifndef SkDebugf
SK_API void SkDebugf(const char format[], ...);
#  endif
#  define SkASSERT_RELEASE(cond)	 \
        static_cast<void>( (cond) ? (void)0 : []{ SK_ABORT("assert(" #cond ")"); }() )
#  ifdef SK_DEBUG
#    define SkASSERT(cond)	 SkASSERT_RELEASE(cond)
#    define SkASSERTF	(cond, fmt, ...) static_cast<void>( (cond) ? (void)0 : [&]{ \
                                          SkDebugf(fmt"\n", __VA_ARGS__);        \
                                          SK_ABORT("assert(" #cond ")");         \
                                      }() )
#    define SkDEBUGFAIL(message)	        SK_ABORT(message)
#    define SkDEBUGFAILF	(fmt, ...)      SkASSERTF(false, fmt, ##__VA_ARGS__)
#    define SkDEBUGCODE	(...)            __VA_ARGS__
#    define SkDEBUGF	(...)               SkDebugf(__VA_ARGS__)
#    define SkAssertResult(cond)	        SkASSERT(cond)
#  else 
#    define SkASSERT(cond)	            static_cast<void>(0)
#    define SkASSERTF	(cond, fmt, ...) static_cast<void>(0)
#    define SkDEBUGFAIL(message)
#    define SkDEBUGFAILF	(fmt, ...)
#    define SkDEBUGCODE	(...)
#    define SkDEBUGF	(...)
#    define SkAssertResult(cond)	         if (cond) {} do {} while(false)
#  endif
////////////////////////////////////////////////////////////////////////////////

/** Fast type for unsigned 8 bits. Use for parameter passing and local
    variables, not for storage
*/
typedef unsigned U8CPU;
/** Fast type for unsigned 16 bits. Use for parameter passing and local
    variables, not for storage
*/
typedef unsigned U16CPU;
/** @return false or true based on the condition
*/
template <typename T>
static bool SkToBool(const T& x)
{
  return 0 != x;
}
static int16_t SK_MaxS16 = INT16_MAX;
static int16_t SK_MinS16 = -SK_MaxS16;
static int32_t SK_MaxS32 = INT32_MAX;
static int32_t SK_MinS32 = -SK_MaxS32;
static int32_t SK_NaN32 = INT32_MIN;
static int64_t SK_MaxS64 = INT64_MAX;
static int64_t SK_MinS64 = -SK_MaxS64;
static int32_t SkLeftShift(int32_t value, int32_t shift)
{
  return (int32_t) ((uint32_t) value << shift);
}
static int64_t SkLeftShift(int64_t value, int32_t shift)
{
  return (int64_t) ((uint64_t) value << shift);
}
////////////////////////////////////////////////////////////////////////////////

/** @return the number of entries in an array (not a pointer)
*/
#  define Define_SkArrayCountHelper	template <typename T, size_t N> char (&SkArrayCountHelper(T (&array)[N]))[N];
Define_SkArrayCountHelper();
#  define SK_ARRAY_COUNT(array)	 (sizeof(SkArrayCountHelper(array)))
////////////////////////////////////////////////////////////////////////////////
template <typename T>
static T SkAlign2(T x)
{
  return (x + 1) >> 1 << 1;
}
template <typename T>
static T SkAlign4(T x)
{
  return (x + 3) >> 2 << 2;
}
template <typename T>
static T SkAlign8(T x)
{
  return (x + 7) >> 3 << 3;
}
template <typename T>
static bool SkIsAlign2(T x)
{
  return 0 == (x & 1);
}
template <typename T>
static bool SkIsAlign4(T x)
{
  return 0 == (x & 3);
}
template <typename T>
static bool SkIsAlign8(T x)
{
  return 0 == (x & 7);
}
template <typename T>
static T SkAlignPtr(T x)
{
  return sizeof(void*) == 8 ? SkAlign8(x) : SkAlign4(x);
}
template <typename T>
static bool SkIsAlignPtr(T x)
{
  return sizeof(void*) == 8 ? SkIsAlign8(x) : SkIsAlign4(x);
}
typedef uint32_t SkFourByteTag;
static SkFourByteTag SkSetFourByteTag(char a, char b, char c, char d)
{
  return (((uint8_t) a << 24) | ((uint8_t) b << 16) | ((uint8_t) c << 8) | (uint8_t) d);
}
////////////////////////////////////////////////////////////////////////////////

/** 32 bit integer to hold a unicode value
*/
typedef int32_t SkUnichar;
/** 16 bit unsigned integer to hold a glyph index
*/
typedef uint16_t SkGlyphID;
/** 32 bit value to hold a millisecond duration
    Note that SK_MSecMax is about 25 days.
*/
typedef uint32_t SkMSec;
/** Maximum representable milliseconds; 24d 20h 31m 23.647s.
*/
static SkMSec SK_MSecMax = INT32_MAX;
/** The generation IDs in Skia reserve 0 has an invalid marker.
*/
static uint32_t SK_InvalidGenID = 0;
/** The unique IDs in Skia reserve 0 has an invalid marker.
*/
static uint32_t SK_InvalidUniqueID = 0;
static int32_t SkAbs32(int32_t value)
{
  SkASSERT(value != SK_NaN32);
  if (value < 0)
  {
    value = -value;
  }
  return value;
}
template <typename T>
static T SkTAbs(T value)
{
  if (value < 0)
  {
    value = -value;
  }
  return value;
}
static int32_t SkMax32(int32_t a, int32_t b)
{
  if (a < b)
  {
    a = b;
  }
  return a;
}
static int32_t SkMin32(int32_t a, int32_t b)
{
  if (a > b)
  {
    a = b;
  }
  return a;
}
template <typename T>
const T& SkTMin(const T& a, const T& b)
{
  return (a < b) ? a : b;
}
template <typename T>
const T& SkTMax(const T& a, const T& b)
{
  return (b < a) ? a : b;
}
template <typename T>
const T& SkTClamp(const T& x, const T& lo, const T& hi)
{
  return (x < lo) ? lo : SkTMin(x, hi);
}
/** @return value pinned (clamped) between min and max, inclusively.
*/
template <typename T>
static const T& SkTPin(const T& value, const T& min, const T& max)
{
  return SkTMax(SkTMin(value, max), min);
}
////////////////////////////////////////////////////////////////////////////////

/** Indicates whether an allocation should count against a cache budget.
*/
enum class SkBudgeted : bool
{
  kNo = false,
  kYes = true
};
/** Indicates whether a backing store needs to be an exact match or can be
    larger than is strictly necessary
*/
enum class SkBackingFit
{
  kApprox,
  kExact
};
#endif
