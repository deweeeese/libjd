#pragma once

#include <jd/math/basic.h>
#include <jd/math/vec3.h>

namespace jd {

template<typename T>
class quat
{
public:
    inline quat() {}
    inline quat( T X, T Y, T Z, T W ) : v(X,Y,Z), w(W) {}
    inline quat( vec3<T> const & V, T W ) : v(V), w(W) {}

    inline T &operator[]( int i ) { return ((float*)this)[i]; }
    inline T const &operator[]( int i ) const { return ((float*)this)[i]; }

    inline quat & operator+=( quat const & q );
    inline quat & operator-=( quat const & q );
    inline quat & operator*=( T scalar );
    inline quat & operator/=( T scalar );

    inline void Normalize();
    inline void NormalizeSafe( T eps = std::numeric_limits<T>::epsilon() );

    inline void SetIdentity() {v.SetZero(); w=1;}
    inline void Set( T X, T Y, T Z, T W ) { v.Set(X,Y,Z); w=W; }
    inline void Set( const vec3<T> & V, T W ) { v=V; w=W; }

    const T * ptr() const {return &x;}
    T * ptr() {return &x;}

public:
    union {
        struct{vec3<T> v;};
        struct{T x,y,z;};
    };
    T w;
};

typedef quat<float> quatf;
typedef quat<double> quatd;


template<typename T>
struct PosQuat
{
    // equivalent to 4x4 matrix
    jd::quatd rot;
    jd::vec3d pos;
};

template<typename T>
inline PosQuat<T> Invert(const PosQuat<T> & pq)
{
    PosQuat<T> inv;
    inv.rot = QuatConjugate(pq.rot);
    inv.pos = -QuatRotateVec(inv.rot, pq.pos);
    return inv;
}

typedef PosQuat<float> PosQuatf;
typedef PosQuat<double> PosQuatd;




template<typename T>
quat<T> QuatProduct( quat<T> const &a, quat<T> const &b )
{
    quat<T> q;
    q.w = a.w*b.w - DotProduct(a.v,b.v);
    q.v = a.w*b.v + b.w*a.v + CrossProduct(a.v,b.v);
    return q;
}

template<typename T>
quat<T> QuatAdd( quat<T> const &a, quat<T> const &b )
{
    if(DotProduct(a,b) < 0) {
        return a - b;
    } else {
        return a + b;
    }
}

template<typename T>
quat<T> QuatSlerp( quat<T> const &q1, quat<T> const &q2, T t )
{
    // @todo this is just not robust?
    T sign = (T)1;
    T cosang = Clamp(DotProduct(q1,q2),(T)-1,(T)1);
    if(cosang < (T)0) {
        cosang = -cosang;
        sign = -1;
    }
    T s1, s2;
    if((1 - cosang) > T(1e-4)) {
        T ang = acos(cosang);
        T invsinang = (T)1.0 / sin(ang);
        s1 = sin(((T)1.0 - t) * ang) * invsinang;
        s2 = sin(t * ang) * invsinang;
    } else {
        s1 = ((T)1.0-t);
        s2 = t;
    }
    return (sign*s1)*q1 + s2*q2;
}

template<typename T>
quat<T> QuatBetween( const vec3<T> & a, const vec3<T> & b )
{
    vec3<T> axis = CrossProduct( a, b );
    if ( LengthSquared(axis) < std::numeric_limits<T>::epsilon() ) {
        return quat<T>(0,0,0,1);
    }
    float len = sqrt( LengthSquared(a) * LengthSquared(b) ) + DotProduct(a,b);
    if ( len < std::numeric_limits<T>::epsilon() ) {
        // @todo [jd] -- wut
        return Normalized( quat<T>(-a.z,a.y,a.x,0) );
    }
    else {
        return Normalized( quat<T>( axis.x, axis.y, axis.z, len ) );
    }
}

template<typename T>
vec3<T> QuatRotateVec( const quat<T> & q, const vec3<T> & v )
{
    quat<T> qc = QuatConjugate(q);
    return QuatProduct( q, QuatProduct( quat<T>(v,0), qc ) ).v;
}

template<typename T>
quat<T> QuatFromAxisAngle( vec3<T> const &axis, T radians )
{
    T c = cos( (T)0.5 * radians );
    T s = sin( (T)0.5 * radians );
    return quat<T>( s*axis, c );
}

template<typename T>
void QuatToAxisAngle( const quat<T> & q, vec3<T> & axis, T & angle )
{
    T qSqrLen = LengthSquared(q.v);
    if ( qSqrLen > T(0) )
    {
        angle = T(2)*acos(q.w);
        // @TODO invsqrt
        T invLen = T(1) / sqrt(qSqrLen);
        axis = q.v * invLen;
    }
    else
    {
        axis.Set(T(1),T(0),T(0));
        angle = T(0);
    }
}

// quat standalone operators

template<typename T>
inline quat<T> operator+( quat<T> const & a, quat<T> const & b )
{
    return quat<T>(a.v.x + b.v.x, a.v.y + b.v.y, a.v.z + b.v.z, a.w + b.w );
}

template<typename T>
inline quat<T> operator-( quat<T> const & a, quat<T> const & b )
{
    return quat<T>(a.v.x - b.v.x, a.v.y - b.v.y, a.v.z - b.v.z, a.w - b.w );
}

template<typename T>
inline quat<T> operator*( quat<T> const & q, T scalar )
{
    return quat<T>( q.v.x * scalar, q.v.y * scalar, q.v.z * scalar, q.w * scalar );
}

template<typename T>
inline quat<T> operator*( T scalar, quat<T> const & q )
{
    return q * scalar;
}

template<typename T>
inline bool operator==( quat<T> const & a, quat<T> const & b)
{
    return ( (a.v == b.v) && (a.w == b.w) );
}

template<typename T>
inline T DotProduct( quat<T> const &a, quat<T> const &b )
{
    return a.v.x*b.v.x + a.v.y*b.v.y + a.v.z*b.v.z + a.w*b.w;
}

// euclidean norm
template<typename T>
inline T LengthSquared( quat<T> const & q )
{
    return Sqr(q.v.x) + Sqr(q.v.y) + Sqr(q.v.z) + Sqr(q.w);
}

template<typename T>
inline T Length( quat<T> const & q )
{
    return sqrt( LengthSquared(q) );
}

template<typename T>
inline quat<T> Normalized( quat<T> const & v )
{
    quat<T> v_norm( v );
    v_norm.Normalize();
    return v_norm;
}

template<typename T>
inline quat<T> NormalizedSafe( quat<T> const & v, T eps = std::numeric_limits<T>::epsilon() )
{
    quat<T> v_norm( v );
    v_norm.NormalizeSafe( eps );
    return v_norm;
}


template<typename T>
inline quat<T> QuatConjugate( quat<T> const &q )
{
    // @optimize can't we just negate w and be done with it because of the double cover?
    // or is that only valid for unit quat<T>s representing rotations?  yes, I think so
    return quat<T>(-q.v.x,-q.v.y,-q.v.z,q.w);
}

template<typename T>
inline bool IsNormalValued( const quat<T> & q )
{
    return IsNormalValued( q.v ) && IsNormalValued( q.w );
}


// quat<T> class definitions

template<typename T>
inline quat<T> & quat<T>::operator+=( quat<T> const & q )
{
    v += q.v;
    w += q.w;
    return(*this);
}

template<typename T>
inline quat<T> & quat<T>::operator-=( quat<T> const & q )
{
    v -= q.v;
    w -= q.w;
    return(*this);
}

template<typename T>
inline quat<T> & quat<T>::operator*=( T scalar )
{
    v *= scalar;
    w *= scalar;
    return(*this);
}

template<typename T>
inline quat<T> & quat<T>::operator/=( T scalar )
{
    v /= scalar;
    w /= scalar;
    return(*this);
}

template<typename T>
inline void quat<T>::Normalize()
{
    T len = Length(*this);
    (*this)/=len;
}

template<typename T>
inline void quat<T>::NormalizeSafe( T eps )
{
    T len = Length(*this);
    (*this)/= (len + eps);
}

// TODO generalize to a 3x3
template<typename T>
quat<T> QuatFromAxes(const vec3<T> & e0, const vec3<T> & e1, const vec3<T> & e2)
{
    // TODO don't copy
    const vec3<T> m[3] = {e0,e1,e2};

    quat<T> q;

    // Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
    // article "Quaternion Calculus and Fast Animation".

    T tr = m[0][0]+m[1][1]+m[2][2];
    T root;

    if ( tr > 0.0 )
    {
        // |w| > 1/2
        root = sqrt(tr + T(1));
        q.w = T(0.5)*root;
        root = T(0.5)/root;
        q.x = (m[1][2]-m[2][1])*root;
        q.y = (m[2][0]-m[0][2])*root;
        q.z = (m[0][1]-m[1][0])*root;
    }
    else
    {
        // |w| <= 1/2
        static const int s_iNext[3] = { 1, 2, 0 };
        int i = 0;
        if ( m[1][1] > m[0][0] )
            i = 1;
        if ( m[2][2] > m[i][i] )
            i = 2;
        int j = s_iNext[i];
        int k = s_iNext[j];

        root = sqrt(m[i][i]-m[j][j]-m[k][k] + T(1));
        T * apkQuat[3] = { &q.x, &q.y, &q.z };
        *apkQuat[i] = T(0.5)*root;
        root = T(0.5)/root;
        q.w = (m[j][k]-m[k][j])*root;
        *apkQuat[j] = (m[i][j]+m[j][i])*root;
        *apkQuat[k] = (m[i][k]+m[k][i])*root;
    }

    return q;
}

//template<typename T>
//quat<T> QuatToAngleAxis

} // namespace jd
