#pragma once


// ASSERT is enabled in release, but not final
// This helps catch evil optimization bugs!
#if BUILD_DEBUG

#if TARGET_OS_IPHONE
#define DEBUG_BREAK() { kill( getpid(), SIGINT ); }
// [jd] can't continue after I hit one of these...
//#define DEBUG_BREAK() __asm("trap")
#elif TARGET_OS_WINDOWS
#define DEBUG_BREAK() __debugbreak()
#elif TARGET_OS_ANDROID
#define DEBUG_BREAK() { raise(SIGTRAP); }
#else
#define DEBUG_BREAK()
#endif

EXTERN_C_BEGIN
bool DoAssert( const char * file, int line, const char * msg );
EXTERN_C_END

// ASSERT(stmt): test stmt, break execution iff false
// @TODO -- thread safety?  GUI?  Ignore/restore?  Logging?
// what about assert() handlers? -- skip it, too annoying
//
#define ASSERT(XXX) if(!(XXX)){DoAssert( __FILE__, __LINE__, #XXX );}

// VERIFY -- in debug mode, same as ASSERT
#define VERIFY(XXX) ( (XXX) || DoAssert(__FILE__,__LINE__,#XXX) )

#else   // below if !BUILD_DEBUG

// release: omit all debug assertions
#define DEBUG_BREAK()
#define ASSERT(XXX)
// VERIFY always executes the statement.  
#define VERIFY(XXX) (XXX)
#endif


// build-independent

// compile-time assert.
// wow, gross. why the hell didn't someone just add this to the compiler 20 years ago?
// here's one that works, but with a strange error message
// @TODO look into boost for this
//
//#define STATIC_ASSERT(pred) switch(0){case 0:case pred:;}

#define CPP_CONCAT_(a, b) a##b
#define CPP_CONCAT(a, b) CPP_CONCAT_(a, b)

#define STATIC_ASSERT(e,m) \
enum { CPP_CONCAT(assert_line_, __LINE__) = 1/(!!(e)) }
