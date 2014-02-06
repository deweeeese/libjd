#pragma once

#include <jd/base/plat.h>

#include <math.h>
#include <float.h>
#include <limits>
#include <stdlib.h>

namespace jd {
    
    typedef float real;
    
    const real real_eps = (real)1e-6;
    const real real_pi = (real)3.14159265358979323846;
    const real real_2pi = (real)2 * real_pi;
    
    template <typename T>
    inline T Clamp(T val, T min, T max)
    {
        if (val < min) return min;
        if (val > max) return max;
        return val;
    }
    
    template <typename T>
    inline T Min( T a, T b ) {
        return (a<b) ? a : b;
    }
    
    template <typename T>
    inline T Max( T a, T b ) {
        return (a>b) ? a : b;
    }
    
    // TODO could be more efficient, but want to avoid divide.
    // best leave this to specialization and platform?
    template <typename T>
    inline T Sign( T a ) {
        if( a == T(0) )
            return T(0);
        else if( a < T(0) ) {
            return T(-1);
        } else {
            return T(1);
        }
    }
    
    // test floating point normality
    bool IsNormalValued( real v );
    
    
    // return a string representing the floating point type of x
    const char * GetFPClassAsString( real x );
    
    //-------------------------------------
    // scalar functions
    
    template <typename T>
    inline T Sqr( T x ) { return x*x; }
    
    template <typename T>
    inline T Lerp( T a, T b, T t ) {
        return (1-t)*a + t*b;
    }
    
    template <typename T>
    inline T SmoothStep( T mn, T mx, T x ) {
        x = Clamp(x,mn,mx);
        x = (x-mn)/(mx-mn);
        T x2 = x*x;
        T x3 = x2*x;
        return 3*x2-2*x3;
    }
    
    template <typename T>
    inline T EvilRandom( T min, T max ) {
        return min + (max-min)*( rand() / (T)RAND_MAX );
    }
    
    template <typename T>
    inline T EvilRandom( T max ) {
        return max * ( rand() / (T)RAND_MAX );
    }
    
    template <typename T>
    inline T DegToRad( T degrees ) {
        return  degrees * T(M_PI) / T(180);
    }
    
    inline real RadToDeg( real radians ) {
        return radians * (real)180.0 / real_pi;
    }
    
    int gcd( int a, int b );
    int lcm(int a, int b);
    
    // math
    inline bool IsOdd(int n) { return (n & 1); }
    
    // Modulo function -- see http://en.wikipedia.org/wiki/Modulo_operation
    // The mod operator (%) of C and C++ is implementation-specific for negative parameters,
    // and probably uses truncated modulo form, which is reflected for negatives (i.e. anti-periodic).
    // This is the Euclidean Modulo, which is basically the projection of
    // the dividend(a) into the periodic domain of the divisor(b), e.g.:
    // Mod( 11, 10) = 1
    // Mod(-11, 10) = 9
    // Mod( 11,-10) = 1
    // Mod(-11,-10) = 9
    int Mod(int a, int b);
    
    const float kDegToRad = (float)M_PI / 180.0f;
    const float kRadToDeg = 180.0f / (float)M_PI;
    
#define DEG_TO_RAD(x) ( x * kDegToRad )
#define RAD_TO_DEG(x) ( x * kRadToDeg )
    
    uint32 NextPow2( uint32 v );
    
    // returns the integer logarithm base 2 of the nearest power of two less than v
    // i.e. log2i(3) == 1, log2i(4) == 2
    uint32 Log2i( uint32 v );
    
    inline real EvilRandomRange( real min, real max )
    {
        return min + (max-min)*( rand() / (real)RAND_MAX );
    }
    
    uint32 EvilRandomRange(uint32 ui_min, uint32 ui_max);
    
    uint32 EvilRandom(uint32 ui_max);
    
    inline real EvilRandom( real max ) {
        return max * ( rand() / (real)RAND_MAX );
    }
    
    // this works for positives, but rounds the wrong way for negatives because int clamps toward negative
    inline int RoundToPositiveInt( real x )
    {
        return (int)(x + (real)0.5);
    }
    
    
    bool IsNormalCardinalOrientationDegrees( int degrees );
    
    template<typename T>
    inline bool IsInRange( const T & x, const T & min, const T & max ) {
        return (x>=min && x<=max);
    }
    
} // namespace jd
