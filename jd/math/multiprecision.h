#pragma once

#include <jd/math/config.h>

#if JD_MATH_MULTIPRECISION

#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/cpp_dec_float.hpp>
//#include <boost/multiprecision/integer.hpp>

//#include <jd/math/mat3x2.h>
//#include <jd/math/vec2.h>

namespace jd {

    namespace mp = boost::multiprecision;
    typedef mp::number<mp::cpp_int_backend<>, mp::et_off>       int_mp;
    typedef mp::number<mp::cpp_rational_backend, mp::et_off>    rational_mp;
    //    typedef mp::number<mp::cpp_dec_float<0,boost::int32_t,std::allocator<int> >, mp::et_off>        float_mp;  // 0 means 'unlimited'
    typedef mp::number<mp::cpp_dec_float<50>, mp::et_off>        float_mp;

    // rational
    typedef vec2<rational_mp>    vec2r_mp;
    typedef mat3x2<rational_mp>  mat3x2r_mp;
    typedef pos_scale<rational_mp> pos_scale_r_mp;
    typedef rect2<rational_mp>   rect2r_mp;
    typedef line2<rational_mp>   line2r_mp;
    
    // float
    typedef vec2<float_mp>    vec2f_mp;
    typedef mat3x2<float_mp>  mat3x2f_mp;
    typedef pos_scale<float_mp> pos_scale_f_mp;
    typedef rect2<float_mp>   rect2f_mp;
    typedef line2<float_mp>   line2f_mp;

    // ...
    
    // hmm, try not to take the sqrt of a rational? :-D
//    template<>
//    inline rational_mp Length( vec2r_mp const & v )
//    {
//        rational_mp sqrlen = LengthSquared(v);
//        if( sqrlen.is_zero())
//            return rational_mp(0);
//        mp::sqrt()
//        rational_mp value = mp::sqrt(sqrlen);
//        int_mp num = numerator(sqrlen);
//        int_mp den = denominator(sqrlen);
//        std::cout << "len=" << sqrlen << " num=" << num << " den=" << den << std::endl;
//        std::cout << "  sqrt(num)=" << mp::sqrt(num) << std::endl;
//        std::cout << "  sqrt(den)=" << mp::sqrt(den) << std::endl;
//        rational_mp x(mp::sqrt(num),
//                 mp::sqrt(den));
//        return x;
////        return rational_mp(mp::sqrt(num),
////                      mp::sqrt(den));
//        //return mp::sqrt<rational_mp>( LengthSquared(v) );
//    }

    
    
    
    // convert_to
    template<typename To, typename From>
    To convert_to( const From & f );
    
    // real
    // (just use builtin constructor or convert_to member func)
    
    // vec2
    template<>
    inline vec2r_mp convert_to<vec2r_mp>( const vec2f & v ) {
        return vec2r_mp( rational_mp(v.x), rational_mp(v.y) );
    }
    
    template<>
    inline vec2f_mp convert_to<vec2f_mp>( const vec2f & v ) {
        return vec2f_mp( float_mp(v.x), float_mp(v.y) );
    }
    
    template<>
    inline vec2f convert_to<vec2f>( const vec2r_mp & v ) {
        return vec2f( v.x.convert_to<float>(), v.y.convert_to<float>() );
    }

    template<>
    inline vec2f convert_to<vec2f>( const vec2f_mp & v ) {
        return vec2f( v.x.convert_to<float>(), v.y.convert_to<float>() );
    }
    
    template<>
    inline rect2f convert_to<rect2f>( const rect2r_mp & v ) {
        return rect2f( convert_to<vec2f>(v.mmin), convert_to<vec2f>(v.mmax) );
    }

    // mat3x2
    template<>
    inline mat3x2r_mp convert_to<mat3x2r_mp>( const mat3x2f & m ) {
        return mat3x2r_mp(rational_mp(m.a),
                        rational_mp(m.b),
                        rational_mp(m.c),
                        rational_mp(m.d),
                        rational_mp(m.tx),
                        rational_mp(m.ty)
                        );
    }

    template<>
    inline mat3x2f_mp convert_to<mat3x2f_mp>( const mat3x2f & m ) {
        return mat3x2f_mp(float_mp(m.a),
                          float_mp(m.b),
                          float_mp(m.c),
                          float_mp(m.d),
                          float_mp(m.tx),
                          float_mp(m.ty)
                          );
    }
    
    template<>
    inline mat3x2f convert_to<mat3x2f>( const mat3x2r_mp & m ) {
        return mat3x2f(m.a.convert_to<float>(),
                       m.b.convert_to<float>(),
                       m.c.convert_to<float>(),
                       m.d.convert_to<float>(),
                       m.tx.convert_to<float>(),
                       m.ty.convert_to<float>()
                       );
    }
    
    template<>
    inline mat3x2f convert_to<mat3x2f>( const pos_scale_r_mp & ps ) {
        float scale = ps.scale.convert_to<float>();
        vec2f pos = convert_to<vec2f>(ps.pos);
        return mat3x2f(scale,0, 0,scale, pos.x,pos.y);
    }
    
    template<>
    inline mat3x2f convert_to<mat3x2f>( const pos_scale_f_mp & ps ) {
        float scale = ps.scale.convert_to<float>();
        vec2f pos = convert_to<vec2f>(ps.pos);
        return mat3x2f(scale,0, 0,scale, pos.x,pos.y);
    }
    
    // return the number of bits of precision
    template<typename MPnum>
    inline int mp_precision( const MPnum & a )
    {
        // 'size' means number of limbs (ints)
        int nL = a.backend().size();
        int sizeofL = sizeof(mp::limb_type);
        int nbits = nL ? nL*sizeofL*8 : 0;
        typename mp::limb_type msL = a.backend().limbs()[nL-1];

        // find the position of the leading bit
        // (numerical recipes in c has a faster way)
        for(int i=sizeofL*8-1; i>=0; --i) {
            if( msL>>i ) {
                return nbits - (sizeofL*8-1-i);
            }
        }
        return 0;
    }
    
    template<>
    inline int mp_precision( const float_mp & a )
    {
        // [jd] stupid boost cpp_dec_float has no precision()
        //return a.backend().prec_elem;
        //... so i added one :-(
        return a.precision();
    }
    
    template<>
    inline int mp_precision( const rational_mp & a )
    {
        int numer_p = mp_precision( numerator(a) );
        int denom_p = mp_precision( denominator(a) );
        return Max( numer_p, denom_p );
    }
    
//    inline int mp_limb_size() {
//        return sizeof(mp::limb_type);
//    }

    // return the truncation of a number, to a given number of bits of precision
    template<typename MPnum>
    inline long mp_mask_low_bits( const MPnum & a, int bits ) {
        if( bits <= 0 ) {
            return 0;
        }
        int p = mp_precision(a);
        if( p <= bits ) {
            return a.template convert_to<long>();
        }
        ASSERT( bits < 8*sizeof(long) );  // uh, probably wrong
        long a_int = a.template convert_to<long>();
        return a_int & ((1<<bits)-1);
    }
    
    // return the truncation of a number, to a given number of bits of precision
    template<typename MPnum>
    inline MPnum mp_truncation( const MPnum & a, int bits ) {
        if( bits <= 0 ) {
            return MPnum(0);
        }
        int p = mp_precision(a);
        if( p <= bits ) {
            return a;
        }
        int dp = p - bits;
        return a >> dp;
    }
    
    template<>
    inline float_mp mp_truncation( const float_mp & a, int digits ) {
        float_mp tr = a;
        tr.precision(digits);
        return tr;
    }
    
    template<>
    inline rational_mp mp_truncation( const rational_mp & a, int bits ) {
        if( bits <= 0 ) {
            return rational_mp(0);
        }
        int p_numer = mp_precision( numerator(a) );
        int p_denom = mp_precision( denominator(a) );
        int p = Max(p_numer, p_denom);
        if( p <= bits ) {
            return a;
        }
        int dp = p - bits;
        // divide both numer and denom by same amount (dp)
        
        if( p_denom-dp <= 0 ) {
            // truncate by integer-divide, then truncate to desired bit count
            return rational_mp( mp_truncation(numerator(a)/denominator(a),bits) );
        }
        else {
            return rational_mp( mp_truncation(numerator(a), p_numer-dp),
                           mp_truncation(denominator(a), p_denom-dp) );
        }
    }
    
    template<>
    inline vec2r_mp mp_truncation( const vec2r_mp & a, int bits ) {
        return vec2r_mp( mp_truncation(a.x,bits), mp_truncation(a.y,bits) );
    }

    template<>
    inline vec2f_mp mp_truncation( const vec2f_mp & a, int digits ) {
        return vec2f_mp( mp_truncation(a.x,digits), mp_truncation(a.y,digits) );
    }

    template<>
    inline rect2r_mp mp_truncation( const rect2r_mp & a, int bits ) {
        return rect2r_mp( mp_truncation(a.Min(),bits), mp_truncation(a.Max(),bits) );
    }
    
    
    
    // mat3x2
    
    inline vec2f mat3x2_mulPoint_trunc( const mat3x2r_mp & m, const vec2r_mp & p, int precision_bits )
    {
        return convert_to<vec2f>(mp_truncation( mat3x2_mulPoint(m, p), precision_bits));
    }
    
    inline vec2f mat3x2_mulVector_trunc( const mat3x2r_mp & m, const vec2r_mp & v, int precision_bits )
    {
        return convert_to<vec2f>(mp_truncation( mat3x2_mulVector(m, v), precision_bits));
    }
    
    
    // pos_scale
    
    template<typename T>
    inline vec2f pos_scale_mulPoint_trunc( const pos_scale<T> & m, const vec2<T> & p, int precision_bits )
    {
        return convert_to<vec2f>(mp_truncation( pos_scale_mulPoint(m, p), precision_bits));
    }
    
    template<typename T>
    inline vec2f pos_scale_mulVector_trunc( const pos_scale<T> & m, const vec2<T> & v, int precision_bits )
    {
        return convert_to<vec2f>(mp_truncation( pos_scale_mulVector(m, v), precision_bits));
    }

    const int_mp intmp_zero(0);
    const int_mp intmp_one(1);
    const int_mp intmp_neg_one(-1);
    const rational_mp realmp_zero(0);
    const rational_mp realmp_one(1);
    
}

#endif