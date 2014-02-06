#pragma once

#include <jd/base/build.h>

EXTERN_C_BEGIN

// Values are in seconds.
// Using a double value gives us sub-nanosecond algebraic accuracy for values up to 10^5 seconds.
// Actual accuracy varies depending on OS and hardware.
// NOTE -- if things get hairy, change this to ratios and do more annoying math.
typedef double TimeSample;

// Call this before any calls to GetTimeSampleSeconds()
void TimeSystemInit();
void TimeSystemShutdown();
    
// Return the current system clock time in seconds
TimeSample GetTimeSampleSeconds();

EXTERN_C_END

#ifdef __cplusplus

// SimpleTimer: 
class SimpleTimer
{
public:
    SimpleTimer();
    
    // start timer and return starting sample time.
    // if timer was already started, clear first.
    TimeSample Start();
    
    // mark a split time, and return elapsed time since start,
    // or return zero if timer hasn't been started
    TimeSample Mark();

    // clear timer, set start time to zero
    void Reset();

    bool IsStarted() const {return startTime != 0.0;}
    
    TimeSample GetStartTime() const {return startTime;}
    TimeSample GetMarkTime() const {return markTime;}
    
    // samples and returns time elapsed since start time, or zero if not started
    TimeSample GetElapsedTime() const;
    
    // samples and returns time elapsed since last mark time,
    // or time since start time if no mark occured yet, or zero if not started
    TimeSample GetElapsedTimeSinceMark() const;
    
private:
    TimeSample startTime;
    TimeSample markTime;
};

#endif
