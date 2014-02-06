#pragma once

#include <jd/math/basic.h>

namespace jd {
    
    //-------------------------------------
    // 2d math
    
    template<typename T>
    class vec2
    {
    public:
        inline vec2() {}
        inline vec2( T v ) :x(v), y(v) {}
        //inline explicit vec2( const T * array ) {Set(array[0],array[1]);}
        inline vec2( T X, T Y ) { Set(X,Y); }

        inline T &operator[]( int i ) { return ptr()[i]; }
        inline T const &operator[]( int i ) const { return ptr()[i]; }
        
        inline vec2 & operator+=( vec2 const & v );
        inline vec2 & operator-=( vec2 const & v );
        inline vec2 & operator*=( T scalar );
        inline vec2 & operator/=( T scalar );
        
        inline void Normalize();
        inline void NormalizeSafe( T eps = std::numeric_limits<T>::epsilon() );
        
        inline void SetZero(){x=0; y=0;}
        inline void Set( T X, T Y ) { x=X; y=Y; }
        
        inline void CopyTo( float * dest );
        
        inline T * ptr() { return &x; }
        inline const T * ptr() const { return &x; }
        
        T x, y;
    };
    
    typedef vec2<int> vec2i;
    typedef vec2<float> vec2f;
    typedef vec2<double> vec2d;
    
    template<typename T>
    inline vec2<T> operator==( vec2<T> const & a, vec2<T> const & b ) {
        return a.x==b.x && a.y==b.y;
    }

    template<typename T>
    inline vec2<T> operator!=( vec2<T> const & a, vec2<T> const & b ) {
        return a.x!=b.x || a.y!=b.y;
    }

    // standalone operators
    template<typename T>
    inline vec2<T> operator+( vec2<T> const & a, vec2<T> const & b )
    {
        return vec2<T>(a.x + b.x, a.y + b.y);
    }
    
    template<typename T>
    inline vec2<T> operator-( vec2<T> const & a, vec2<T> const & b )
    {
        return vec2<T>(a.x - b.x, a.y - b.y);
    }
    
    template<typename T>
    inline vec2<T> operator-( vec2<T> const &v )
    {
        return vec2<T>( -v.x, -v.y );
    }
    
    template<typename T>
    inline vec2<T> operator*( vec2<T> const & v, T scalar )
    {
        return vec2<T>( v.x * scalar, v.y * scalar );
    }
    
    template<typename T>
    inline vec2<T> operator*( T scalar, vec2<T> const & v )
    {
        return v * scalar;
    }
    
    template<typename T>
    inline vec2<T> operator/( vec2<T> const & v, T scalar )
    {
        return v * (T(1)/scalar);
    }
    
    template<typename T>
    inline bool operator==( vec2<T> const & a, vec2<T> const & b)
    {
        return ( (a.x == b.x) && (a.y == b.y) );
    }

    
    // functions
    template<typename T>
    inline T PerpDotProduct( vec2<T> const &a, vec2<T> const &b )
    {
        return a.x * b.y - a.y * b.x;
    }
    
    template<typename T>
    inline T DotProduct( vec2<T> const &a, vec2<T> const &b )
    {
        return a.x * b.x + a.y * b.y;
    }
    
    template<typename T>
    inline vec2<T> Lerp( vec2<T> const & a, vec2<T> const & b, T t )
    {
        return (1-t)*a + t*b;
    }
    
    template<typename T>
    inline T LengthSquared( vec2<T> const & v )
    {
        return DotProduct(v,v);
    }
    
    template<typename T>
    inline T Length( vec2<T> const & v )
    {
        return sqrt( LengthSquared(v) );
    }
    
    template<typename T>
    inline T TaxicabLength( vec2<T> const & v )
    {
        return abs(v.x) + abs(v.y);
    }
    
    template<typename T>
    inline T Distance( const vec2<T> & a, const vec2<T> & b )
    {
        return sqrt( Sqr(a.x-b.x) + Sqr(a.y-b.y) );
    }
    
    template<typename T>
    inline vec2<T> Normalized( vec2<T> const & v )
    {
        vec2<T> v_norm( v );
        v_norm.Normalize();
        return v_norm;
    }
    
    template<typename T>
    inline vec2<T> Normalized( vec2<T> const & v, float & len ) {
        len = Length(v);
        vec2<T> v_norm = v;
        v_norm /= len;
        return v_norm;
    }
    
    template<typename T>
    inline vec2<T> NormalizedSafe( vec2<T> const & v )
    {
        vec2<T> v_norm( v );
        v_norm.NormalizeSafe( std::numeric_limits<T>::epsilon() );
        return v_norm;
    }
    
    template<typename T>
    inline vec2<T> NormalizedSafe( vec2<T> const & v, T & len ) {
        len = Length(v);
        vec2<T> v_norm = v;
        v_norm /= len + std::numeric_limits<T>::epsilon();
        return v_norm;
    }
    
    template<typename T>
    inline bool IsNormalValued( const vec2<T> & v )
    {
        return IsNormalValued( v.x ) && IsNormalValued( v.y );
    }
    
    template<typename T>
    inline void vec2<T>::CopyTo( float * dest )
    {
        dest[0] = x;
        dest[1] = y;
    }
    
    
    // class definitions
    
    template<typename T>
    inline vec2<T> & vec2<T>::operator+=( vec2<T> const & v )
    {
        x += v.x;
        y += v.y;
        return(*this);
    }
    
    template<typename T>
    inline vec2<T> & vec2<T>::operator-=( vec2<T> const & v )
    {
        x -= v.x;
        x -= v.x;
        return(*this);
    }
    
    template<typename T>
    inline vec2<T> & vec2<T>::operator*=( T scalar )
    {
        x *= scalar;
        y *= scalar;
        return(*this);
    }
    
    template<typename T>
    inline vec2<T> & vec2<T>::operator/=( T scalar )
    {
        x /= scalar;
        y /= scalar;
        return(*this);
    }
    
    template<typename T>
    inline void vec2<T>::Normalize()
    {
        T len = Length(*this);
        (*this)/=len;
    }
    
    template<typename T>
    inline void vec2<T>::NormalizeSafe( T eps )
    {
        T len = Length(*this);
        (*this)/= (len + eps);
    }
    
    // This means either right-perpendicular for a left-handed coordinate frame, or
    // left-perpendicular for a right-handed coordinate frame
    template<typename T>
    inline jd::vec2<T> PerpR( vec2<T> const & v )
	{ return jd::vec2<T>(-v.y, v.x); }
    
	// This means either left-perpendicular for a left-handed coordinate frame, or
    // right-perpendicular for a right-handed coordinate frame
    template<typename T>
	inline jd::vec2<T> PerpL( vec2<T> const & v )
	{ return jd::vec2<T>(v.y, -v.x); }
    
    template<typename T>
    inline vec2<T> Average( const vec2<T> a, const vec2<T> b ) {
        return T(0.5) * (a + b);
    }
    
} // namespace jd
