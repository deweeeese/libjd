#pragma once

#include <jd/math/basic.h>
#include <jd/math/vec3.h>
#include <jd/math/quat.h>
#include <jd/base/assert.h>

namespace jd {

// This matrix should be assumed affine
// Columns indexed 0,1,2 are the frame, column index 3 is the translation.
// Column vectors multiply on the right side of a matrix.
// To compose matrix transformations, consider a column vector in a local coordinate space on the right.
// The matrix closest to the vector's left will promote it to the more global space,
// and so a matrix that promotes further should compose on the LEFT of more local matrix.

template<typename T>
class mat4x4 {
public:
    mat4x4(){ el[15] = 1; }
    mat4x4( const float * elements_16 ) { for(int i=0; i<16; i++){ el[i]=elements_16[i]; } }

    void SetIdentity() { for(int i=0; i<16; i++) { el[i] = (i%5==0) ? 1.0f : 0.0f; } }
    inline void SetFrameCol( int col, const vec3<T> & col_vec ) {
        cols[col][0] = col_vec[0];
        cols[col][1] = col_vec[1];
        cols[col][2] = col_vec[2];
        cols[col][3] = 0;
    }
    inline void SetFrameCol( int col, T x, T y, T z ) {
        cols[col][0] = x;
        cols[col][1] = y;
        cols[col][2] = z;
        cols[col][3] = 0;
    }
    void SetFrame( const vec3<T> & x, const vec3<T> & y, const vec3<T> & z ) {
        SetFrameCol( 0, x );
        SetFrameCol( 1, y );
        SetFrameCol( 2, z );
    }
    inline void SetTranslation( const vec3<T> & translation ) {
        cols[3][0] = translation[0];
        cols[3][1] = translation[1];
        cols[3][2] = translation[2];
        cols[3][3] = 1;
    }
    inline vec3<T> GetTranslation() { return GetCol(3); }
    inline vec3<T> GetCol( int col ) { return vec3<T>( cols[col][0], cols[col][1], cols[col][2] ); }

    void InvertOrthoNormalTransform();

    const T * ptr() const {return &el[0];}

public:
    union {
        T cols[4][4];
        T el[16];
    };
};

typedef mat4x4<float> mat4x4f;
typedef mat4x4<double> mat4x4d;

// decls

template<typename T>
void mat_mul_restrict( mat4x4<T> & out, const mat4x4<T> & a, const mat4x4<T> & b );

template<typename T>
void mat_mul( mat4x4<T> & out, const mat4x4<T> & a, const mat4x4<T> & b );

template<typename T>
vec3<T> mat_mulPoint( const mat4x4<T> & M, const vec3<T> & v );

template<typename T>
vec3<T> mat_mulVec( const mat4x4<T> & M, const vec3<T> & v );

template<typename T>
void mat_fromPosRot( mat4x4<T> & out, const vec3<T> & pos, const quat<T> & q );

// todo: assumes y forward, x right, z up
template<typename T>
void mat_lookAt( mat4x4<T> & out, const vec3<T> & origin, const vec3<T> & direction, const vec3<T> & up );

template<typename T>
void mat_perspective( mat4x4<T> & out, T fovyInDegrees, T aspectRatio, T znear, T zfar);

template<typename T>
void mat_frustum( mat4x4<T> & out, T left, T right, T bottom, T top, T znear, T zfar);


// defs

template<typename T>
void mat4x4<T>::InvertOrthoNormalTransform()
{
    // TODO: assert orthonormal

    // transpose the rotation part
    std::swap<T>( cols[1][0], cols[0][1] );
    std::swap<T>( cols[2][0], cols[0][2] );
    std::swap<T>( cols[2][1], cols[1][2] );

    // rotate the translation by the (inverted) rotation
    vec3<T> new_translation = -mat_mulVec( *this, GetTranslation() );
    SetTranslation( new_translation );
}

// Code taken and adapted from nVidia's nv_algebra: det2x2, det3x3, invert, multiply
// -----
// Calculate the determinant of a 2x2 matrix
template<typename T>
inline T det2x2(T a1, T a2, T b1, T b2)
{
    return a1 * b2 - b1 * a2;
}

// Calculate the determinent of a 3x3 matrix
template<typename T>
inline T det3x3(T a1, T a2, T a3, T b1, T b2, T b3, T c1, T c2, T c3)
{
    return a1 * det2x2(b2, b3, c2, c3) - b1 * det2x2(a2, a3, c2, c3) + c1 * det2x2(a2, a3, b2, b3);
}

template<typename T>
void mat_invert( mat4x4<T> & out, const mat4x4<T> & in )
{
    out.cols[0][0] =  det3x3(in.cols[1][1], in.cols[1][2], in.cols[1][3], in.cols[2][1], in.cols[2][2], in.cols[2][3], in.cols[3][1], in.cols[3][2], in.cols[3][3]);
    out.cols[0][1] = -det3x3(in.cols[0][1], in.cols[0][2], in.cols[0][3], in.cols[2][1], in.cols[2][2], in.cols[2][3], in.cols[3][1], in.cols[3][2], in.cols[3][3]);
    out.cols[0][2] =  det3x3(in.cols[0][1], in.cols[0][2], in.cols[0][3], in.cols[1][1], in.cols[1][2], in.cols[1][3], in.cols[3][1], in.cols[3][2], in.cols[3][3]);
    out.cols[0][3] = -det3x3(in.cols[0][1], in.cols[0][2], in.cols[0][3], in.cols[1][1], in.cols[1][2], in.cols[1][3], in.cols[2][1], in.cols[2][2], in.cols[2][3]);

    out.cols[1][0] = -det3x3(in.cols[1][0], in.cols[1][2], in.cols[1][3], in.cols[2][0], in.cols[2][2], in.cols[2][3], in.cols[3][0], in.cols[3][2], in.cols[3][3]);
    out.cols[1][1] =  det3x3(in.cols[0][0], in.cols[0][2], in.cols[0][3], in.cols[2][0], in.cols[2][2], in.cols[2][3], in.cols[3][0], in.cols[3][2], in.cols[3][3]);
    out.cols[1][2] = -det3x3(in.cols[0][0], in.cols[0][2], in.cols[0][3], in.cols[1][0], in.cols[1][2], in.cols[1][3], in.cols[3][0], in.cols[3][2], in.cols[3][3]);
    out.cols[1][3] =  det3x3(in.cols[0][0], in.cols[0][2], in.cols[0][3], in.cols[1][0], in.cols[1][2], in.cols[1][3], in.cols[2][0], in.cols[2][2], in.cols[2][3]);

    out.cols[2][0] =  det3x3(in.cols[1][0], in.cols[1][1], in.cols[1][3], in.cols[2][0], in.cols[2][1], in.cols[2][3], in.cols[3][0], in.cols[3][1], in.cols[3][3]);
    out.cols[2][1] = -det3x3(in.cols[0][0], in.cols[0][1], in.cols[0][3], in.cols[2][0], in.cols[2][1], in.cols[2][3], in.cols[3][0], in.cols[3][1], in.cols[3][3]);
    out.cols[2][2] =  det3x3(in.cols[0][0], in.cols[0][1], in.cols[0][3], in.cols[1][0], in.cols[1][1], in.cols[1][3], in.cols[3][0], in.cols[3][1], in.cols[3][3]);
    out.cols[2][3] = -det3x3(in.cols[0][0], in.cols[0][1], in.cols[0][3], in.cols[1][0], in.cols[1][1], in.cols[1][3], in.cols[2][0], in.cols[2][1], in.cols[2][3]);

    out.cols[3][0] = -det3x3(in.cols[1][0], in.cols[1][1], in.cols[1][2], in.cols[2][0], in.cols[2][1], in.cols[2][2], in.cols[3][0], in.cols[3][1], in.cols[3][2]);
    out.cols[3][1] =  det3x3(in.cols[0][0], in.cols[0][1], in.cols[0][2], in.cols[2][0], in.cols[2][1], in.cols[2][2], in.cols[3][0], in.cols[3][1], in.cols[3][2]);
    out.cols[3][2] = -det3x3(in.cols[0][0], in.cols[0][1], in.cols[0][2], in.cols[1][0], in.cols[1][1], in.cols[1][2], in.cols[3][0], in.cols[3][1], in.cols[3][2]);
    out.cols[3][3] =  det3x3(in.cols[0][0], in.cols[0][1], in.cols[0][2], in.cols[1][0], in.cols[1][1], in.cols[1][2], in.cols[2][0], in.cols[2][1], in.cols[2][2]);

    T det = (in.cols[0][0] * out.cols[0][0]) + (in.cols[1][0] * out.cols[0][1]) + (in.cols[2][0] * out.cols[0][2]) + (in.cols[3][0] * out.cols[0][3]);

    // mash away any degeneracy
    T epsilon = std::numeric_limits<T>::epsilon();
    if (det + epsilon >= T(0) && det - epsilon <= T(0))
        det = jd::Sign(det) * T(0.000001);
    T det_inv = T(1) / det;

    out.cols[0][0] *= det_inv;
    out.cols[0][1] *= det_inv;
    out.cols[0][2] *= det_inv;
    out.cols[0][3] *= det_inv;

    out.cols[1][0] *= det_inv;
    out.cols[1][1] *= det_inv;
    out.cols[1][2] *= det_inv;
    out.cols[1][3] *= det_inv;

    out.cols[2][0] *= det_inv;
    out.cols[2][1] *= det_inv;
    out.cols[2][2] *= det_inv;
    out.cols[2][3] *= det_inv;

    out.cols[3][0] *= det_inv;
    out.cols[3][1] *= det_inv;
    out.cols[3][2] *= det_inv;
    out.cols[3][3] *= det_inv;
}

template<typename T>
void mat_mul( mat4x4<T> & out, const mat4x4<T> & a, const mat4x4<T> & b )
{
    mat4x4<T> c;
    mat_mul_restrict(c,a,b);
    out = c;
}

template<typename T>
void mat_mul_restrict( mat4x4<T> & c, const mat4x4<T> & a, const mat4x4<T> & b )
{
    ASSERT( &c != &a && &c != &b );
    c.el[0]=a.el[0]*b.el[0]+
        a.el[4]*b.el[1]+
        a.el[8]*b.el[2]+
        a.el[12]*b.el[3];
    c.el[4]=a.el[0]*b.el[4]+
        a.el[4]*b.el[5]+
        a.el[8]*b.el[6]+
        a.el[12]*b.el[7];
    c.el[8]=a.el[0]*b.el[8]+
        a.el[4]*b.el[9]+
        a.el[8]*b.el[10]+
        a.el[12]*b.el[11];
    c.el[12]=a.el[0]*b.el[12]+
        a.el[4]*b.el[13]+
        a.el[8]*b.el[14]+
        a.el[12]*b.el[15];

    c.el[1]=a.el[1]*b.el[0]+
        a.el[5]*b.el[1]+
        a.el[9]*b.el[2]+
        a.el[13]*b.el[3];
    c.el[5]=a.el[1]*b.el[4]+
        a.el[5]*b.el[5]+
        a.el[9]*b.el[6]+
        a.el[13]*b.el[7];
    c.el[9]=a.el[1]*b.el[8]+
        a.el[5]*b.el[9]+
        a.el[9]*b.el[10]+
        a.el[13]*b.el[11];
    c.el[13]=a.el[1]*b.el[12]+
        a.el[5]*b.el[13]+
        a.el[9]*b.el[14]+
        a.el[13]*b.el[15];

    c.el[2]=a.el[2]*b.el[0]+
        a.el[6]*b.el[1]+
        a.el[10]*b.el[2]+
        a.el[14]*b.el[3];
    c.el[6]=a.el[2]*b.el[4]+
        a.el[6]*b.el[5]+
        a.el[10]*b.el[6]+
        a.el[14]*b.el[7];
    c.el[10]=a.el[2]*b.el[8]+
        a.el[6]*b.el[9]+
        a.el[10]*b.el[10]+
        a.el[14]*b.el[11];
    c.el[14]=a.el[2]*b.el[12]+
        a.el[6]*b.el[13]+
        a.el[10]*b.el[14]+
        a.el[14]*b.el[15];

    c.el[3]=a.el[3]*b.el[0]+
        a.el[7]*b.el[1]+
        a.el[11]*b.el[2]+
        a.el[15]*b.el[3];
    c.el[7]=a.el[3]*b.el[4]+
        a.el[7]*b.el[5]+
        a.el[11]*b.el[6]+
        a.el[15]*b.el[7];
    c.el[11]=a.el[3]*b.el[8]+
        a.el[7]*b.el[9]+
        a.el[11]*b.el[10]+
        a.el[15]*b.el[11];
    c.el[15]=a.el[3]*b.el[12]+
        a.el[7]*b.el[13]+
        a.el[11]*b.el[14]+
        a.el[15]*b.el[15];
}


template<typename T>
vec3<T> mat_mulPoint( const mat4x4<T> & M, const vec3<T> & v )
{
    vec3<T> out;
    out[0] = M.el[0]*v[0]+
            M.el[4]*v[1]+
            M.el[8]*v[2]+
            M.el[12];
    out[1] = M.el[1]*v[0]+
            M.el[5]*v[1]+
            M.el[9]*v[2]+
            M.el[13];
    out[2] = M.el[2]*v[0]+
            M.el[6]*v[1]+
            M.el[10]*v[2]+
            M.el[14];
    return out;
}

template<typename T>
vec3<T> mat_mulPoint( const mat4x4<T> & M, const vec3<T> & v, T * outW )
{
    vec3<T> out;
    out[0] = M.el[0]*v[0]+
            M.el[4]*v[1]+
            M.el[8]*v[2]+
            M.el[12];
    out[1] = M.el[1]*v[0]+
            M.el[5]*v[1]+
            M.el[9]*v[2]+
            M.el[13];
    out[2] = M.el[2]*v[0]+
            M.el[6]*v[1]+
            M.el[10]*v[2]+
            M.el[14];
    if( outW ) {
        *outW = M.el[3]*v[0]+
                M.el[7]*v[1]+
                M.el[12]*v[2]+
                M.el[15];
    }
    return out;
}

template<typename T>
vec3<T> mat_mulVec( const mat4x4<T> & M, const vec3<T> & v )
{
    vec3<T> out;
    out[0] = M.el[0]*v[0]+
            M.el[4]*v[1]+
            M.el[8]*v[2];
    out[1] = M.el[1]*v[0]+
            M.el[5]*v[1]+
            M.el[9]*v[2];
    out[2] = M.el[2]*v[0]+
            M.el[6]*v[1]+
            M.el[10]*v[2];
    return out;
}


template<typename T>
void mat_fromPosRot( mat4x4<T> & out, const vec3<T> & pos, const quat<T> & q )
{
    T x2 = Sqr(q.v.x);
    T y2 = Sqr(q.v.y);
    T z2 = Sqr(q.v.z);
    T xy = q.v.x * q.v.y;
    T xz = q.v.x * q.v.z;
    T yz = q.v.y * q.v.z;
    T wx = q.w * q.v.x;
    T wy = q.w * q.v.y;
    T wz = q.w * q.v.z;

    out.SetFrameCol( 0, 1.0 - 2.0*(y2 + z2), 2.0*(xy + wz), 2.0*(xz - wy) );
    out.SetFrameCol( 1, 2.0*(xy - wz), 1.0 - 2.0*(x2 + z2), 2.0*(yz + wx) );
    out.SetFrameCol( 2, 2.0*(xz + wy), 2.0*(yz - wx), 1.0 - 2.0*(x2 + y2) );
    out.SetTranslation( pos );
}

// todo: assumes y forward, x right, z up
template<typename T>
void mat_lookAt( mat4x4<T> & out, const vec3<T> & origin, const vec3<T> & direction, const vec3<T> & up )
{
    vec3<T> right = CrossProduct( direction, up );
    right.Normalize();
    vec3<T> T_up = CrossProduct( right, direction );
    out.SetFrame( right, direction, T_up );
    out.SetTranslation( origin );
}

// from man 3 glOrtho:
//   GLdouble left,
//   GLdouble right,
//   GLdouble bottom,
//   GLdouble top,
//   GLdouble zNear,
//   GLdouble zFar
//
// glOrtho describes a transformation that produces a parallel projection.
// The current matrix (see glMatrixMode) is multiplied by this matrix  and
// the  result replaces the current matrix, as if glMultMatrix were called
// with the following matrix as its argument:
//
//        2
//  ------------       0              0              tx
//  right - left
//
//                     2
//      0         ------------        0              ty
//                top - bottom
//
//                                    -2
//      0              0         ------------        tz
//                                zFar-zNear
//
//      0              0              0              1
//
// where
//
//                 tx = - (right + left) / (right - left)
//                 ty = - (top + bottom) / (top - bottom)
//                 tz = - (zFar + zNear) / (zFar - zNear)
//
// Typically,  the  matrix  mode  is  GL_PROJECTION,  and  (left,  bottom,
// -zNear)  and (right, top,  -zNear) specify the points on the near clip-
// ping plane that are mapped to the lower left and upper right corners of
// the  window,  respectively,  assuming that the eye is located at (0, 0,
// 0).  -zFar specifies the location of  the  far  clipping  plane.   Both
// zNear and zFar can be either positive or negative.
//
template<typename T>
void mat_ortho( mat4x4<T> & out, T left, T right, T bottom, T top, T zNear, T zFar )
{
    out.el[0] = T(2)/(right-left);
    out.el[1] = T(0);
    out.el[2] = T(0);
    out.el[3] = T(0);

    out.el[4] = T(0);
    out.el[5] = T(2)/(top-bottom);
    out.el[6] = T(0);
    out.el[7] = T(0);

    out.el[8] = T(0);
    out.el[9] = T(0);
    out.el[10] = T(-2)/(zFar-zNear);
    out.el[11] = T(0);

    out.el[12] = -(right + left) / (right - left);
    out.el[13] = -(top + bottom) / (top - bottom);
    out.el[14] = -(zFar + zNear) / (zFar - zNear);
    out.el[15] = T(1);
}


template<typename T>
void mat_perspective( mat4x4<T> & out, T fovyInDegrees, T aspectRatio, T znear, T zfar)
{
    T ymax, xmax;
    ymax = znear*tanf(fovyInDegrees*M_PI/360.0);
    //ymin=-ymax;
    //xmin=-ymax*aspectRatio;
    xmax=ymax*aspectRatio;
    mat_frustum<T>(out, -xmax, xmax, -ymax, ymax, znear, zfar);
}

template<typename T>
void mat_frustum( mat4x4<T> & out, T left, T right, T bottom, T top, T znear, T zfar)
{
    T temp=T(2)*znear;
    T temp2=right-left;
    T temp3=top-bottom;
    T temp4=zfar-znear;
    out.el[0] = temp / temp2;
    out.el[1] = T(0);
    out.el[0] = temp/temp2;
    out.el[1] = T(0);
    out.el[2] = T(0);
    out.el[3] = T(0);
    out.el[4] = T(0);
    out.el[5] = temp/temp3;
    out.el[6] = T(0);
    out.el[7] = T(0);
    out.el[8] = (right+left)/temp2;
    out.el[9] = (top+bottom)/temp3;
    out.el[10] = (-zfar-znear)/temp4;
    out.el[11] = T(-1);
    out.el[12] = T(0);
    out.el[13] = T(0);
    out.el[14] = (-temp*zfar)/temp4;
    out.el[15] = T(0);
}

template<typename T>
void mat_viewport( mat4x4<T> & out, T left, T bottom, T width, T height )
{
    T half_w = width*T(0.5);
    T half_h = height*T(0.5);

    out.cols[0][0] = half_w;
    out.cols[0][1] = T(0);
    out.cols[0][2] = T(0);
    out.cols[0][3] = T(0);

    out.cols[1][0] = T(0);
    out.cols[1][1] = half_h;
    out.cols[1][2] = T(0);
    out.cols[1][3] = T(0);

    out.cols[2][0] = T(0);
    out.cols[2][1] = T(0);
    out.cols[2][2] = T(1);
    out.cols[2][3] = T(0);

    out.cols[3][0] = half_w + left;
    out.cols[3][1] = half_h + bottom;
    out.cols[3][2] = T(0);
    out.cols[3][3] = T(1);
}

}
