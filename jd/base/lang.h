#pragma once

#define MACRO_CONCAT3_FINAL( a,b,c )  a##b##c
#define MACRO_CONCAT3( a,b,c )  MACRO_CONCAT3_FINAL(a,b,c)

#define MACRO_STR_FINAL(a) #a
#define MACRO_STR(a) MACRO_STR_FINAL(a)

#if __cplusplus
#define EXTERN_C_BEGIN extern "C" {
#define EXTERN_C_END }  // extern "C"

#else
#define EXTERN_C_BEGIN
#define EXTERN_C_END
#endif


#define scast static_cast
#define ccast const_cast
#define rcast reinterpret_cast


#ifdef __cplusplus
namespace jd
{
    // ref_cast: pass in fromType, convert it 'by reference' to toType, e.g.:
    // SomeMathVector v;   // ...
    // float * positions = ref_cast<float*>( &v );
    //
    // it's a hack!
    
    template <typename toT, typename fromT>
    const toT & ref_cast( const fromT & a ) { return *(const toT*)&a; }

    template <typename toT, typename fromT>
    toT & ref_cast( fromT & a ) { return *(toT*)&a; }

}
#endif
