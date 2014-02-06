#pragma once

#ifdef _MANAGED
#define BUILD_CLR_MANAGED 1
#else
#define BUILD_CLR_MANAGED 0
#endif

// on android, ndk-build NDK_DEBUG=0 creates a define NDEBUG=1, otherwise it's undefined.
// so when NDEBUG is 0 or undefined, turn debugging on, otherwise keep it off.
#if !TARGET_OS_ANDROID
#if !defined(NDEBUG)
#define _DEBUG 1
#endif
#endif

#if defined(_DEBUG)
// this used to be if !NDEBUG, but symbols evaluate to 0 if they're not defined
#define BUILD_DEBUG     1
#define BUILD_FINAL     0
#else
#define BUILD_DEBUG     0
#define BUILD_FINAL     1
#endif

// deploy -- DO NOT ENABLE UNLESS PREPARING FOR APP SUBMISSION!
#ifdef _DEPLOY
#define DEPLOY_TO_CUSTOMER 1
#else
#define DEPLOY_TO_CUSTOMER 0
#endif

#endif
