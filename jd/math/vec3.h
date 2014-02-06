#pragma once

#include <jd/math/basic.h>
#include <iostream>

namespace jd {

template<typename T>
class vec3
{
public:
    inline vec3() {}

    template<typename OtherT>
    explicit inline vec3(OtherT all) : x(all), y(all), z(all) {}

    inline vec3( T X, T Y, T Z ) : x(X), y(Y), z(Z) {}

    inline T &operator[]( int i ) { return el[i]; }
    inline T const &operator[]( int i ) const { return el[i]; }

    vec3 & operator+=( vec3 const & v );
    vec3 & operator-=( vec3 const & v );

    template<typename S>
    vec3 & operator*=( S scalar );

    template<typename S>
    vec3 & operator*=( const vec3<S> & v);

    template<typename S>
    vec3 & operator/=( S scalar );

    void Normalize();
    void NormalizeSafe( T eps = std::numeric_limits<T>::epsilon() );

    inline void SetZero(){x=0;y=0;z=0;}
    inline void Set( T X, T Y, T Z ) { x=X; y=Y; z=Z; }

    inline T * ptr() { return &x; }
    inline const T * ptr() const { return &x; }

    union {
        struct {
            T x, y, z;
        };
        T el[3];
    };
};

typedef vec3<int> vec3i;
typedef vec3<float> vec3f;
typedef vec3<double> vec3d;


template<typename T>
std::ostream & operator << (std::ostream& out, const vec3<T> & v )
{
    out << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return out;
}


// vec3 class definitions

template<typename T>
inline vec3<T> & vec3<T>::operator+=( vec3<T> const & v )
{
    x += v.x;
    y += v.y;
    z += v.z;
    return(*this);
}

template<typename T>
inline vec3<T> & vec3<T>::operator-=( vec3<T> const & v )
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return(*this);
}

template<typename T>
template<typename S>
inline vec3<T> & vec3<T>::operator*=( S scalar )
{
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return(*this);
}

template<typename T>
template<typename S>
inline vec3<T> & vec3<T>::operator*=( const vec3<S> & v)
{
    x *= v.x;
    y *= v.y;
    z *= v.z;
    return(*this);
}

template<typename T>
template<typename S>
inline vec3<T> & vec3<T>::operator/=( S scalar )
{
    x /= scalar;
    y /= scalar;
    z /= scalar;
    return(*this);
}

// euclidean norm
template<typename T>
inline T LengthSquared( vec3<T> const & v )
{
    return Sqr(v.x) + Sqr(v.y) + Sqr(v.z);
}

template<typename T>
inline T Length( vec3<T> const & v )
{
    return sqrt( LengthSquared(v) );
}

template<typename T>
inline T TaxicabLength( vec3<T> const & v )
{
    return abs(v.x) + abs(v.y) + abs(v.z);
}

template<typename T>
inline void vec3<T>::Normalize()
{
    T len = Length(*this);
    (*this)/=len;
}

template<typename T>
inline void vec3<T>::NormalizeSafe( T eps )
{
    T len = Length(*this);
    (*this)/= (len + eps);
}



// vec3 standalone operators

template<typename T>
inline vec3<T> operator+( vec3<T> const & a, vec3<T> const & b )
{
    return vec3<T>(a.x + b.x, a.y + b.y, a.z + b.z);
}

template<typename T>
inline vec3<T> operator-( vec3<T> const & a, vec3<T> const & b )
{
    return vec3<T>(a.x - b.x, a.y - b.y, a.z - b.z);
}

template<typename T>
inline vec3<T> operator-( vec3<T> const &v )
{
    return vec3<T>( -v.x, -v.y, -v.z );
}

template<typename T, typename S>
inline vec3<T> operator*( vec3<T> const & v, S scalar )
{
    return vec3<T>( v.x * scalar, v.y * scalar, v.z * scalar );
}

template<typename T, typename S>
inline vec3<T> operator*( S scalar, vec3<T> const & v )
{
    return v * scalar;
}

// hadamard product
template<typename T, typename S>
inline vec3<T> operator*( vec3<T> const & a, vec3<S> const & b )
{
    return vec3<T>(a.x*b.x, a.y*b.y, a.z*b.z);
}

template<typename T, typename S>
inline vec3<T> operator/( vec3<T> const & v, S s )
{
    return vec3<T>(v.x/s, v.y/s, v.z/s);
}

template<typename T, typename S>
inline vec3<T> operator/( vec3<T> const & a, vec3<S> const & b )
{
    return vec3<T>(a.x/b.x, a.y/b.y, a.z/b.z);
}

template<typename T>
inline bool operator==( vec3<T> const & a, vec3<T> const & b)
{
    return ( (a.x == b.x) && (a.y == b.y) && (a.z == b.z) );
}



// vec3 functions

template<typename T>
inline vec3<T> CrossProduct( vec3<T> const &a, vec3<T> const &b )
{
    return vec3<T>(a.y * b.z - a.z * b.y,
                a.z * b.x - a.x * b.z,
                a.x * b.y - a.y * b.x);
}

template<typename T>
inline T DotProduct( vec3<T> const &a, vec3<T> const &b )
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

template<typename T>
inline vec3<T> Lerp( vec3<T> const & a, vec3<T> const & b, T t )
{
    return (1-t)*a + t*b;
}

template<typename T>
inline vec3<T> Normalized( vec3<T> const & v )
{
    vec3<T> v_norm( v );
    v_norm.Normalize();
    return v_norm;
}

template<typename T>
inline vec3<T> Normalized( vec3<T> const & v, float & len ) {
    len = Length(v);
    vec3<T> v_norm = v;
    v_norm /= len;
    return v_norm;
}

template<typename T>
inline vec3<T> NormalizedSafe( vec3<T> const & v )
{
    vec3<T> v_norm( v );
    v_norm.NormalizeSafe( std::numeric_limits<T>::epsilon() );
    return v_norm;
}

template<typename T>
inline vec3<T> NormalizedSafe( vec3<T> const & v, float & len ) {
    len = Length(v);
    vec3<T> v_norm = v;
    v_norm /= len + std::numeric_limits<T>::epsilon();
    return v_norm;
}

template<typename T>
inline bool IsNormalValued( const vec3<T> & v )
{
    return IsNormalValued( v.x ) && IsNormalValued( v.y ) && IsNormalValued( v.z );
}





} // namespace jd

////////////////////////////////////////////

template<typename T>
inline jd::vec3<T> sqrt( const jd::vec3<T> & v )
{
    return jd::vec3<T>(sqrt(v.x), sqrt(v.y), sqrt(v.z));
}

namespace std
{

template <class T>
struct less : binary_function<T, T, bool>
{
    bool operator()(const T& a, const T& b) const {
        return ((a.x<b.x)? true :
                           ((a.y<b.y)? true :
                                       ((a.z<b.z)? true :
                                                   false)
                                       )
                           );
    }
};


}
