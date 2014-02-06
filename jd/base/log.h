#pragma once


// logging

#if DEPLOY_TO_CUSTOMER
#define LOGGING_ENABLED 0
#else
#define LOGGING_ENABLED 1
#endif

#define LOGGING_THREADSAFE 1

#if LOGGING_ENABLED
#if LOGGING_THREADSAFE
#if TARGET_OS_IPHONE
#define LOG(...) {iOSThreadSafeLogUTF8(__VA_ARGS__);}
#else
#include <stdio.h>
#define LOG(...) {printf(__VA_ARGS__);printf("\n");}
#endif
#else
#define LOG(...) {printf(__VA_ARGS__);printf("\n");}
#endif
// Use like NSLog(@"-_-"),
#define NSLOG(...) NSQuietLog(__VA_ARGS__)
#else
#define LOG(...) /**/
#define NSLOG(...) /**/
#endif



#if TARGET_OS_IPHONE
#include <jd/base/iOS/log_iOS.h>
#endif

