#pragma once

#include <jd/math/vec2.h>
#include <jd/base/assert.h>

namespace jd {
    
    // Quadrilaterals & friends [jd]
    // Quad defined by four vec2<T> vertices with LH-CW / RH-CCW winding
    
    
    // bilinearly interpolate a 2d position in ([0..1],[0..1]) onto a quadrilateral.
    // quadVerts are assumed to be wound 'naturally' for their coordinate system --
    // clockwise for left-handed coords, counter-clockwise for right-handed coords.
    template<typename T>
    vec2<T> QuadInterp(const vec2<T> * quadVerts, const vec2<T> & normPos)
    {
        //ASSERT( pos.x >= 0.0f && pos.y <= 1.0f );
        //ASSERT( pos.y >= 0.0f && pos.y <= 1.0f );
        vec2<T> top = Lerp(quadVerts[0], quadVerts[1], normPos.x);
        vec2<T> bottom = Lerp(quadVerts[3], quadVerts[2], normPos.x);
        return Lerp(top, bottom, normPos.y);
    }
    
    template<typename T>
    vec2<T> QuadCenter( const vec2<T> * quadVerts )
    {
        vec2<T> center(0,0);
        for( int i=0; i<4; i++ )
        {
            center += quadVerts[i];
        }
        center /= 4.0f;
        return center;
    }
    
    // return the approximate width and height of a loose fitting oriented bounding box,
    // which is probably a little smaller than a real oriented bounding box.
    template<typename T>
    vec2<T> QuadApproxDims( const vec2<T> * quadVerts )
    {
        vec2<T> approxDims;
        approxDims[0] = 0.5f * ( Length(quadVerts[0]-quadVerts[1]) + Length(quadVerts[2]-quadVerts[3]) );
        approxDims[1] = 0.5f * ( Length(quadVerts[0]-quadVerts[3]) + Length(quadVerts[1]-quadVerts[2]) );
        return approxDims;
    }
    
    // expand the edges of the quad perpendicularly outwards by an amount,
    // extending to meet the intersections of the other expanded edges.
    // WARNING -- could generate invalid quads if input & amount are too wierd.
    template<typename T>
    void QuadExpand( const vec2<T> * quadIn, float amount, vec2<T> * quadOut )
    {
        vec2<T> edgePerp[4];
        for( int i=0; i<4; i++ )
        {
            vec2<T> edgeDir = quadIn[(i+1)%4] - quadIn[i];
            edgePerp[i] = NormalizedSafe( PerpR(edgeDir) );
        }
        
        line2<T> expEdges[4];
        for( int i=0; i<4; i++ )
        {
            vec2<T> p0 = quadIn[i] + edgePerp[i] * amount;
            vec2<T> p1 = quadIn[(i+1)%4] + edgePerp[i] * amount;
            expEdges[i] = line2<T>( p0, p1 );
        }
        
        int intersectResult = 0;
        for( int i=0; i<4; i++ )
        {
            const line2<T> & a = expEdges[Mod(i-1,4)];
            const line2<T> & b = expEdges[i];
            vec2<T> p;
            intersectResult = LineIntersect( a.posA, a.posB, b.posA, b.posB, false, p );
            ASSERT( intersectResult == 1 && "lines in quad are parallel or coincident or something." );
            if( intersectResult <= 0 )
                break;
            quadOut[i] = p;
        }
        
        if ( intersectResult <= 0 )
        {
            for( int i=0; i<4; i++ )
            {
                quadOut[i] = quadIn[i];
            }
        }
    }
    
    // given point p, find penetration depth in convex poly.
    // if p is inside quad, returns a positive value indicating distance inside nearest edge.
    // NOTE returns 0 if point is outside poly (@TODO fix this)
    template<typename T>
    float QuadPenetrationDepth( const vec2<T> * polyVerts, const vec2<T> & p )
    {
        // edges[i] == vert[i]-->vert[i+1]
        vec2<T> edges[4];
        for( int i=0; i<4; i++ )
        {
            vec2<T> edge = polyVerts[(i+1)%4] - polyVerts[i];
            edges[i] = Normalized(edge);
        }
        
        float leastDepth = FLT_MAX;
        for( int i=0; i<4; i++ )
        {
            int prev_i = Mod(i-1,4);
            vec2<T> toPoint = p - polyVerts[i];
            
            // if point is outside and closest to a corner, return radial depth.
            // test if point is to the right of a rightmost previous edge normal and
            // to the left of a leftmost next edge normal.
            if( DotProduct(toPoint, edges[i]) < 0.0f &&
               DotProduct(toPoint, edges[prev_i]) > 0.0f )
            {
                float radial = Length(toPoint);
                return -radial;	// return depth, which is amount INSIDE quad
            }
            else
            {
                // it's not a corner, it's a side
                float depth = DotProduct( PerpR(edges[i]), toPoint);
                if ( depth < leastDepth )
                    leastDepth = depth;
            }
        }
        
        return leastDepth;
    }
    
    // test whether a point 'p' is inside a regular convex polygon
    // with clockwise winding in a left-handed coordinate frame (x right, y down)
    template<typename T>
    bool ConvexPolyContainsPoint( const vec2<T> * polyVerts, int numVerts, const vec2<T> & p )
    {
        // test if the point is in the negative halfspace of all segments.
        for( int i=0; i<numVerts; i++ )
        {
            vec2<T> toPoint = p - polyVerts[i];
            vec2<T> edge = polyVerts[(i+1)%numVerts] - polyVerts[i];
            if ( DotProduct( PerpL(edge), toPoint) >= 0.0f )
            {
                return false;
            }
        }
        
        return true;
    }
    
    template<typename T>
    void poly2_mulPoints( const mat3x2<T> & m, const vec2<T> * polyVerts, int numVerts, vec2<T> * outVerts )
    {
        for(int i=0; i<numVerts; i++)
        {
            outVerts[i] = mat3x2_mulPoint(m, polyVerts[i]);
        }
    }
    
    
} // jd
