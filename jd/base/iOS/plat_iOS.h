#pragma once

#include <CoreFoundation/CoreFoundation.h>

#include <MacTypes.h>
#include <stdint.h>
#include <string.h>

// TARGET_OS_IPHONE  already defined
// TARGET_CPU_*  already defined

#if TARGET_RT_LITTLE_ENDIAN
#define TARGET_ENDIAN_LITTLE    1
#define TARGET_ENDIAN_BIG       0
#elif TARGET_RT_BIG_ENDIAN
#define TARGET_ENDIAN_LITTLE    0
#define TARGET_ENDIAN_BIG       1
#endif


// types
 
typedef UInt64  uint64;
typedef SInt64  int64;
typedef UInt32  uint32;
typedef SInt32  int32;
typedef UInt16  uint16;
typedef SInt16  int16;
typedef UInt8   uint8;
typedef SInt8   int8;
typedef UInt8   byte;

typedef float   real32;
typedef double  real64;

#if TARGET_CPU_ARM 
// ...
#elif TARGET_CPU_ARM64  // ...  maybe some CPU_BITS_64?
#define CPU_NEON_ASM_BUGGY_REGISTER_CLOBBERING 1
#endif


#if __ARM_NEON__
#define CPU_HAS_NEON 1
#else
#define CPU_HAS_NEON 0
#endif
