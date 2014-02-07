#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <tchar.h>
#include <malloc.h>

#define TARGET_OS_WINDOWS 1
#define TARGET_ENDIAN_LITTLE 1
#define TARGET_CPU_i386 1

// types
typedef unsigned __int64    uint64;
typedef __int64             int64;
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




//#include "WinUtils.h"


//#define LOG(...) PrintODS(__VA_ARGS__)
