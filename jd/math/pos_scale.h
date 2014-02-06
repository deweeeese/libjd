#pragma once

#include <jd/math/basic.h>

namespace jd {
    
    template<typename T>
    class pos_scale
    {
    public:
        typedef typename jd::vec2<T> vec2_T;
        
        pos_scale() {}
        
        pos_scale( const vec2_T & _pos, const T & _scale )
        : pos(_pos), scale(_scale) {}
        
        // camera (in world units)
        vec2_T pos;
        T scale;
    };
    
    template<typename T>
    inline pos_scale<T> pos_scale_invert( const pos_scale<T> & ps )
    {
//        mat3x2<T> out;
//        T det=m.a*m.d - m.c*m.b;
//        ASSERT( det!=T(0) && "Singular Matrix" );
//        out.a = (  m.d)/det;
//        out.c = (- m.c)/det;
//        out.tx = (m.c*m.ty - m.tx*m.d)/det;
//        out.b = (- m.b)/det;
//        out.d = (  m.a)/det;
//        out.ty = (m.tx*m.b - m.a*m.ty)/det;
//        return out;
        return pos_scale<T>(-ps.pos*ps.scale, T(1)/ps.scale);
    }
    
    template<typename T>
    inline mat3x2<T> mat3x2_from_pos_scale( const pos_scale<T> & ps )
    {
        return mat3x2<T>(ps.scale,0, 0,ps.scale, ps.pos[0], ps.pos[1]);
    }
 
    template<typename T>
    inline vec2<T> pos_scale_mulPoint( const pos_scale<T> & ps, const vec2<T> & p )
    {
        return vec2<T>( ps.scale * p.x + ps.pos.x, ps.scale * p.y + ps.pos.y );
    }

    template<typename T>
    inline vec2<T> pos_scale_mulVector( const pos_scale<T> & ps, const vec2<T> & v )
    {
        return vec2<T>( ps.scale * v.x, ps.scale * v.y );
    }
}