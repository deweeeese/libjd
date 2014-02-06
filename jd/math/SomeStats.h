#pragma once

#include <jd/math/basic.h>
#include <vector>
#include <limits>

namespace jd {

// SomeStats -- basic statistics like mean, median, stddev gathered from an array of numbers
// Use CalcSomeStats, pass in a forward iterator, the number of elements to process, and a data accessor predicate.
// (the predicate simply returns the value you are interested in, given an iterator)

/*
 * EXAMPLE:
 *
 struct float2_x {
    float2_x() {}
    inline float operator()( float2 f ) const { return f.x; }
 };
 
 struct float2_y {
    float2_y() {}
    inline float operator()( float2 f ) const { return f.y; }
 };
 
 // given a vector of positions, find the 2D median vector
 vector<float2> positions = ...
 SomeStats sx = CalcSomeStats( positions.begin(), positions.size(), memAlloc, float2_x() );
 SomeStats sy = CalcSomeStats( positions.begin(), positions.size(), memAlloc, float2_y() );
 float2 positions_med( sx.med, sy.med );
 
 */

template<typename StatsT>
struct SomeStats
{
    SomeStats() {
        n = 0;
        min = max = mean = stddev = med = lq = uq = StatsT(0);
    }
    
    bool IsValid() const {return n>0;}
    
    // data
    int n;
    StatsT min, max;
    StatsT mean;
    StatsT stddev;
    
    StatsT med;
    StatsT lq, uq;   // upper & lower quartiles, just like median (50%) but at 25% and 75%
};

// IterT is a forward iterator type.
// PredT retrieves the value of interest from each datum; it's a functor with float operator()( const IterT & i )
template<typename StatsT, typename IterT, typename PredT>
SomeStats<StatsT> CalcSomeStats( const IterT & begin, int count, PredT pred )
{
    SomeStats<StatsT> s;
    if( count <= 0 ) {
        return s;
    }
    
    std::vector<StatsT> sorted(count);
    
    int i = 0;
    for( IterT iter=begin; i < count; ++iter, ++i )
    {
        StatsT x = (StatsT)pred(iter);
        sorted[i] = x;
        s.mean += x;
    }
    s.n = count;
    StatsT sample_inv = StatsT(1) / StatsT(s.n);
    s.mean *= sample_inv;
    
    // lq, median, uq
    sort( sorted.begin(), sorted.end(), std::less<StatsT>() );
    
    // median
    const int half = s.n >> 1;
    if( s.n % 2 ) /*  odd */ { s.med = sorted[half]; }
    else          /* even */ { s.med = StatsT(0.5) * (sorted[half] + sorted[half-1]); }
    // lq  @TODO this should be a float with a lerp
    const int lq = s.n >> 2;
    s.lq = sorted[lq];
    // uq  @TODO this should be a float with a lerp
    const int uq = (s.n*3)>>2;
    s.uq = sorted[uq];
    
    // min max
    s.min = sorted[0];
    s.max = sorted[s.n-1];
    
    // mean & stddev -- use the 'population' measure (n), not 'sample' (n-1)
    StatsT residuals = StatsT(0);
    for(int i=0; i<s.n; i++)
    {
        residuals += Sqr(sorted[i] - s.mean);
    }
    s.stddev = sqrt( residuals * sample_inv );
    
    return s;
}


}
