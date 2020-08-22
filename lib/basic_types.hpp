#pragma once

///Guaranteed 8-bit unsigned integer type
typedef unsigned char     u_int8;     // range is [0..255]
///Guaranteed 8-bit signed integer type
typedef signed   char     int8;       // range is [-128..127]
///Guaranteed 16-bit unsigned integer type
typedef unsigned short    u_int16;    // range is [0..65.535]
///Guaranteed 16-bit signed integer type
typedef signed   short    int16;      // range is [-32768..32.767]
///Guaranteed 32-bit unsigned integer type
typedef unsigned long     u_int32;    // range is [0..4.294.967.295]
#if !defined(BASIC_TYPES_SKIP_INT32) && !defined(_SAL_TYPES_H)
///Guaranteed 32-bit signed integer type
typedef signed long       int32;      // range is [-2.147.483.648..
                                      //            2.147.483.647]
#endif

#ifdef WIN32
typedef unsigned __int64    u_int64;
typedef __int64             int64;
#else
#if !defined(BASIC_TYPES_SKIP_INT64)
typedef unsigned long long  u_int64;
typedef signed long long    int64;
#endif
#endif

#ifdef __cplusplus
typedef bool                   boolean;
#endif // __cplusplus

typedef unsigned char          T_bool;
#ifndef TRUE
#define TRUE  1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

#undef NULL

#ifdef __cplusplus
#define NULL (0)
#else
#define NULL  ((void*)0)
#endif /* __cplusplus */

