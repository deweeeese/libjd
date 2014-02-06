#pragma once

#include <jd/math/basic.h>
#include <jd/math/vec2.h>

#include <algorithm>

namespace jd {
    
    template<typename T>
    class rect2
    {
    public:
        inline rect2() {}
        inline rect2( T MinX, T MinY, T MaxX, T MaxY ) : mmin(MinX,MinY),mmax(MaxX,MaxY) {}
        inline rect2( const vec2<T> & Min, const vec2<T> & Max ) : mmin(Min), mmax(Max) {}
        
        inline const vec2<T> & Min() const { return mmin; }
        inline vec2<T> & Min() { return mmin; }
        inline const vec2<T> & Max() const { return mmax; }
        inline vec2<T> & Max() { return mmax; }
        
        inline void Set( const vec2<T> & Min, const vec2<T> & Max ) {mmin=Min; mmax=Max;}
        inline void SetZero() { mmin.SetZero(); mmax.SetZero(); }
        
        // add (dx,dy) to max, subtract (dx,dy) from min
        inline void Expand( T dx, T dy );
        
        inline bool SetByExtrudingSegmentToRight( const vec2<T> & a, const vec2<T> & b, T delta );
        
        // xi=0|1 yi=0|1   where 0 mean min, and 1 means max
        inline vec2<T> Corner( int xi, int yi ) const;
        // i=[0..3] where 0 is upper left, clockwise order
        inline vec2<T> Corner( int i ) const;

        inline vec2<T> Center() const;
        
        inline T Width() const {return mmax.x-mmin.x;}
        inline T Height() const {return mmax.y-mmin.y;}
        inline vec2<T> Size() const {return vec2<T>(Width(),Height());}
        
        vec2<T> * Ptr() { return &mmin; }
        
        void MakePositive() {
            if( mmin.x > mmax.x ) std::swap(mmin.x, mmax.x);
            if( mmin.y > mmax.y ) std::swap(mmin.y, mmax.y);
        }
        
        inline bool IsValid() const {return Width() >= T(0) && Height() >= T(0);}
        inline void Invalidate() {mmin.Set(T(1),T(1)); mmax.SetZero();}
        
        vec2<T> mmin;
        vec2<T> mmax;
    };
    
    typedef rect2<float> rect2f;
    typedef rect2<int> rect2i;
    
    
    // rect2
    template<typename T>
    inline vec2<T> rect2<T>::Corner( int xi, int yi ) const
    {
        return vec2<T>( xi ? mmax.x : mmin.x, yi ? mmax.y : mmin.y );
    }
    
    // @TODO slow
    template<typename T>
    inline vec2<T> rect2<T>::Corner( int i ) const
    {
        int x = (i==1 || i==2);
        int y = (i==2 || i==3);
        return Corner( x,y );
    }
    
    template<typename T>
    inline vec2<T> rect2<T>::Center() const
    {
        return (mmin+mmax)*0.5f;
    }
    
    template<typename T>
    inline void rect2<T>::Expand( T dx, T dy )
    {
        mmin.x -= dx;
        mmin.y -= dy;
        mmax.x += dx;
        mmax.y += dy;
        
        if ( mmin.x > mmax.x ) { mmin.x = mmax.x = (mmin.x+mmax.x)*0.5f; }
        if ( mmin.y > mmax.y ) { mmin.y = mmax.y = (mmin.y+mmax.y)*0.5f; }
    }
    
    template<typename T>
    inline bool rect2<T>::SetByExtrudingSegmentToRight( const vec2<T> & a, const vec2<T> & b, T delta )
    {
        if ( a.x == b.x ) {
            // if segment travels up, extrude right (positive x)
            real direction = (a.y < b.y) ? 1.0f : -1.0f;
            
            // sort x
            real minx = a.x;
            real maxx = a.x + direction * delta;
            if ( minx > maxx ) {
                std::swap(minx, maxx);
            }
            
            // sort y
            real miny = a.y;
            real maxy = b.y;
            if ( miny > maxy ) {
                std::swap(miny, maxy);
            }
            
            mmin.Set(minx,miny);
            mmax.Set(maxx,maxy);
        }
        else if ( a.y == b.y ) {
            // if segment travels right, extrude down (negative y)
            real direction = (a.x < b.x) ? -1.0f : 1.0f;
            
            // sort x
            real minx = a.x;
            real maxx = b.x;
            if ( minx > maxx ) {
                std::swap(minx, maxx);
            }
            
            // sort y
            real miny = a.y;
            real maxy = a.y + direction * delta;
            if ( miny > maxy ) {
                std::swap(miny, maxy);
            }
            
            mmin.Set(minx,miny);
            mmax.Set(maxx,maxy);
        }
        else {
            SetZero();
            return false;
        }
        return true;
    }
    
    
    
    template<typename T>
    inline bool operator == (const rect2<T> & a, const rect2<T> & b )
    {
        return a.Min()==b.Min() && a.Max()==b.Max();
    }

    
//    enum rect2_intersection_code {
//        kIntersection_None = 0,
//        kIntersection_Exists = 1,
//        kIntersection_MAX,
//    };
    
    template<typename T>
    struct rect2_intersection {
        bool valid;
        rect2<T> r;
        rect2_intersection() : valid(false) {}
    };
    
    template<typename T>
    inline rect2_intersection<T> intersect( const rect2<T> a, const rect2<T> b )
    {
        rect2_intersection<T> i;
        
        if( !a.IsValid() || !b.IsValid() )
        {
            return i;
        }
        
        // no intersection?
        if( a.mmin.x > b.mmax.x || a.mmin.y > b.mmax.y
           || a.mmax.x < b.mmin.x || a.mmax.y < b.mmin.y )
        {
            return i;
        }
        
        // there is an intersection
        i.r.mmin.x = Max(a.mmin.x, b.mmin.x);
        i.r.mmin.y = Max(a.mmin.y, b.mmin.y);
        i.r.mmax.x = Min(a.mmax.x, b.mmax.x);
        i.r.mmax.y = Min(a.mmax.y, b.mmax.y);
        i.valid = true;
        return i;
    }

    template<typename T>
    inline bool rect2_intersects( const rect2<T> a, const rect2<T> b )
    {
        // no intersection?
        if( a.mmin.x > b.mmax.x || a.mmin.y > b.mmax.y
           || a.mmax.x < b.mmin.x || a.mmax.y < b.mmin.y )
        {
            return false;
        }
        return true;
    }
}