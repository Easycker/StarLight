/* ============================================================

Copyright (c) 2009 Advanced Micro Devices, Inc.  All rights reserved.

Redistribution and use of this material is permitted under the following
conditions:

Redistributions must retain the above copyright notice and all terms of this
license.

In no event shall anyone redistributing or accessing or using this material
commence or participate in any arbitration or legal action relating to this
material against Advanced Micro Devices, Inc. or any copyright holders or
contributors. The foregoing shall survive any expiration or termination of
this license or any agreement or access or use related to this material.

ANY BREACH OF ANY TERM OF THIS LICENSE SHALL RESULT IN THE IMMEDIATE REVOCATION
OF ALL RIGHTS TO REDISTRIBUTE, ACCESS OR USE THIS MATERIAL.

THIS MATERIAL IS PROVIDED BY ADVANCED MICRO DEVICES, INC. AND ANY COPYRIGHT
HOLDERS AND CONTRIBUTORS "AS IS" IN ITS CURRENT CONDITION AND WITHOUT ANY
REPRESENTATIONS, GUARANTEE, OR WARRANTY OF ANY KIND OR IN ANY WAY RELATED TO
SUPPORT, INDEMNITY, ERROR FREE OR UNINTERRUPTED OPERATION, OR THAT IT IS FREE
FROM DEFECTS OR VIRUSES.  ALL OBLIGATIONS ARE HEREBY DISCLAIMED - WHETHER
EXPRESS, IMPLIED, OR STATUTORY - INCLUDING, BUT NOT LIMITED TO, ANY IMPLIED
WARRANTIES OF TITLE, MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE,
ACCURACY, COMPLETENESS, OPERABILITY, QUALITY OF SERVICE, OR NON-INFRINGEMENT.
IN NO EVENT SHALL ADVANCED MICRO DEVICES, INC. OR ANY COPYRIGHT HOLDERS OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, PUNITIVE,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, REVENUE, DATA, OR PROFITS; OR
BUSINESS INTERRUPTION) HOWEVER CAUSED OR BASED ON ANY THEORY OF LIABILITY
ARISING IN ANY WAY RELATED TO THIS MATERIAL, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE. THE ENTIRE AND AGGREGATE LIABILITY OF ADVANCED MICRO DEVICES,
INC. AND ANY COPYRIGHT HOLDERS AND CONTRIBUTORS SHALL NOT EXCEED TEN DOLLARS
(US $10.00). ANYONE REDISTRIBUTING OR ACCESSING OR USING THIS MATERIAL ACCEPTS
THIS ALLOCATION OF RISK AND AGREES TO RELEASE ADVANCED MICRO DEVICES, INC. AND
ANY COPYRIGHT HOLDERS AND CONTRIBUTORS FROM ANY AND ALL LIABILITIES,
OBLIGATIONS, CLAIMS, OR DEMANDS IN EXCESS OF TEN DOLLARS (US $10.00). THE
FOREGOING ARE ESSENTIAL TERMS OF THIS LICENSE AND, IF ANY OF THESE TERMS ARE
CONSTRUED AS UNENFORCEABLE, FAIL IN ESSENTIAL PURPOSE, OR BECOME VOID OR
DETRIMENTAL TO ADVANCED MICRO DEVICES, INC. OR ANY COPYRIGHT HOLDERS OR
CONTRIBUTORS FOR ANY REASON, THEN ALL RIGHTS TO REDISTRIBUTE, ACCESS OR USE
THIS MATERIAL SHALL TERMINATE IMMEDIATELY. MOREOVER, THE FOREGOING SHALL
SURVIVE ANY EXPIRATION OR TERMINATION OF THIS LICENSE OR ANY AGREEMENT OR
ACCESS OR USE RELATED TO THIS MATERIAL.

NOTICE IS HEREBY PROVIDED, AND BY REDISTRIBUTING OR ACCESSING OR USING THIS
MATERIAL SUCH NOTICE IS ACKNOWLEDGED, THAT THIS MATERIAL MAY BE SUBJECT TO
RESTRICTIONS UNDER THE LAWS AND REGULATIONS OF THE UNITED STATES OR OTHER
COUNTRIES, WHICH INCLUDE BUT ARE NOT LIMITED TO, U.S. EXPORT CONTROL LAWS SUCH
AS THE EXPORT ADMINISTRATION REGULATIONS AND NATIONAL SECURITY CONTROLS AS
DEFINED THEREUNDER, AS WELL AS STATE DEPARTMENT CONTROLS UNDER THE U.S.
MUNITIONS LIST. THIS MATERIAL MAY NOT BE USED, RELEASED, TRANSFERRED, IMPORTED,
EXPORTED AND/OR RE-EXPORTED IN ANY MANNER PROHIBITED UNDER ANY APPLICABLE LAWS,
INCLUDING U.S. EXPORT CONTROL LAWS REGARDING SPECIFICALLY DESIGNATED PERSONS,
COUNTRIES AND NATIONALS OF COUNTRIES SUBJECT TO NATIONAL SECURITY CONTROLS.
MOREOVER, THE FOREGOING SHALL SURVIVE ANY EXPIRATION OR TERMINATION OF ANY
LICENSE OR AGREEMENT OR ACCESS OR USE RELATED TO THIS MATERIAL.

NOTICE REGARDING THE U.S. GOVERNMENT AND DOD AGENCIES: This material is
provided with "RESTRICTED RIGHTS" and/or "LIMITED RIGHTS" as applicable to
computer software and technical data, respectively. Use, duplication,
distribution or disclosure by the U.S. Government and/or DOD agencies is
subject to the full extent of restrictions in all applicable regulations,
including those found at FAR52.227 and DFARS252.227 et seq. and any successor
regulations thereof. Use of this material by the U.S. Government and/or DOD
agencies is acknowledgment of the proprietary rights of any copyright holders
and contributors, including those of Advanced Micro Devices, Inc., as well as
the provisions of FAR52.227-14 through 23 regarding privately developed and/or
commercial computer software.

This license forms the entire agreement regarding the subject matter hereof and
supersedes all proposals and prior discussions and writings between the parties
with respect thereto. This license does not affect any ownership, rights, title,
or interest in, or relating to, this material. No terms of this license can be
modified or waived, and no breach of this license can be excused, unless done
so in a writing signed by all affected parties. Each term of this license is
separately enforceable. If any term of this license is determined to be or
becomes unenforceable or illegal, such term shall be reformed to the minimum
extent necessary in order for this license to remain in effect in accordance

with its terms as modified by such reformation. This license shall be governed
by and construed in accordance with the laws of the State of Texas without
regard to rules on conflicts of law of any state or jurisdiction or the United
Nations Convention on the International Sale of Goods. All disputes arising out
of this license shall be subject to the jurisdiction of the federal and state
courts in Austin, Texas, and all defenses are hereby waived concerning personal
jurisdiction and venue of these courts.

============================================================ */

#ifndef CL_PLATFORM_H_
#define CL_PLATFORM_H_

#if !defined(_WIN32)
# include <stdint.h>
#endif /* !_WIN32 */
#include <stddef.h>
#include <float.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef _MSC_VER
# define __CL_IMPORT __declspec(dllimport)
# define __CL_EXPORT
# define __CL_CALL   __stdcall
# define __CL_SUFFIX
#else
# define __CL_IMPORT
# define __CL_EXPORT
# define __CL_CALL
# define __CL_SUFFIX
#endif

#define CL_API_CALL   __CL_CALL
#define CL_API_SUFFIX __CL_SUFFIX
#define CL_API_SUFFIX__VERSION_1_0 CL_API_SUFFIX
#define CL_EXTENSION_WEAK_LINK

#ifdef OPENCL_EXPORTS
# define CL_API_ENTRY __CL_EXPORT
#else /* !OPENCL_EXPORTS */
# define CL_API_ENTRY __CL_IMPORT
#endif /* !OPENCL_EXPORTS */

#if defined(_WIN32)
typedef signed   __int8   cl_char;
typedef unsigned __int8   cl_uchar;
typedef signed   __int16  cl_short;
typedef unsigned __int16  cl_ushort;
typedef signed   __int32  cl_int;
typedef unsigned __int32  cl_uint;
typedef signed   __int64  cl_long;
typedef unsigned __int64  cl_ulong;
typedef unsigned __int16  cl_half;
#else /* !_WIN32 */
typedef int8_t    cl_char;
typedef uint8_t   cl_uchar;
typedef int16_t   cl_short;
typedef uint16_t  cl_ushort;
typedef int32_t   cl_int;
typedef uint32_t  cl_uint;
typedef int64_t   cl_long;
typedef uint64_t  cl_ulong;
typedef uint16_t  cl_half;
#endif /* !_WIN32 */
typedef float     cl_float;
typedef double    cl_double;

#define CL_CHAR_BIT         8
#define CL_SCHAR_MAX        127
#define CL_SCHAR_MIN        (-127-1)
#define CL_CHAR_MAX         CL_SCHAR_MAX
#define CL_CHAR_MIN         CL_SCHAR_MIN
#define CL_UCHAR_MAX        255
#define CL_SHRT_MAX         32767
#define CL_SHRT_MIN         (-32767-1)
#define CL_USHRT_MAX        65535
#define CL_INT_MAX          2147483647
#define CL_INT_MIN          (-2147483647-1)
#define CL_UINT_MAX         0xffffffffU
#define CL_LONG_MAX         ((cl_long) 0x7FFFFFFFFFFFFFFFLL)
#define CL_LONG_MIN         ((cl_long) -0x7FFFFFFFFFFFFFFFLL - 1LL)
#define CL_ULONG_MAX        ((cl_ulong) 0xFFFFFFFFFFFFFFFFULL)

#define CL_FLT_DIG          6
#define CL_FLT_MANT_DIG     24
#define CL_FLT_MAX_10_EXP   +38
#define CL_FLT_MAX_EXP      +128
#define CL_FLT_MIN_10_EXP   -37
#define CL_FLT_MIN_EXP      -125
#define CL_FLT_RADIX        2
#define CL_FLT_MAX          FLT_MAX
#define CL_FLT_MIN          FLT_MIN
#define CL_FLT_EPSILON      FLT_EPSILON

#define CL_DBL_DIG          15
#define CL_DBL_MANT_DIG     53
#define CL_DBL_MAX_10_EXP   +308
#define CL_DBL_MAX_EXP      +1024
#define CL_DBL_MIN_10_EXP   -307
#define CL_DBL_MIN_EXP      -1021
#define CL_DBL_RADIX        2
#define CL_DBL_MAX          DBL_MAX
#define CL_DBL_MIN          DBL_MIN
#define CL_DBL_EPSILON      DBL_EPSILON


/*
 * Vector types 
 *
 *  Note:   OpenCL requires that all types be naturally aligned. 
 *          This means that vector types must be naturally aligned.
 *          For example, a vector of four floats must be aligned to
 *          a 16 byte boundary (calculated as 4 * the natural 4-byte 
 *          alignment of the float).  The alignment qualifiers here
 *          will only function properly if your compiler supports them
 *          and if you don't actively work to defeat them.  For example,
 *          in order for a cl_float4 to be 16 byte aligned in a struct,
 *          the start of the struct must itself be 16-byte aligned. 
 *
 *          Maintaining proper alignment is the user's responsibility.
 */

/* Define basic vector types */
#if defined( __VEC__ )
   #include <altivec.h>   /* may be omitted depending on compiler. AltiVec spec provides no way to detect whether the header is required. */
   typedef vector unsigned char     __cl_uchar16;
   typedef vector signed char       __cl_char16;
   typedef vector unsigned short    __cl_ushort8;
   typedef vector signed short      __cl_short8;
   typedef vector unsigned int      __cl_uint4;
   typedef vector signed int        __cl_int4;
   #define  __CL_UCHAR16__  1
   #define  __CL_CHAR16__   1
   #define  __CL_USHORT8__  1
   #define  __CL_SHORT8__   1
   #define  __CL_UINT4__    1
   #define  __CL_INT4__     1
#endif

#if defined( __SSE__ )
    #include <xmmintrin.h>
    #if defined( __GNUC__ )
        typedef float __cl_float4   __attribute__((vector_size(16)));
    #else
        typedef __m128 __cl_float4;
    #endif
    #define __CL_FLOAT4__   1
#endif

#if defined( __SSE2__ )
    #include <emmintrin.h>
    #if defined( __GNUC__ )
        typedef cl_uchar    __cl_uchar16    __attribute__((vector_size(16)));
        typedef cl_char     __cl_char16     __attribute__((vector_size(16)));
        typedef cl_ushort   __cl_ushort8    __attribute__((vector_size(16)));
        typedef cl_short    __cl_short8     __attribute__((vector_size(16)));
        typedef cl_uint     __cl_uint4      __attribute__((vector_size(16)));
        typedef cl_int      __cl_int4       __attribute__((vector_size(16)));
        typedef cl_ulong    __cl_ulong2     __attribute__((vector_size(16)));
        typedef cl_long     __cl_long2      __attribute__((vector_size(16)));
        typedef cl_double   __cl_double2    __attribute__((vector_size(16)));
    #else
        typedef __m128i __cl_uchar16;
        typedef __m128i __cl_char16;
        typedef __m128i __cl_ushort8;
        typedef __m128i __cl_short8;
        typedef __m128i __cl_uint4;
        typedef __m128i __cl_int4;
        typedef __m128i __cl_ulong2;
        typedef __m128i __cl_long2;
        typedef __m128d __cl_double2;
    #endif
    #define __CL_UCHAR16__  1
    #define __CL_CHAR16__   1
    #define __CL_USHORT8__  1
    #define __CL_SHORT8__   1
    #define __CL_INT4__     1
    #define __CL_UINT4__    1
    #define __CL_ULONG2__   1
    #define __CL_LONG2__    1
    #define __CL_DOUBLE2__  1
#endif

#if defined( __MMX__ )
    #include <mmintrin.h>
    #if defined( __GNUC__ )
        typedef cl_uchar    __cl_uchar8     __attribute__((vector_size(8)));
        typedef cl_char     __cl_char8      __attribute__((vector_size(8)));
        typedef cl_ushort   __cl_ushort4    __attribute__((vector_size(8)));
        typedef cl_short    __cl_short4     __attribute__((vector_size(8)));
        typedef cl_uint     __cl_uint2      __attribute__((vector_size(8)));
        typedef cl_int      __cl_int2       __attribute__((vector_size(8)));
        typedef cl_ulong    __cl_ulong1     __attribute__((vector_size(8)));
        typedef cl_long     __cl_long1      __attribute__((vector_size(8)));
        typedef cl_float    __cl_float2     __attribute__((vector_size(8)));
    #else
        typedef __m64       __cl_uchar8;
        typedef __m64       __cl_char8;
        typedef __m64       __cl_ushort4;
        typedef __m64       __cl_short4;
        typedef __m64       __cl_uint2;
        typedef __m64       __cl_int2;
        typedef __m64       __cl_ulong1;
        typedef __m64       __cl_long1;
        typedef __m64       __cl_float2;
    #endif
    #define __CL_UCHAR8__   1
    #define __CL_CHAR8__    1
    #define __CL_USHORT4__  1
    #define __CL_SHORT4__   1
    #define __CL_INT2__     1
    #define __CL_UINT2__    1
    #define __CL_ULONG1__   1
    #define __CL_LONG1__    1
    #define __CL_FLOAT2__   1
#endif

#if defined( __AVX__ )
    #include <gmmintrin.h> 
    #if defined( __GNUC__ )
        typedef cl_float    __cl_float8     __attribute__((vector_size(32)));
        typedef cl_double   __cl_double4    __attribute__((vector_size(32)));
    #else
        typedef __m256      __cl_float8;
        typedef __m256d     __cl_double4;
    #endif
    #define __CL_FLOAT8__   1
    #define __CL_DOUBLE4__  1
#endif

/* Define alignment keys */
#if defined( __GNUC__ )
    #define CL_ALIGNED(_x)      __attribute__ ((aligned(_x)))
#elif defined( _WIN32) && (_MSC_VER)
    #include <crtdefs.h>
    #define CL_ALIGNED(_x)         _CRT_ALIGN(_x)
#else
   #warning  Need to implement some method to align data here
   #define  CL_ALIGNED(_x)
#endif

/* Define cl_vector types */

/* ---- cl_charn ---- */
typedef union
{
    cl_char  CL_ALIGNED(2) s[2];
#if defined( __GNUC__) && ! defined( __STRICT_ANSI__ )
   __extension__ struct{ cl_char  x, y; };
   __extension__ struct{ cl_char  s0, s1; };
   __extension__ struct{ cl_char  lo, hi; };
#endif
#if defined( __CL_CHAR2__) 
    __cl_char2     v2;
#endif
}cl_char2;

typedef union
{
    cl_char  CL_ALIGNED(4) s[4];
#if defined( __GNUC__) && ! defined( __STRICT_ANSI__ )
   __extension__ struct{ cl_char  x, y, z, w; };
   __extension__ struct{ cl_char  s0, s1, s2, s3; };
   __extension__ struct{ cl_char2 lo, hi; };
#endif
#if defined( __CL_CHAR2__) 
    __cl_char2     v2[2];
#endif
#if defined( __CL_CHAR4__) 
    __cl_char4     v4;
#endif
}cl_char4;

typedef union
{
    cl_char   CL_ALIGNED(8) s[8];
#if defined( __GNUC__) && ! defined( __STRICT_ANSI__ )
   __extension__ struct{ cl_char  x, y, z, w; };
   __extension__ struct{ cl_char  s0, s1, s2, s3, s4, s5, s6, s7; };
   __extension__ struct{ cl_char4 lo, hi; };
#endif
#if defined( __CL_CHAR2__) 
    __cl_char2     v2[4];
#endif
#if defined( __CL_CHAR4__) 
    __cl_char4     v4[2];
#endif
#if defined( __CL_CHAR8__ )
    __cl_char8     v8;
#endif
}cl_char8;

typedef union
{
    cl_char  CL_ALIGNED(16) s[16];
#if defined( __GNUC__) && ! defined( __STRICT_ANSI__ )
   __extension__ struct{ cl_char  x, y, z, w, __spacer4, __spacer5, __spacer6, __spacer7, __spacer8, __spacer9, sa, sb, sc, sd, se, sf; };
   __extension__ struct{ cl_char  s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, sA, sB, sC, sD, sE, sF; };
   __extension__ struct{ cl_char8 lo, hi; };
#endif
#if defined( __CL_CHAR2__) 
    __cl_char2     v2[8];
#endif
#if defined( __CL_CHAR4__) 
    __cl_char4     v4[4];
#endif
#if defined( __CL_CHAR8__ )
    __cl_char8     v8[2];
#endif
#if defined( __CL_CHAR16__ )
    __cl_char16    v16;
#endif
}cl_char16;


/* ---- cl_ucharn ---- */
typedef union
{
    cl_uchar  CL_ALIGNED(2) s[2];
#if defined( __GNUC__) && ! defined( __STRICT_ANSI__ )
   __extension__ struct{ cl_uchar  x, y; };
   __extension__ struct{ cl_uchar  s0, s1; };
   __extension__ struct{ cl_uchar  lo, hi; };
#endif
#if defined( __cl_uchar2__) 
    __cl_uchar2     v2;
#endif
}cl_uchar2;

typedef union
{
    cl_uchar  CL_ALIGNED(4) s[4];
#if defined( __GNUC__) && ! defined( __STRICT_ANSI__ )
   __extension__ struct{ cl_uchar  x, y, z, w; };
   __extension__ struct{ cl_uchar  s0, s1, s2, s3; };
   __extension__ struct{ cl_uchar2 lo, hi; };
#endif
#if defined( __CL_UCHAR2__) 
    __cl_uchar2     v2[2];
#endif
#if defined( __CL_UCHAR4__) 
    __cl_uchar4     v4;
#endif
}cl_uchar4;

typedef union
{
    cl_uchar   CL_ALIGNED(8) s[8];
#if defined( __GNUC__) && ! defined( __STRICT_ANSI__ )
   __extension__ struct{ cl_uchar  x, y, z, w; };
   __extension__ struct{ cl_uchar  s0, s1, s2, s3, s4, s5, s6, s7; };
   __extension__ struct{ cl_uchar4 lo, hi; };
#endif
#if defined( __CL_UCHAR2__) 
    __cl_uchar2     v2[4];
#endif
#if defined( __CL_UCHAR4__) 
    __cl_uchar4     v4[2];
#endif
#if defined( __CL_UCHAR8__ )
    __cl_uchar8     v8;
#endif
}cl_uchar8;

typedef union
{
    cl_uchar  CL_ALIGNED(16) s[16];
#if defined( __GNUC__) && ! defined( __STRICT_ANSI__ )
   __extension__ struct{ cl_uchar  x, y, z, w, __spacer4, __spacer5, __spacer6, __spacer7, __spacer8, __spacer9, sa, sb, sc, sd, se, sf; };
   __extension__ struct{ cl_uchar  s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, sA, sB, sC, sD, sE, sF; };
   __extension__ struct{ cl_uchar8 lo, hi; };
#endif
#if defined( __CL_UCHAR2__) 
    __cl_uchar2     v2[8];
#endif
#if defined( __CL_UCHAR4__) 
    __cl_uchar4     v4[4];
#endif
#if defined( __CL_UCHAR8__ )
    __cl_uchar8     v8[2];
#endif
#if defined( __CL_UCHAR16__ )
    __cl_uchar16    v16;
#endif
}cl_uchar16;


/* ---- cl_shortn ---- */
typedef union
{
    cl_short  CL_ALIGNED(4) s[2];
#if defined( __GNUC__) && ! defined( __STRICT_ANSI__ )
   __extension__ struct{ cl_short  x, y; };
   __extension__ struct{ cl_short  s0, s1; };
   __extension__ struct{ cl_short  lo, hi; };
#endif
#if defined( __CL_SHORT2__) 
    __cl_short2     v2;
#endif
}cl_short2;

typedef union
{
    cl_short  CL_ALIGNED(8) s[4];
#if defined( __GNUC__) && ! defined( __STRICT_ANSI__ )
   __extension__ struct{ cl_short  x, y, z, w; };
   __extension__ struct{ cl_short  s0, s1, s2, s3; };
   __extension__ struct{ cl_short2 lo, hi; };
#endif
#if defined( __CL_SHORT2__) 
    __cl_short2     v2[2];
#endif
#if defined( __CL_SHORT4__) 
    __cl_short4     v4;
#endif
}cl_short4;

typedef union
{
    cl_short   CL_ALIGNED(16) s[8];
#if defined( __GNUC__) && ! defined( __STRICT_ANSI__ )
   __extension__ struct{ cl_short  x, y, z, w; };
   __extension__ struct{ cl_short  s0, s1, s2, s3, s4, s5, s6, s7; };
   __extension__ struct{ cl_short4 lo, hi; };
#endif
#if defined( __CL_SHORT2__) 
    __cl_short2     v2[4];
#endif
#if defined( __CL_SHORT4__) 
    __cl_short4     v4[2];
#endif
#if defined( __CL_SHORT8__ )
    __cl_short8     v8;
#endif
}cl_short8;

typedef union
{
    cl_short  CL_ALIGNED(32) s[16];
#if defined( __GNUC__) && ! defined( __STRICT_ANSI__ )
   __extension__ struct{ cl_short  x, y, z, w, __spacer4, __spacer5, __spacer6, __spacer7, __spacer8, __spacer9, sa, sb, sc, sd, se, sf; };
   __extension__ struct{ cl_short  s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, sA, sB, sC, sD, sE, sF; };
   __extension__ struct{ cl_short8 lo, hi; };
#endif
#if defined( __CL_SHORT2__) 
    __cl_short2     v2[8];
#endif
#if defined( __CL_SHORT4__) 
    __cl_short4     v4[4];
#endif
#if defined( __CL_SHORT8__ )
    __cl_short8     v8[2];
#endif
#if defined( __CL_SHORT16__ )
    __cl_short16    v16;
#endif
}cl_short16;


/* ---- cl_ushortn ---- */
typedef union
{
    cl_ushort  CL_ALIGNED(4) s[2];
#if defined( __GNUC__) && ! defined( __STRICT_ANSI__ )
   __extension__ struct{ cl_ushort  x, y; };
   __extension__ struct{ cl_ushort  s0, s1; };
   __extension__ struct{ cl_ushort  lo, hi; };
#endif
#if defined( __CL_USHORT2__) 
    __cl_ushort2     v2;
#endif
}cl_ushort2;

typedef union
{
    cl_ushort  CL_ALIGNED(8) s[4];
#if defined( __GNUC__) && ! defined( __STRICT_ANSI__ )
   __extension__ struct{ cl_ushort  x, y, z, w; };
   __extension__ struct{ cl_ushort  s0, s1, s2, s3; };
   __extension__ struct{ cl_ushort2 lo, hi; };
#endif
#if defined( __CL_USHORT2__) 
    __cl_ushort2     v2[2];
#endif
#if defined( __CL_USHORT4__) 
    __cl_ushort4     v4;
#endif
}cl_ushort4;

typedef union
{
    cl_ushort   CL_ALIGNED(16) s[8];
#if defined( __GNUC__) && ! defined( __STRICT_ANSI__ )
   __extension__ struct{ cl_ushort  x, y, z, w; };
   __extension__ struct{ cl_ushort  s0, s1, s2, s3, s4, s5, s6, s7; };
   __extension__ struct{ cl_ushort4 lo, hi; };
#endif
#if defined( __CL_USHORT2__) 
    __cl_ushort2     v2[4];
#endif
#if defined( __CL_USHORT4__) 
    __cl_ushort4     v4[2];
#endif
#if defined( __CL_USHORT8__ )
    __cl_ushort8     v8;
#endif
}cl_ushort8;

typedef union
{
    cl_ushort  CL_ALIGNED(32) s[16];
#if defined( __GNUC__) && ! defined( __STRICT_ANSI__ )
   __extension__ struct{ cl_ushort  x, y, z, w, __spacer4, __spacer5, __spacer6, __spacer7, __spacer8, __spacer9, sa, sb, sc, sd, se, sf; };
   __extension__ struct{ cl_ushort  s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, sA, sB, sC, sD, sE, sF; };
   __extension__ struct{ cl_ushort8 lo, hi; };
#endif
#if defined( __CL_USHORT2__) 
    __cl_ushort2     v2[8];
#endif
#if defined( __CL_USHORT4__) 
    __cl_ushort4     v4[4];
#endif
#if defined( __CL_USHORT8__ )
    __cl_ushort8     v8[2];
#endif
#if defined( __CL_USHORT16__ )
    __cl_ushort16    v16;
#endif
}cl_ushort16;

/* ---- cl_intn ---- */
typedef union
{
    cl_int  CL_ALIGNED(8) s[2];
#if defined( __GNUC__) && ! defined( __STRICT_ANSI__ )
   __extension__ struct{ cl_int  x, y; };
   __extension__ struct{ cl_int  s0, s1; };
   __extension__ struct{ cl_int  lo, hi; };
#endif
#if defined( __CL_INT2__) 
    __cl_int2     v2;
#endif
}cl_int2;

typedef union
{
    cl_int  CL_ALIGNED(16) s[4];
#if defined( __GNUC__) && ! defined( __STRICT_ANSI__ )
   __extension__ struct{ cl_int  x, y, z, w; };
   __extension__ struct{ cl_int  s0, s1, s2, s3; };
   __extension__ struct{ cl_int2 lo, hi; };
#endif
#if defined( __CL_INT2__) 
    __cl_int2     v2[2];
#endif
#if defined( __CL_INT4__) 
    __cl_int4     v4;
#endif
}cl_int4;

typedef union
{
    cl_int   CL_ALIGNED(32) s[8];
#if defined( __GNUC__) && ! defined( __STRICT_ANSI__ )
   __extension__ struct{ cl_int  x, y, z, w; };
   __extension__ struct{ cl_int  s0, s1, s2, s3, s4, s5, s6, s7; };
   __extension__ struct{ cl_int4 lo, hi; };
#endif
#if defined( __CL_INT2__) 
    __cl_int2     v2[4];
#endif
#if defined( __CL_INT4__) 
    __cl_int4     v4[2];
#endif
#if defined( __CL_INT8__ )
    __cl_int8     v8;
#endif
}cl_int8;

typedef union
{
    cl_int  CL_ALIGNED(64) s[16];
#if defined( __GNUC__) && ! defined( __STRICT_ANSI__ )
   __extension__ struct{ cl_int  x, y, z, w, __spacer4, __spacer5, __spacer6, __spacer7, __spacer8, __spacer9, sa, sb, sc, sd, se, sf; };
   __extension__ struct{ cl_int  s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, sA, sB, sC, sD, sE, sF; };
   __extension__ struct{ cl_int8 lo, hi; };
#endif
#if defined( __CL_INT2__) 
    __cl_int2     v2[8];
#endif
#if defined( __CL_INT4__) 
    __cl_int4     v4[4];
#endif
#if defined( __CL_INT8__ )
    __cl_int8     v8[2];
#endif
#if defined( __CL_INT16__ )
    __cl_int16    v16;
#endif
}cl_int16;


/* ---- cl_uintn ---- */
typedef union
{
    cl_uint  CL_ALIGNED(8) s[2];
#if defined( __GNUC__) && ! defined( __STRICT_ANSI__ )
   __extension__ struct{ cl_uint  x, y; };
   __extension__ struct{ cl_uint  s0, s1; };
   __extension__ struct{ cl_uint  lo, hi; };
#endif
#if defined( __CL_UINT2__) 
    __cl_uint2     v2;
#endif
}cl_uint2;

typedef union
{
    cl_uint  CL_ALIGNED(16) s[4];
#if defined( __GNUC__) && ! defined( __STRICT_ANSI__ )
   __extension__ struct{ cl_uint  x, y, z, w; };
   __extension__ struct{ cl_uint  s0, s1, s2, s3; };
   __extension__ struct{ cl_uint2 lo, hi; };
#endif
#if defined( __CL_UINT2__) 
    __cl_uint2     v2[2];
#endif
#if defined( __CL_UINT4__) 
    __cl_uint4     v4;
#endif
}cl_uint4;

typedef union
{
    cl_uint   CL_ALIGNED(32) s[8];
#if defined( __GNUC__) && ! defined( __STRICT_ANSI__ )
   __extension__ struct{ cl_uint  x, y, z, w; };
   __extension__ struct{ cl_uint  s0, s1, s2, s3, s4, s5, s6, s7; };
   __extension__ struct{ cl_uint4 lo, hi; };
#endif
#if defined( __CL_UINT2__) 
    __cl_uint2     v2[4];
#endif
#if defined( __CL_UINT4__) 
    __cl_uint4     v4[2];
#endif
#if defined( __CL_UINT8__ )
    __cl_uint8     v8;
#endif
}cl_uint8;

typedef union
{
    cl_uint  CL_ALIGNED(64) s[16];
#if defined( __GNUC__) && ! defined( __STRICT_ANSI__ )
   __extension__ struct{ cl_uint  x, y, z, w, __spacer4, __spacer5, __spacer6, __spacer7, __spacer8, __spacer9, sa, sb, sc, sd, se, sf; };
   __extension__ struct{ cl_uint  s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, sA, sB, sC, sD, sE, sF; };
   __extension__ struct{ cl_uint8 lo, hi; };
#endif
#if defined( __CL_UINT2__) 
    __cl_uint2     v2[8];
#endif
#if defined( __CL_UINT4__) 
    __cl_uint4     v4[4];
#endif
#if defined( __CL_UINT8__ )
    __cl_uint8     v8[2];
#endif
#if defined( __CL_UINT16__ )
    __cl_uint16    v16;
#endif
}cl_uint16;

/* ---- cl_longn ---- */
typedef union
{
    cl_long  CL_ALIGNED(16) s[2];
#if defined( __GNUC__) && ! defined( __STRICT_ANSI__ )
   __extension__ struct{ cl_long  x, y; };
   __extension__ struct{ cl_long  s0, s1; };
   __extension__ struct{ cl_long  lo, hi; };
#endif
#if defined( __CL_LONG2__) 
    __cl_long2     v2;
#endif
}cl_long2;

typedef union
{
    cl_long  CL_ALIGNED(32) s[4];
#if defined( __GNUC__) && ! defined( __STRICT_ANSI__ )
   __extension__ struct{ cl_long  x, y, z, w; };
   __extension__ struct{ cl_long  s0, s1, s2, s3; };
   __extension__ struct{ cl_long2 lo, hi; };
#endif
#if defined( __CL_LONG2__) 
    __cl_long2     v2[2];
#endif
#if defined( __CL_LONG4__) 
    __cl_long4     v4;
#endif
}cl_long4;

typedef union
{
    cl_long   CL_ALIGNED(64) s[8];
#if defined( __GNUC__) && ! defined( __STRICT_ANSI__ )
   __extension__ struct{ cl_long  x, y, z, w; };
   __extension__ struct{ cl_long  s0, s1, s2, s3, s4, s5, s6, s7; };
   __extension__ struct{ cl_long4 lo, hi; };
#endif
#if defined( __CL_LONG2__) 
    __cl_long2     v2[4];
#endif
#if defined( __CL_LONG4__) 
    __cl_long4     v4[2];
#endif
#if defined( __CL_LONG8__ )
    __cl_long8     v8;
#endif
}cl_long8;

typedef union
{
    cl_long  CL_ALIGNED(128) s[16];
#if defined( __GNUC__) && ! defined( __STRICT_ANSI__ )
   __extension__ struct{ cl_long  x, y, z, w, __spacer4, __spacer5, __spacer6, __spacer7, __spacer8, __spacer9, sa, sb, sc, sd, se, sf; };
   __extension__ struct{ cl_long  s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, sA, sB, sC, sD, sE, sF; };
   __extension__ struct{ cl_long8 lo, hi; };
#endif
#if defined( __CL_LONG2__) 
    __cl_long2     v2[8];
#endif
#if defined( __CL_LONG4__) 
    __cl_long4     v4[4];
#endif
#if defined( __CL_LONG8__ )
    __cl_long8     v8[2];
#endif
#if defined( __CL_LONG16__ )
    __cl_long16    v16;
#endif
}cl_long16;


/* ---- cl_ulongn ---- */
typedef union
{
    cl_ulong  CL_ALIGNED(16) s[2];
#if defined( __GNUC__) && ! defined( __STRICT_ANSI__ )
   __extension__ struct{ cl_ulong  x, y; };
   __extension__ struct{ cl_ulong  s0, s1; };
   __extension__ struct{ cl_ulong  lo, hi; };
#endif
#if defined( __CL_ULONG2__) 
    __cl_ulong2     v2;
#endif
}cl_ulong2;

typedef union
{
    cl_ulong  CL_ALIGNED(32) s[4];
#if defined( __GNUC__) && ! defined( __STRICT_ANSI__ )
   __extension__ struct{ cl_ulong  x, y, z, w; };
   __extension__ struct{ cl_ulong  s0, s1, s2, s3; };
   __extension__ struct{ cl_ulong2 lo, hi; };
#endif
#if defined( __CL_ULONG2__) 
    __cl_ulong2     v2[2];
#endif
#if defined( __CL_ULONG4__) 
    __cl_ulong4     v4;
#endif
}cl_ulong4;

typedef union
{
    cl_ulong   CL_ALIGNED(64) s[8];
#if defined( __GNUC__) && ! defined( __STRICT_ANSI__ )
   __extension__ struct{ cl_ulong  x, y, z, w; };
   __extension__ struct{ cl_ulong  s0, s1, s2, s3, s4, s5, s6, s7; };
   __extension__ struct{ cl_ulong4 lo, hi; };
#endif
#if defined( __CL_ULONG2__) 
    __cl_ulong2     v2[4];
#endif
#if defined( __CL_ULONG4__) 
    __cl_ulong4     v4[2];
#endif
#if defined( __CL_ULONG8__ )
    __cl_ulong8     v8;
#endif
}cl_ulong8;

typedef union
{
    cl_ulong  CL_ALIGNED(128) s[16];
#if defined( __GNUC__) && ! defined( __STRICT_ANSI__ )
   __extension__ struct{ cl_ulong  x, y, z, w, __spacer4, __spacer5, __spacer6, __spacer7, __spacer8, __spacer9, sa, sb, sc, sd, se, sf; };
   __extension__ struct{ cl_ulong  s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, sA, sB, sC, sD, sE, sF; };
   __extension__ struct{ cl_ulong8 lo, hi; };
#endif
#if defined( __CL_ULONG2__) 
    __cl_ulong2     v2[8];
#endif
#if defined( __CL_ULONG4__) 
    __cl_ulong4     v4[4];
#endif
#if defined( __CL_ULONG8__ )
    __cl_ulong8     v8[2];
#endif
#if defined( __CL_ULONG16__ )
    __cl_ulong16    v16;
#endif
}cl_ulong16;


/* --- cl_floatn ---- */

typedef union
{
    cl_float  CL_ALIGNED(8) s[2];
#if defined( __GNUC__) && ! defined( __STRICT_ANSI__ )
   __extension__ struct{ cl_float  x, y; };
   __extension__ struct{ cl_float  s0, s1; };
   __extension__ struct{ cl_float  lo, hi; };
#endif
#if defined( __CL_FLOAT2__) 
    __cl_float2     v2;
#endif
}cl_float2;

typedef union
{
    cl_float  CL_ALIGNED(16) s[4];
#if defined( __GNUC__) && ! defined( __STRICT_ANSI__ )
   __extension__ struct{ cl_float   x, y, z, w; };
   __extension__ struct{ cl_float   s0, s1, s2, s3; };
   __extension__ struct{ cl_float2  lo, hi; };
#endif
#if defined( __CL_FLOAT2__) 
    __cl_float2     v2[2];
#endif
#if defined( __CL_FLOAT4__) 
    __cl_float4     v4;
#endif
}cl_float4;

typedef union
{
    cl_float   CL_ALIGNED(32) s[8];
#if defined( __GNUC__) && ! defined( __STRICT_ANSI__ )
   __extension__ struct{ cl_float   x, y, z, w; };
   __extension__ struct{ cl_float   s0, s1, s2, s3, s4, s5, s6, s7; };
   __extension__ struct{ cl_float4  lo, hi; };
#endif
#if defined( __CL_FLOAT2__) 
    __cl_float2     v2[4];
#endif
#if defined( __CL_FLOAT4__) 
    __cl_float4     v4[2];
#endif
#if defined( __CL_FLOAT8__ )
    __cl_float8     v8;
#endif
}cl_float8;

typedef union
{
    cl_float  CL_ALIGNED(64) s[16];
#if defined( __GNUC__) && ! defined( __STRICT_ANSI__ )
   __extension__ struct{ cl_float  x, y, z, w, __spacer4, __spacer5, __spacer6, __spacer7, __spacer8, __spacer9, sa, sb, sc, sd, se, sf; };
   __extension__ struct{ cl_float  s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, sA, sB, sC, sD, sE, sF; };
   __extension__ struct{ cl_float8 lo, hi; };
#endif
#if defined( __CL_FLOAT2__) 
    __cl_float2     v2[8];
#endif
#if defined( __CL_FLOAT4__) 
    __cl_float4     v4[4];
#endif
#if defined( __CL_FLOAT8__ )
    __cl_float8     v8[2];
#endif
#if defined( __CL_FLOAT16__ )
    __cl_float16    v16;
#endif
}cl_float16;

/* --- cl_doublen ---- */

typedef union
{
    cl_double  CL_ALIGNED(16) s[2];
#if defined( __GNUC__) && ! defined( __STRICT_ANSI__ )
   __extension__ struct{ cl_double  x, y; };
   __extension__ struct{ cl_double s0, s1; };
   __extension__ struct{ cl_double lo, hi; };
#endif
#if defined( __CL_DOUBLE2__) 
    __cl_double2     v2;
#endif
}cl_double2;

typedef union
{
    cl_double  CL_ALIGNED(32) s[4];
#if defined( __GNUC__) && ! defined( __STRICT_ANSI__ )
   __extension__ struct{ cl_double  x, y, z, w; };
   __extension__ struct{ cl_double  s0, s1, s2, s3; };
   __extension__ struct{ cl_double2 lo, hi; };
#endif
#if defined( __CL_DOUBLE2__) 
    __cl_double2     v2[2];
#endif
#if defined( __CL_DOUBLE4__) 
    __cl_double4     v4;
#endif
}cl_double4;

typedef union
{
    cl_double   CL_ALIGNED(64) s[8];
#if defined( __GNUC__) && ! defined( __STRICT_ANSI__ )
   __extension__ struct{ cl_double  x, y, z, w; };
   __extension__ struct{ cl_double  s0, s1, s2, s3, s4, s5, s6, s7; };
   __extension__ struct{ cl_double4 lo, hi; };
#endif
#if defined( __CL_DOUBLE2__) 
    __cl_double2     v2[4];
#endif
#if defined( __CL_DOUBLE4__) 
    __cl_double4     v4[2];
#endif
#if defined( __CL_DOUBLE8__ )
    __cl_double8     v8;
#endif
}cl_double8;

typedef union
{
    cl_double  CL_ALIGNED(128) s[16];
#if defined( __GNUC__) && ! defined( __STRICT_ANSI__ )
   __extension__ struct{ cl_double  x, y, z, w, __spacer4, __spacer5, __spacer6, __spacer7, __spacer8, __spacer9, sa, sb, sc, sd, se, sf; };
   __extension__ struct{ cl_double  s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, sA, sB, sC, sD, sE, sF; };
   __extension__ struct{ cl_double8 lo, hi; };
#endif
#if defined( __CL_DOUBLE2__) 
    __cl_double2     v2[8];
#endif
#if defined( __CL_DOUBLE4__) 
    __cl_double4     v4[4];
#endif
#if defined( __CL_DOUBLE8__ )
    __cl_double8     v8[2];
#endif
#if defined( __CL_DOUBLE16__ )
    __cl_double16    v16;
#endif
}cl_double16;


#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* CL_PLATFORM_H_ */
