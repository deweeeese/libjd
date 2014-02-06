
#include <jd/math/basic.h>
#include <jd/base/assert.h>
using namespace jd;

#if TARGET_OS_WINDOWS
#include <float.h>

namespace jd {
    
bool IsNormalValued( real v ) {
    int c = _fpclass(v);
    return (c == _FPCLASS_NN) || (c == _FPCLASS_NZ) || (c == _FPCLASS_PN) || (c == _FPCLASS_PZ);
}

const char * GetFPClassAsStrings(real x)
{
    int fpclass = _fpclass(x);
    switch (fpclass)
    {
        case _FPCLASS_SNAN: return "Signaling NaN";                break;
        case _FPCLASS_QNAN: return "Quiet NaN";                    break; 
        case _FPCLASS_NINF: return "Negative infinity (-INF)";     break; 
        case _FPCLASS_NN:   return "Negative normalized non-zero"; break;
        case _FPCLASS_ND:   return "Negative denormalized";        break; 
        case _FPCLASS_NZ:   return "Negative zero (-0)";           break; 
        case _FPCLASS_PZ:   return "Positive 0 (+0)";              break; 
        case _FPCLASS_PD:   return "Positive denormalized";        break; 
        case _FPCLASS_PN:   return "Positive normalized non-zero"; break; 
        case _FPCLASS_PINF: return "Positive infinity (+INF)";     break;
        default:            return 
    }
}
    
}  // namespace jd

#else // TARGET_OS_WINDOWS
    
#include <math.h>

namespace jd {
    
bool IsNormalValued( real v ) {
    int c = fpclassify(v);
    return (c == FP_NORMAL) || (c == FP_ZERO);
}

const char * GetFPClassAsStrings(real x)
{
    int fpclass = fpclassify(x);
    switch (fpclass)
    {
        case FP_NAN: return "NaN";                break;
        case FP_INFINITE: return "infinite";     break; 
        case FP_SUBNORMAL:   return "denormalized"; break;
        case FP_NORMAL:   return "normal"; break; 
        case FP_ZERO:   return "zero"; break;
        default:            return  "(unknown)"; break;
    }
}

} // namespace jd

#endif

namespace jd {

    int gcd( int a, int b )
    {
        for (;;)
        {
            if (a == 0) return b;
            b %= a;
            if (b == 0) return a;
            a %= b;
        }
    }
    
    int lcm(int a, int b)
    {
        int temp = gcd(a, b);
        
        return temp ? (a / temp * b) : 0;
    }
    
    // returns the integer logarithm base 2 of the nearest power of two less than v
    // i.e. log2i(3) == 1, log2i(4) == 2
    uint32 Log2i( uint32 v )
    {
        ASSERT( v != 0 );
        uint32 powTwo = 0;
        while (v >>= 1)
            ++powTwo;
        return powTwo;
    }
    
    
    int Mod(int a, int b)
    {
        ASSERT(b != 0 && "You can't use 0 as a divisor.");
        const int rem = a % b;
        if ( (-1 % 2 == 1) || rem >= 0)
            return rem;
        else
            return rem + abs(b);
    }
    
    uint32 EvilRandom(uint32 ui_max)
    {
        return rand() % ui_max;
    }
    
    uint32 EvilRandomRange(uint32 ui_min, uint32 ui_max)
    {
        return EvilRandom(ui_max-ui_min)+ui_min;
    }
    
    uint32 NextPow2( uint32 v )
    {
        /*
         float nextLog2f = log(x)/log(2);
         int nextLog2i = (int)nextLog2f;
         if ( nextLog2f == (float)nextLog2i ) {
         return x;
         }
         return 1<<(nextLog2i+1);
         */
        
        v--;
        v |= v >> 1;
        v |= v >> 2;
        v |= v >> 4;
        v |= v >> 8;
        v |= v >> 16;
        v++;
        
        return v;
    }
    
    bool IsNormalCardinalOrientationDegrees( int degrees )
    {
        switch (degrees)
        {
            case 0:
            case 90:
            case 180:
            case 270:
                return true;
            default:
                return false;
        }
    }

}  // namespace jd
