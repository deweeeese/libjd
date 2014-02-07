#pragma once

#include <stdint.h>
#include <string.h>

#define TARGET_OS_ANDROID 1
#define TARGET_CPU_ARM 1
#define TARGET_ENDIAN_LITTLE    1

#include <stdbool.h>
#include <limits.h>
//#include <asm/signal.h>
#include <sys/types.h>
//#include "AndroidUtils.h"

// logging

#if DEPLOY_TO_CUSTOMER
#define LOGGING_ENABLED 0
#else
#define LOGGING_ENABLED 1
#endif

#if LOGGING_ENABLED
#include <android/log.h>
// @TODO this is complete crap; make a Logging.h and specialize it with functions and stuff
#define ANDROID_LOGE(tag,msg) __android_log_write(ANDROID_LOG_ERROR, tag, msg)
#define ANDROID_LOG(tag,msg) __android_log_write(ANDROID_LOG_INFO, tag, msg)
#define LOG(...) {char buf[256];int amount=snprintf(buf,256,__VA_ARGS__);if(amount==0){buf[0]=' '; buf[1]=0;};ANDROID_LOG("QV",buf);}
#else
#define LOG(...) /**/
#endif


// types
typedef unsigned long long  uint64;
typedef long long           int64;
typedef unsigned long int   uint32;
typedef signed long int     int32;
typedef unsigned short      uint16;
typedef signed short        int16;
typedef unsigned char       uint8;
typedef signed char         int8;
typedef unsigned char       byte;

typedef float               real32;
typedef double              real64;



// all platform-specific POD types are defined by now
typedef uint32          FourCharCode;
typedef FourCharCode    OSType;

// if compiling arm, assume we're compiling for armv7... blarg
#if TARGET_CPU_ARM
#if __ARM_NEON__
#define CPU_HAS_NEON 1
// ndk r7 gcc generates an internal compiler error for some neon code
// turn this off once the compiler bug is fixed.
// see http://code.google.com/p/android/issues/detail?id=22336  for the bug report and patch tracking
// stackoverflow: http://stackoverflow.com/questions/3441297/internal-compiler-error-when-compiling-native-android-code
//
#define ANDROID_GCC_NEON_BUG BUILD_DEBUG

#else
#define CPU_HAS_NEON 0
#endif
#endif


#if CPU_HAS_NEON

// Move this to another file...
#include <arm_neon.h>

// Uh... vdupq_u16 doesn't work - 'unrecognizable insn' internal compiler error.
inline uint16x8_t vrep_u16x8(uint16_t a)
{
	uint16x4_t q = vmovn_u16(vdupq_n_u32(a));
	return vcombine_u16(q, q);
}

inline float32x4_t vrep_f32x4(uint32_t a)
{
	return vcvtq_f32_u32(vdupq_n_u32(a));
}

#endif