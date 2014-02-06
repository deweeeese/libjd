#include "log_iOS.h"

EXTERN_C_BEGIN

void NSQuietLog(NSString *format, ...)
{
    if (format == nil) {
        printf("nil\n");
        return;
    }
    // Get a reference to the arguments that follow the format parameter
    va_list argList;
    va_start(argList, format);
    // Perform format string argument substitution, reinstate %% escapes, then print
    NSString *s = [[NSString alloc] initWithFormat:format arguments:argList];
    LOG("%s", [[s stringByReplacingOccurrencesOfString:@"%%" withString:@"%%%%"] UTF8String]);
#if ARC_ENABLED
    [s release];
#endif
    va_end(argList);
}


void iOSThreadSafeLogUTF8( const char * fmt, ... )
{
    if ( fmt==NULL || fmt[0]==0 )
        fmt = " ";
    va_list argList;
    va_start(argList, fmt);
	NSLogv( [NSString stringWithCString:fmt encoding:NSUTF8StringEncoding], argList );
    va_end(argList);
}

EXTERN_C_END