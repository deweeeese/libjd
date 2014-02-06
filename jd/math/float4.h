#pragma once

// extremely basic quad of floats :(
namespace jd {

    struct float4 {
        float4() {}
        float4( float _a, float _b, float _c, float _d ) : a(_a), b(_b), c(_c), d(_d) {}
        float & operator[](int i) {return *(&ptr()[i]);}
        float * ptr() {return &a;}
        const float * ptr() const {return &a;}
        
        float a, b, c, d;
    };
    
    inline float4 operator * (const float4 & x, float t) {
        return float4(x.a*t,x.b*t,x.c*t,x.d*t);
    }
    
    inline float4 operator * (const float4 & x, const float4 & y) {
        return float4(x.a*y.a, x.b*y.b, x.c*y.c, x.d*y.d);
    }
    
    inline float4 operator + (const float4 & x, const float4 & y) {
        return float4(x.a+y.a, x.b+y.b, x.c+y.c, x.d+y.d);
    }
    
    inline float4 Lerp( const float4 & a, const float4 & b, real t )
    {
        return a*(1-t) + b*t;
    }
    
}