#pragma once

// ConfigPlatform_* headers should be set up these conditionals:
// TARGET_OS_{WINDOWS|IPHONE|MAC}
// TARGET_CPU_{i386|ARM}
// TARGET_ENDIAN_{LITTLE|BIG}

#ifdef WIN32
#include <jd/base/win/plat_win.h>
#elif defined(__GNUC__) && ( defined(__APPLE_CPP__) || defined(__APPLE_CC__) )
#include <jd/base/iOS/plat_iOS.h>
#elif defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__)
#include <jd/base/mac/plat_mac.h>
#elif ANDROID
#include <jd/base/android/plat_android.h>
#endif
