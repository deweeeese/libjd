#include "stdafx.h"
#include <jd/base/plat.h>
#include <jd/base/Timing.h>

#if TARGET_OS_IPHONE || TARGET_OS_MAC

#include <mach/mach.h>
#include <mach/mach_time.h>
#include <unistd.h>

static mach_timebase_info_data_t timebase;
static double osTimeToSecondsScale = 0.0;

EXTERN_C_BEGIN

void TimeSystemInit()
{
    mach_timebase_info(&timebase);
    osTimeToSecondsScale = ((double)timebase.numer) / ((double)timebase.denom) / 1000000000.0;
}

void TimeSystemShutdown()
{
}


TimeSample GetTimeSampleSeconds()
{
    return ((double)mach_absolute_time()) * osTimeToSecondsScale;
}

EXTERN_C_END

#elif TARGET_OS_WINDOWS

static LARGE_INTEGER qpcFreq;
static double osTimeToSecondsScale = 0.0;

void TimeSystemInit()
{
    BOOL ok = QueryPerformanceFrequency(&qpcFreq);
    ASSERT(ok);
    qpcFreq.QuadPart = qpcFreq.QuadPart;

    osTimeToSecondsScale = 1.0 / (double)qpcFreq.QuadPart;
}

void TimeSystemShutdown()
{
}


TimeSample GetTimeSampleSeconds()
{
    LARGE_INTEGER time;
    BOOL ok = QueryPerformanceCounter(&time);
    ASSERT(ok);

    return time.QuadPart * osTimeToSecondsScale;
}

#elif TARGET_OS_ANDROID

#include <unistd.h>
#include <time.h>

static double osTimeToSecondsScale = 0.0;

void TimeSystemInit()
{
    LOG( "TimeSystemInit" );
    timespec spec;
    int result = clock_getres(CLOCK_MONOTONIC,&spec);

    ASSERT( result == 0  && "WTF, clock_getres failed" );
    ASSERT( spec.tv_sec == 0 );
    LOG("  clock resolution is %d nanoseconds", spec.tv_nsec );
    
    TimeSample t = GetTimeSampleSeconds();
    LOG("  clock time at init: %g", t );
}

void TimeSystemShutdown()
{
}


TimeSample GetTimeSampleSeconds()
{
    timespec spec;
    int result = clock_gettime(CLOCK_MONOTONIC,&spec);
    ASSERT( result == 0  && "WTF, clock_getres failed" );
    return (double)spec.tv_sec + ((double)spec.tv_nsec) / 1000000000.0;
}

#endif




// SimpleTimer
SimpleTimer::SimpleTimer() : startTime(0), markTime(0) { }

TimeSample SimpleTimer::Start()
{
    startTime = GetTimeSampleSeconds();
    markTime = startTime;
    return startTime;
}

TimeSample SimpleTimer::Mark()
{
    if ( startTime != 0.0 )
    {
        markTime = GetTimeSampleSeconds();
		double total = markTime - startTime;
		startTime = markTime;
        return total;
    }
    return 0.0;
}

void SimpleTimer::Reset()
{
    startTime = 0.0;
    markTime = 0.0;
}

TimeSample SimpleTimer::GetElapsedTime() const
{
    if ( startTime == 0.0 )
    {
        // the clock wasn't started yet
        return 0.0;
    }
    return GetTimeSampleSeconds() - startTime;
}

TimeSample SimpleTimer::GetElapsedTimeSinceMark() const
{
    if ( startTime == 0.0 )
    {
        // the clock wasn't started yet
        return 0.0;
    }
    return GetTimeSampleSeconds() - markTime;
}
