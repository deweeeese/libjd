#pragma once

#include <jd/math/math.h>
#include <CoreGraphics/CGGeometry.h>

#define CGAffine_from_mat3x2( m ) (*reinterpret_cast<CGAffineTransform*>(&m))

// on 64-bit systems, i think CGFloat might be double
inline jd::vec2f vec2_from_CGPoint( const CGPoint & p ) {
    return jd::vec2f( p.x, p.y );
}

