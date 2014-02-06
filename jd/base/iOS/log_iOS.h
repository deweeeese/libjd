#pragma once

EXTERN_C_BEGIN

// assumes UTF8 string, can't define UTF16 string literals, so the UTF16 version is not as useful as we hoped.
// For now, use %S as an iOS specific way to handle printing UTF16 in the console
void iOSThreadSafeLogUTF8( const char * fmt, ...);
//void iOSThreadSafeLogUTF16( const char * fmt, ...);

#ifdef __OBJC__
// NSString version of log
void NSQuietLog(NSString *format, ...);
#endif

EXTERN_C_END
