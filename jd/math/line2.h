#pragma once

#include <jd/math/vec2.h>

namespace jd {
    
    template<typename T>
    class line2
    {
    public:
        line2(void) {}
        
        line2(const vec2<T> & a, const vec2<T> & b)
		: posA(a), posB(b)
        {
        }
        
        inline T Length() const { return Length(posB-posA); }
        inline vec2<T> Direction() const { return NormalizedSafe(posB-posA); }
        
    public:
        vec2<T> posA, posB;
    };
    
    typedef line2<float> line2f;
    
    // LineIntersect: test line intersection between the lines represented by two segments.
    // if the lines represented by the segments are colinear, returns an arbitrary point on the line.
    // if withinSegments is true, will reject any line intersection outside the segments.
    // BUG: if the lines represented by the segments are colinear, withinSegments is ignored.
    //
    // return value:
    //  0 = lines are parallel non-colinear; nothing written out
    //  1 = single intersection inside segments, intersection point is written out
    //
    template<typename T>
    int LineIntersect(const vec2<T> & a0, const vec2<T> & a1,
                      const vec2<T> & b0, const vec2<T> & b1,
                      bool withinSegments, vec2<T> & out )
    {
        T denom  = (b1[1]-b0[1])*(a1[0]-a0[0]) - (b1[0]-b0[0])*(a1[1]-a0[1]);
        T ua_num = (b1[0]-b0[0])*(a0[1]-b0[1]) - (b1[1]-b0[1])*(a0[0]-b0[0]);
        T ub_num = (a1[0]-a0[0])*(a0[1]-b0[1]) - (a1[1]-a0[1])*(a0[0]-b0[0]);
        const T eps = std::numeric_limits<T>::epsilon();
        if( abs(ua_num) < eps && abs(ub_num) < eps && abs(denom) < eps )
        {
            out = Average(a0,a1);
            return 1;
        }
        if( fabs(denom) < real_eps )
        {
            return 0;
        }
        
        T ua = ua_num / denom;
        out = a0 + ua * (a1-a0);
        
        if( withinSegments )
        {
            //float ub = ub_num / denom;
            if(ua_num < T(0) || ua_num > T(1) ||
               ub_num < T(0) || ub_num > T(1) )
            {
                return 0;
            }
        }
        
        return 1;
    }
    
}