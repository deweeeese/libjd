//#pragma once

//#include <jd/math/basic.h>
//#include <iostream>

//namespace jd {

//template<typename T>
//class vec4
//{
//public:
//    inline vec4() {}
//    template<typename OtherT>
//    explicit inline vec4(OtherT all) : x(all), y(all), z(all) {}
//    inline vec4( T X, T Y, T Z, T W ) : x(X), y(Y), z(Z), w(W) {}

//    static vec4 homoPoint( const vec3 & p ) {return vec4(p.x,p.y,p.z,T(1));}
//    static vec4 homoVec( const vec3 & v ) {return vec4(v.x,v.y,v.z,T(0));}

//    inline T &operator[]( int i ) { return el[i]; }
//    inline T const &operator[]( int i ) const { return el[i]; }

//    vec4 & operator+=( vec4 const & v );
//    vec4 & operator-=( vec4 const & v );

//    template<typename S>
//    vec4 & operator*=( S scalar );

//    template<typename S>
//    vec4 & operator*=( const vec4<S> & v);

//    template<typename S>
//    vec4 & operator/=( S scalar );

//    void Normalize();
//    void NormalizeSafe( T eps = std::numeric_limits<T>::epsilon() );

//    inline void SetZero(){x=0;y=0;z=0;}
//    inline void Set( T X, T Y, T Z ) { x=X; y=Y; z=Z; }

//    inline T * ptr() { return &x; }
//    inline const T * ptr() const { return &x; }

//    union {
//        struct {
//            T x, y, z, w;
//        };
//        T el[4];
//    };
//};

//typedef vec4<int> vec4i;
//typedef vec4<float> vec4f;
//typedef vec4<double> vec4d;


//template<typename T>
//std::ostream & operator << (std::ostream& out, const vec4<T> & v )
//{
//    out << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
//    return out;
//}


//// vec4 class definitions

//template<typename T>
//inline vec4<T> & vec4<T>::operator+=( vec4<T> const & v )
//{
//    x += v.x;
//    y += v.y;
//    z += v.z;
//    //w += v.w;
//    return(*this);
//}

//template<typename T>
//inline vec4<T> & vec4<T>::operator-=( vec4<T> const & v )
//{
//    x -= v.x;
//    y -= v.y;
//    z -= v.z;
//    //w -= v.w;
//    return(*this);
//}

//template<typename T>
//template<typename S>
//inline vec4<T> & vec4<T>::operator*=( S scalar )
//{
//    x *= scalar;
//    y *= scalar;
//    z *= scalar;
//    //w *= scalar;
//    return(*this);
//}

//template<typename T>
//template<typename S>
//inline vec4<T> & vec4<T>::operator*=( const vec4<S> & v)
//{
//    x *= v.x;
//    y *= v.y;
//    z *= v.z;
//    return(*this);
//}

//template<typename T>
//template<typename S>
//inline vec4<T> & vec4<T>::operator/=( S scalar )
//{
//    x /= scalar;
//    y /= scalar;
//    z /= scalar;
//    return(*this);
//}

//// euclidean norm
//template<typename T>
//inline T LengthSquared( vec4<T> const & v )
//{
//    return Sqr(v.x) + Sqr(v.y) + Sqr(v.z);
//}

//template<typename T>
//inline T Length( vec4<T> const & v )
//{
//    return sqrt( LengthSquared(v) );
//}

//template<typename T>
//inline T TaxicabLength( vec4<T> const & v )
//{
//    return abs(v.x) + abs(v.y) + abs(v.z);
//}

//template<typename T>
//inline void vec4<T>::Normalize()
//{
//    T len = Length(*this);
//    (*this)/=len;
//}

//template<typename T>
//inline void vec4<T>::NormalizeSafe( T eps )
//{
//    T len = Length(*this);
//    (*this)/= (len + eps);
//}



//// vec4 standalone operators

//template<typename T>
//inline vec4<T> operator+( vec4<T> const & a, vec4<T> const & b )
//{
//    return vec4<T>(a.x + b.x, a.y + b.y, a.z + b.z);
//}

//template<typename T>
//inline vec4<T> operator-( vec4<T> const & a, vec4<T> const & b )
//{
//    return vec4<T>(a.x - b.x, a.y - b.y, a.z - b.z);
//}

//template<typename T>
//inline vec4<T> operator-( vec4<T> const &v )
//{
//    return vec4<T>( -v.x, -v.y, -v.z );
//}

//template<typename T, typename S>
//inline vec4<T> operator*( vec4<T> const & v, S scalar )
//{
//    return vec4<T>( v.x * scalar, v.y * scalar, v.z * scalar );
//}

//template<typename T, typename S>
//inline vec4<T> operator*( S scalar, vec4<T> const & v )
//{
//    return v * scalar;
//}

//// hadamard product
//template<typename T, typename S>
//inline vec4<T> operator*( vec4<T> const & a, vec4<S> const & b )
//{
//    return vec4<T>(a.x*b.x, a.y*b.y, a.z*b.z);
//}

//template<typename T, typename S>
//inline vec4<T> operator/( vec4<T> const & v, S s )
//{
//    return vec4<T>(v.x/s, v.y/s, v.z/s);
//}

//template<typename T, typename S>
//inline vec4<T> operator/( vec4<T> const & a, vec4<S> const & b )
//{
//    return vec4<T>(a.x/b.x, a.y/b.y, a.z/b.z);
//}

//template<typename T>
//inline bool operator==( vec4<T> const & a, vec4<T> const & b)
//{
//    return ( (a.x == b.x) && (a.y == b.y) && (a.z == b.z) );
//}



//// vec4 functions

//template<typename T>
//inline vec4<T> CrossProduct( vec4<T> const &a, vec4<T> const &b )
//{
//    return vec4<T>(a.y * b.z - a.z * b.y,
//                a.z * b.x - a.x * b.z,
//                a.x * b.y - a.y * b.x);
//}

//template<typename T>
//inline T DotProduct( vec4<T> const &a, vec4<T> const &b )
//{
//    return a.x * b.x + a.y * b.y + a.z * b.z;
//}

//template<typename T>
//inline vec4<T> Lerp( vec4<T> const & a, vec4<T> const & b, T t )
//{
//    return (1-t)*a + t*b;
//}

//template<typename T>
//inline vec4<T> Normalized( vec4<T> const & v )
//{
//    vec4<T> v_norm( v );
//    v_norm.Normalize();
//    return v_norm;
//}

//template<typename T>
//inline vec4<T> Normalized( vec4<T> const & v, float & len ) {
//    len = Length(v);
//    vec4<T> v_norm = v;
//    v_norm /= len;
//    return v_norm;
//}

//template<typename T>
//inline vec4<T> NormalizedSafe( vec4<T> const & v )
//{
//    vec4<T> v_norm( v );
//    v_norm.NormalizeSafe( std::numeric_limits<T>::epsilon() );
//    return v_norm;
//}

//template<typename T>
//inline vec4<T> NormalizedSafe( vec4<T> const & v, float & len ) {
//    len = Length(v);
//    vec4<T> v_norm = v;
//    v_norm /= len + std::numeric_limits<T>::epsilon();
//    return v_norm;
//}

//template<typename T>
//inline bool IsNormalValued( const vec4<T> & v )
//{
//    return IsNormalValued( v.x ) && IsNormalValued( v.y ) && IsNormalValued( v.z );
//}





//} // namespace jd

//////////////////////////////////////////////

//template<typename T>
//inline jd::vec4<T> sqrt( const jd::vec4<T> & v )
//{
//    return jd::vec4<T>(sqrt(v.x), sqrt(v.y), sqrt(v.z));
//}

//namespace std
//{

//template <class T>
//struct less : binary_function<T, T, bool>
//{
//    bool operator()(const T& a, const T& b) const {
//        return ((a.x<b.x)? true :
//                           ((a.y<b.y)? true :
//                                       ((a.z<b.z)? true :
//                                                   false)
//                                       )
//                           );
//    }
//};


//}
