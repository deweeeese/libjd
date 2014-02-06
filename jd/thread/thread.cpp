#if TARGET_OS_MAC || TARGET_OS_IPHONE
#if !defined(__OBJC__) || !defined(__cplusplus)
#error On Mac/iOS, compile this file as a sourcecode.cpp.objcpp file.
#endif
#endif

#include "stdafx.h"
#include <jd/thread/thread.h>
//#include <boost/thread/thread.hpp>

using namespace std;
using namespace jd;

static boost::thread::id main_thread_id;

int GetHardwareThreadCount()
{
	return (int)boost::thread::hardware_concurrency();
}

bool IsMainThread()
{
    return boost::this_thread::get_id() == main_thread_id;
}

bool ThreadSystemInit()
{
#if TARGET_OS_MAC || TARGET_OS_IPHONE
	// Apple Thread Programming Guide says that if ![NSThread isMultiThreaded],
	// then creating posix threads might make Cocoa APIs unsafe.  Recommended to
	// instantiate a thread and kill it, which initiates isMultiThreaded.
	if( ![NSThread isMultiThreaded] )
	{
		LOG( "Enabling multithreading for Cocoa..." );
		NSThread * thread = [[[NSThread alloc] init] autorelease];
		[thread start];
	}
#endif
    
    main_thread_id = boost::this_thread::get_id();
    
	return true;
}

void ThreadSystemShutdown()
{
}

void ThreadLogCurrentId(){
    boost::thread::id id = boost::this_thread::get_id();
    ostringstream burf;
    burf << id;
    LOG( "Current thread id == %s", burf.str().c_str() );
}
