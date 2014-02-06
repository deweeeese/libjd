#pragma once

#include <jd/math/basic.h>
#include <jd/math/vec2.h>


namespace jd {
    
    // mat3x2
    // layout is '3 columns of 2-vectors'.
    // storage is column-major (xyxyxy, not xxxyyy).
    // when multiplying the implied third row is (0 0 1).
    //
    //    b0  b1   t
    // x   a   c  tx
    // y   b   d  ty
    //
    // (b=basis  t=translation)
    //
    // access elements like this:
    //   mat.at[col][row]
    //   mat.ptr()[0..5]
    //
    // when accumulating matrix B onto A, multiply it on the right:
    //   mat3x2_mul( A, B );
    //
    template<typename T>
    class mat3x2
    {
    public:
        mat3x2() {}
        
        mat3x2( const mat3x2 & mat ) { copyFrom(mat); }
        mat3x2( T X0, T Y0, T X1, T Y1, T tX, T tY )
        : a(X0), b(Y0), c(X1), d(Y1), tx(tX), ty(tY) {}
        
        inline void copyFrom( const mat3x2 & mat ) {
            a = mat.a;
            b = mat.b;
            c = mat.c;
            d = mat.d;
            tx = mat.tx;
            ty = mat.ty;
        }
        
        inline T * ptr() {return &a;}
        inline const T * ptr() const {return &a;}
        
        inline T & at( int col, int row ) {return ptr()[(col<<1)+row];}
        inline const T & at( int col, int row ) const {return ptr()[(col<<1)+row];}
        
        // matrix that rotates a point counterclockwise about the origin by an angle in radians.
        static inline mat3x2 rotation( T angle_radians ) {
            mat3x2 out;
            T cos_angle = cos( angle_radians );
            T sin_angle = sin( angle_radians );
            return mat3x2(cos_angle, sin_angle,
                          -sin_angle, cos_angle,
                          T(0), T(0) );
        }
        
        static inline mat3x2 identity( ) {
            return mat3x2(1,0,0,1,0,0);
        }
        
        static inline mat3x2 translation( const vec2<T> & p ) {
            return mat3x2(1,0, 0,1, p[0], p[1]);
        }
        
        static inline mat3x2 scale( const vec2<T> & s ) {
            return mat3x2(s.x,0, 0,s.y, 0,0);
        }
        
    public:
        // i don't use a union here, because some number types
        // have non-trivial constructors (like boost multiprecision rationals).
        // you can still use ptr() or at(row,col).
        T a, b, c, d, tx, ty;
    };
    
    typedef mat3x2<float> mat3x2f;
    typedef mat3x2<float> mat3x2d;
    
    template<typename T>
    inline mat3x2<T> mat3x2_makeScale( T s ) {return mat3x2_makeScale(vec2<T>(s,s));}
    
    
    
    // multiplies two affine 2x3 matrices a*b, assuming they are embedded in a homogeneous 3x3.
    template<typename T>
    mat3x2<T> mat3x2_mul( const mat3x2<T> & a, const mat3x2<T> & b )
    {
        mat3x2<T> out;
        
        for( int y=0; y<2; y++ )
        {
            int x=0;
            for( ; x<2; x++ )
            {
                T sum(0);
                for( int k=0; k<2; k++ )    // limit 2 (assume homogeneous row[2] == [0 0 1]
                {
                    sum += a.at(k,y) * b.at(x,k);
                }
                out.at(x,y) = sum;
            }
            
            // x==2, k in [0..3)
            x=2;
            {
                T sum(0);
                for( int k=0; k<2; k++ )
                {
                    sum += a.at(k,y) * b.at(x,k);
                }
                sum += a.at(2,y);
                
                out.at(x,y) = sum;
            }
        }
        
        return out;
    }
    
    // transform a point (applies translation)
    // use this transform for positions, which need to translate between reference frames
    //
    // takes a 2-row by 3-column affine matrix, grouped by columns (basis vectors) as float[3][2]
    // and transforms point p by multiplication
    template<typename T>
    vec2<T> mat3x2_mulPoint( const mat3x2<T> & xform, const vec2<T> & p )
    {
        vec2<T> out;
        out.x = xform.a*p.x + xform.c*p.y + xform.tx;
        out.y = xform.b*p.x + xform.d*p.y + xform.ty;
        return out;
    }
    
    // transform a vector (does not apply translation)
    // use this transform for vectors, which are reference frame independent.
    //
    // takes a 2-row by 3-column affine matrix, grouped by columns (basis vectors) as float[3][2]
    // and transforms point p by multiplication of the affine basis (but not column[2], the translation)
    template<typename T>
    vec2<T> mat3x2_mulVector( const mat3x2<T> & xform, const vec2<T> & p )
    {
        vec2<T> out;
        out.x = xform.a*p.x + xform.c*p.y;
        out.y = xform.b*p.x + xform.d*p.y;
        return out;
    }

    
    template<typename T>
    mat3x2<T> mat3x2_scale( const mat3x2<T> & m, const vec2<T> & s )
    {
        mat3x2<T> out;
        mat3x2<T> m_scale = mat3x2<T>::scale(s);
        out = mat3x2_mul( m_scale, m );
        return out;
    }
    
    template<typename T>
    mat3x2<T> mat3x2_rotate( const mat3x2<T> & m, T angle_radians )
    {
        mat3x2<T> out;
        mat3x2<T> m_rot = mat3x2<T>::rotation(angle_radians);
        out = mat3x2_mul( m_rot, m );
        return out;
    }
    
    template<typename T>
    mat3x2<T> mat3x2_translate( const mat3x2<T> & m, const vec2<T> & p )
    {
        mat3x2<T> m_t = mat3x2_makeTranslation(p);
        return mat3x2_mul( m_t, m);
    }
    
    template<typename T>
    mat3x2<T> mat3x2_invert( const mat3x2<T> & m )
    {
        // assuming third row is (0 0 1)
        mat3x2<T> out;
        T det=m.a*m.d - m.c*m.b;
        ASSERT( det!=T(0) && "Singular Matrix" );
        out.a = (  m.d)/det;
        out.c = (- m.c)/det;
        out.tx = (m.c*m.ty - m.tx*m.d)/det;
        out.b = (- m.b)/det;
        out.d = (  m.a)/det;
        out.ty = (m.tx*m.b - m.a*m.ty)/det;
        return out;
    }
    
} // namespace jd