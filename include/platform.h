/***************************************************************************************************

Compute Engine - $CE_VERSION_TAG$ <$CE_ID_TAG$>

Copyright (c) 2010, Derek Gerstmann <derek.gerstmann[|AT|]uwa.edu.au> 
The University of Western Australia. All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

    * Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

***************************************************************************************************/

#ifndef __CE_PLATFORM_H__
#define __CE_PLATFORM_H__

/* Platform  **************************************************************************************/

#if defined(__APPLE__) && !defined(CE_PLATFORM_MACOSX)
	#define CE_PLATFORM_MACOSX	(1)
#elif (defined (_WIN32) || defined(__WIN32__)) && defined(_MSC_VER) && !defined(CE_PLATFORM_WINDOWS)
	#define CE_PLATFORM_WINDOWS	(1)
#elif (defined(linux) || defined(__linux) || defined (__linux__)) && !defined(CE_PLATFORM_LINUX)
	#define CE_PLATFORM_LINUX	(1)
#endif

/* C Declaration **********************************************************************************/

#if !defined(CE_EXTERN_C_BEGIN)
	#if defined(__cplusplus)
		#define CE_EXTERN_C_BEGIN extern CE_API_EXPORT "C" {
		#define CE_EXTERN_C_END   }
	#else
		#define CE_EXTERN_C_BEGIN
		#define CE_EXTERN_C_END
	#endif
#endif

/* Restrict Declaration ***************************************************************************/

#if !defined(__cplusplus) && defined(__GNUC__)
	#define CE_RESTRICT			restrict
#else
	#define CE_RESTRICT
#endif

/**************************************************************************************************/

CE_EXTERN_C_BEGIN

/* Export Declaration *****************************************************************************/

#if defined(__WIN32__)
	#if defined(CE_SHARED_LIB_TARGET) && defined(CE_SHARED_LIB_EXPORT)
		#define CE_API_EXPORT 				__declspec(dllexport) 
	#elif defined(CE_SHARED_LIB_TARGET)
		#define CE_API_EXPORT 				__declspec(dllimport) 
	#endif
#else
	#define CE_API_EXPORT
#endif

/* Inline Declaration *****************************************************************************/

#if !defined(CE_INLINE)
	#if defined(__GNUC__) && (__GNUC__ == 4) && !defined(DEBUG)
		#define CE_INLINE 			static __inline__ __attribute__((always_inline))
	#elif defined(__GNUC__)
		#define CE_INLINE 			static __inline__
	#elif defined(__cplusplus)
		#define CE_INLINE 			static inline
	#elif defined(_MSC_VER)
		#define CE_INLINE 			static __inline
	#elif defined(__WIN32__)
		#define CE_INLINE			static __inline__
	#endif
#endif

/* alignment detection *********************************************************/

#if defined(__GNUC__)
	#define CE_DECLARE_ALIGNED(x) 
	#define CE_ALIGN_ATTRIBUTE(x) __attribute__ ((aligned(x)))
#elif defined(_MSC_VER)
	#define CE_DECLARE_ALIGNED(x) __declspec(align( x )) 
	#define CE_ALIGN_ATTRIBUTE(x) 
#else
	#define CE_DECLARE_ALIGNED(x)
	#define CE_ALIGN_ATTRIBUTE(x)
#endif

/* platform types *************************************************************/

#if (CE_PLATFORM_WINDOWS)

#define _CE_INT8_ 			signed   __int8
#define _CE_UINT8_			unsigned __int8
#define _CE_INT16_			signed   __int16
#define _CE_UINT16_			unsigned __int16
#define _CE_INT32_			signed   __int32
#define _CE_UINT32_			unsigned __int32
#define _CE_INT64_			signed   __int64
#define _CE_UINT64_			unsigned __int64

#else

#include <stdint.h>

#define _CE_INT8_         	int8_t
#define _CE_UINT8_			uint8_t
#define _CE_INT16_			int16_t
#define _CE_UINT16_			uint16_t
#define _CE_INT32_			int32_t
#define _CE_UINT32_			uint32_t
#define _CE_INT64_			int64_t
#define _CE_UINT64_			uint64_t

#endif

/* Scalar Types (aligned) *************************************************************/

typedef _CE_INT8_      		ce_char;
typedef _CE_UINT8_         	ce_uchar;
typedef _CE_INT16_         	ce_short    		CE_ALIGN_ATTRIBUTE(2);
typedef _CE_UINT16_        	ce_ushort   		CE_ALIGN_ATTRIBUTE(2);
typedef _CE_INT32_         	ce_int      		CE_ALIGN_ATTRIBUTE(4);
typedef _CE_UINT32_        	ce_uint     		CE_ALIGN_ATTRIBUTE(4);
typedef _CE_INT64_         	ce_long     		CE_ALIGN_ATTRIBUTE(8);
typedef _CE_UINT64_        	ce_ulong    		CE_ALIGN_ATTRIBUTE(8);
typedef _CE_UINT16_        	ce_half     		CE_ALIGN_ATTRIBUTE(2);
typedef float              	ce_float    		CE_ALIGN_ATTRIBUTE(4);
typedef double             	ce_double   		CE_ALIGN_ATTRIBUTE(8);
typedef ce_uint				ce_bitfield;
typedef ce_ulong			ce_hashcode;
typedef ce_uint				ce_bool;

/* Vector Types (aligned) *************************************************************/

typedef _CE_INT8_          	ce_char2[2]     	CE_ALIGN_ATTRIBUTE(2);
typedef _CE_INT8_          	ce_char4[4]     	CE_ALIGN_ATTRIBUTE(4);
typedef _CE_INT8_          	ce_char8[8]     	CE_ALIGN_ATTRIBUTE(8);
typedef _CE_INT8_          	ce_char16[16]   	CE_ALIGN_ATTRIBUTE(16);
typedef _CE_UINT8_         	ce_uchar2[2]    	CE_ALIGN_ATTRIBUTE(2);
typedef _CE_UINT8_         	ce_uchar4[4]    	CE_ALIGN_ATTRIBUTE(4);
typedef _CE_UINT8_         	ce_uchar8[8]    	CE_ALIGN_ATTRIBUTE(8);
typedef _CE_UINT8_         	ce_uchar16[16]  	CE_ALIGN_ATTRIBUTE(16);
typedef _CE_INT16_         	ce_short2[2]     	CE_ALIGN_ATTRIBUTE(4);
typedef _CE_INT16_         	ce_short4[4]     	CE_ALIGN_ATTRIBUTE(8);
typedef _CE_INT16_         	ce_short8[8]     	CE_ALIGN_ATTRIBUTE(16);
typedef _CE_INT16_         	ce_short16[16]   	CE_ALIGN_ATTRIBUTE(32);
typedef _CE_UINT16_        	ce_ushort2[2]    	CE_ALIGN_ATTRIBUTE(4);
typedef _CE_UINT16_        	ce_ushort4[4]    	CE_ALIGN_ATTRIBUTE(8);
typedef _CE_UINT16_        	ce_ushort8[8]    	CE_ALIGN_ATTRIBUTE(16);
typedef _CE_UINT16_        	ce_ushort16[16]  	CE_ALIGN_ATTRIBUTE(32);
typedef _CE_INT32_         	ce_int2[2]      	CE_ALIGN_ATTRIBUTE(8);
typedef _CE_INT32_         	ce_int4[4]      	CE_ALIGN_ATTRIBUTE(16);
typedef _CE_INT32_         	ce_int8[8]      	CE_ALIGN_ATTRIBUTE(32);
typedef _CE_INT32_         	ce_int16[16]    	CE_ALIGN_ATTRIBUTE(64);
typedef _CE_UINT32_        	ce_uint2[2]     	CE_ALIGN_ATTRIBUTE(8);
typedef _CE_UINT32_        	ce_uint4[4]     	CE_ALIGN_ATTRIBUTE(16);
typedef _CE_UINT32_        	ce_uint8[8]     	CE_ALIGN_ATTRIBUTE(32);
typedef _CE_UINT32_        	ce_uint16[16]   	CE_ALIGN_ATTRIBUTE(64);
typedef _CE_INT64_         	ce_long2[2]     	CE_ALIGN_ATTRIBUTE(16);
typedef _CE_INT64_         	ce_long4[4]     	CE_ALIGN_ATTRIBUTE(32);
typedef _CE_INT64_         	ce_long8[8]     	CE_ALIGN_ATTRIBUTE(64);
typedef _CE_INT64_         	ce_long16[16]   	CE_ALIGN_ATTRIBUTE(128);
typedef _CE_UINT64_        	ce_ulong2[2]    	CE_ALIGN_ATTRIBUTE(16);
typedef _CE_UINT64_        	ce_ulong4[4]    	CE_ALIGN_ATTRIBUTE(32);
typedef _CE_UINT64_        	ce_ulong8[8]    	CE_ALIGN_ATTRIBUTE(64);
typedef _CE_UINT64_        	ce_ulong16[16]  	CE_ALIGN_ATTRIBUTE(128);
typedef float              	ce_float2[2]    	CE_ALIGN_ATTRIBUTE(8);
typedef float              	ce_float4[4]    	CE_ALIGN_ATTRIBUTE(16);
typedef float              	ce_float8[8]    	CE_ALIGN_ATTRIBUTE(32);
typedef float              	ce_float16[16]  	CE_ALIGN_ATTRIBUTE(64);
typedef double             	ce_double2[2]   	CE_ALIGN_ATTRIBUTE(16);
typedef double             	ce_double4[4]   	CE_ALIGN_ATTRIBUTE(32);
typedef double             	ce_double8[8]   	CE_ALIGN_ATTRIBUTE(64);
typedef double             	ce_double16[16] 	CE_ALIGN_ATTRIBUTE(128);

/* Boolean Values *************************************************************/

#define CE_TRUE				(1)
#define CE_FALSE			(0)

/* Helper Macros **************************************************************/

#define CE_CONCAT(a,b)		a##b
#define CE_MAKE_NAME(a,b)	CE_CONCAT(a,b)

/* OS Dependent Headers *******************************************************/

#if defined(CE_PLATFORM_WINDOWS)
	#include <windows.h>
	#include <CL/cl.h>
#else
    #include <sys/stat.h>
    #include <sys/types.h>
    #include <sys/time.h>
    #include <stdlib.h>
    #include <string.h>
    #include <stdarg.h>

	#if defined(CE_PLATFORM_MACOSX)
		#include <OpenCL/opencl.h>
	#else
		#include <CL/cl.h>
	#endif
#endif

/* Standard Headers ***********************************************************/

#include <stdio.h>
#include <stddef.h>
#include <limits.h>
#include <float.h>
#include <math.h>
#include <ctype.h>

/******************************************************************************/

CE_EXTERN_C_END

#endif	/* __CE_PLATFORM_H__ */

