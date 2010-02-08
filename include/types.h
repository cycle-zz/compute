/***************************************************************************************************

Scalable Compute Library - $SC_VERSION_TAG$ <$SC_ID_TAG$>

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

#ifndef __SC_TYPES_H__
#define __SC_TYPES_H__

/**************************************************************************************************/

#include "platform.h"

/**************************************************************************************************/

SC_EXTERN_C_BEGIN

/* Basic types ************************************************************************************/

#if (SC_PLATFORM_WINDOWS)

#define _SC_INT8_ 			signed   __int8
#define _SC_UINT8_			unsigned __int8
#define _SC_INT16_			signed   __int16
#define _SC_UINT16_			unsigned __int16
#define _SC_INT32_			signed   __int32
#define _SC_UINT32_			unsigned __int32
#define _SC_INT64_			signed   __int64
#define _SC_UINT64_			unsigned __int64

#else

#include <stdint.h>

#define _SC_INT8_         	int8_t
#define _SC_UINT8_			uint8_t
#define _SC_INT16_			int16_t
#define _SC_UINT16_			uint16_t
#define _SC_INT32_			int32_t
#define _SC_UINT32_			uint32_t
#define _SC_INT64_			int64_t
#define _SC_UINT64_			uint64_t

#endif

/* Scalar Types (aligned) *************************************************************************/

typedef _SC_INT8_      		sc_char;
typedef _SC_UINT8_         	sc_uchar;
typedef _SC_INT16_         	sc_short    		SC_ALIGN_ATTRIBUTE(2);
typedef _SC_UINT16_        	sc_ushort   		SC_ALIGN_ATTRIBUTE(2);
typedef _SC_INT32_         	sc_int      		SC_ALIGN_ATTRIBUTE(4);
typedef _SC_UINT32_        	sc_uint     		SC_ALIGN_ATTRIBUTE(4);
typedef _SC_INT64_         	sc_long     		SC_ALIGN_ATTRIBUTE(8);
typedef _SC_UINT64_        	sc_ulong    		SC_ALIGN_ATTRIBUTE(8);
typedef _SC_UINT16_        	sc_half     		SC_ALIGN_ATTRIBUTE(2);
typedef float              	sc_float    		SC_ALIGN_ATTRIBUTE(4);
typedef double             	sc_double   		SC_ALIGN_ATTRIBUTE(8);
typedef sc_uint				sc_bitfield;
typedef sc_ulong			sc_hashcode;
typedef sc_uint				sc_bool;

/* Vector Types (aligned) *************************************************************************/

typedef _SC_INT8_          	sc_char2[2]     	SC_ALIGN_ATTRIBUTE(2);
typedef _SC_INT8_          	sc_char4[4]     	SC_ALIGN_ATTRIBUTE(4);
typedef _SC_INT8_          	sc_char8[8]     	SC_ALIGN_ATTRIBUTE(8);
typedef _SC_INT8_          	sc_char16[16]   	SC_ALIGN_ATTRIBUTE(16);
typedef _SC_UINT8_         	sc_uchar2[2]    	SC_ALIGN_ATTRIBUTE(2);
typedef _SC_UINT8_         	sc_uchar4[4]    	SC_ALIGN_ATTRIBUTE(4);
typedef _SC_UINT8_         	sc_uchar8[8]    	SC_ALIGN_ATTRIBUTE(8);
typedef _SC_UINT8_         	sc_uchar16[16]  	SC_ALIGN_ATTRIBUTE(16);
typedef _SC_INT16_         	sc_short2[2]     	SC_ALIGN_ATTRIBUTE(4);
typedef _SC_INT16_         	sc_short4[4]     	SC_ALIGN_ATTRIBUTE(8);
typedef _SC_INT16_         	sc_short8[8]     	SC_ALIGN_ATTRIBUTE(16);
typedef _SC_INT16_         	sc_short16[16]   	SC_ALIGN_ATTRIBUTE(32);
typedef _SC_UINT16_        	sc_ushort2[2]    	SC_ALIGN_ATTRIBUTE(4);
typedef _SC_UINT16_        	sc_ushort4[4]    	SC_ALIGN_ATTRIBUTE(8);
typedef _SC_UINT16_        	sc_ushort8[8]    	SC_ALIGN_ATTRIBUTE(16);
typedef _SC_UINT16_        	sc_ushort16[16]  	SC_ALIGN_ATTRIBUTE(32);
typedef _SC_INT32_         	sc_int2[2]      	SC_ALIGN_ATTRIBUTE(8);
typedef _SC_INT32_         	sc_int4[4]      	SC_ALIGN_ATTRIBUTE(16);
typedef _SC_INT32_         	sc_int8[8]      	SC_ALIGN_ATTRIBUTE(32);
typedef _SC_INT32_         	sc_int16[16]    	SC_ALIGN_ATTRIBUTE(64);
typedef _SC_UINT32_        	sc_uint2[2]     	SC_ALIGN_ATTRIBUTE(8);
typedef _SC_UINT32_        	sc_uint4[4]     	SC_ALIGN_ATTRIBUTE(16);
typedef _SC_UINT32_        	sc_uint8[8]     	SC_ALIGN_ATTRIBUTE(32);
typedef _SC_UINT32_        	sc_uint16[16]   	SC_ALIGN_ATTRIBUTE(64);
typedef _SC_INT64_         	sc_long2[2]     	SC_ALIGN_ATTRIBUTE(16);
typedef _SC_INT64_         	sc_long4[4]     	SC_ALIGN_ATTRIBUTE(32);
typedef _SC_INT64_         	sc_long8[8]     	SC_ALIGN_ATTRIBUTE(64);
typedef _SC_INT64_         	sc_long16[16]   	SC_ALIGN_ATTRIBUTE(128);
typedef _SC_UINT64_        	sc_ulong2[2]    	SC_ALIGN_ATTRIBUTE(16);
typedef _SC_UINT64_        	sc_ulong4[4]    	SC_ALIGN_ATTRIBUTE(32);
typedef _SC_UINT64_        	sc_ulong8[8]    	SC_ALIGN_ATTRIBUTE(64);
typedef _SC_UINT64_        	sc_ulong16[16]  	SC_ALIGN_ATTRIBUTE(128);
typedef float              	sc_float2[2]    	SC_ALIGN_ATTRIBUTE(8);
typedef float              	sc_float4[4]    	SC_ALIGN_ATTRIBUTE(16);
typedef float              	sc_float8[8]    	SC_ALIGN_ATTRIBUTE(32);
typedef float              	sc_float16[16]  	SC_ALIGN_ATTRIBUTE(64);
typedef double             	sc_double2[2]   	SC_ALIGN_ATTRIBUTE(16);
typedef double             	sc_double4[4]   	SC_ALIGN_ATTRIBUTE(32);
typedef double             	sc_double8[8]   	SC_ALIGN_ATTRIBUTE(64);
typedef double             	sc_double16[16] 	SC_ALIGN_ATTRIBUTE(128);

/* Boolean Values *********************************************************************************/

#define SC_TRUE				(1)
#define SC_FALSE			(0)

/* Opaque Types ***********************************************************************************/

typedef struct _sc_session *    			sc_session;
typedef struct _sc_reference*	 			sc_reference;
typedef struct _sc_log* 		 			sc_log;
typedef struct _sc_map* 		 			sc_map;
typedef struct _sc_stack* 		 			sc_stack;
typedef struct _sc_symbol* 	 				sc_symbol;
typedef struct _sc_option* 	 				sc_option;
typedef struct _sc_value*					sc_value;
typedef struct _sc_session_memory_info* 	sc_session_memory_info;
typedef struct _sc_memory_info*  		 	sc_memory_info;
typedef struct _sc_logging_info*  		 	sc_logging_info;
typedef struct _sc_profiling_info*    	 	sc_profiling_info;

/* Type Enumerations *******************************************************************************/

typedef enum _sc_type {
	SC_TYPE_INVALID	= 0,
	SC_TYPE_BOOL,
	SC_TYPE_CHAR,
	SC_TYPE_UCHAR,
	SC_TYPE_SHORT,
	SC_TYPE_USHORT,
	SC_TYPE_INT,
	SC_TYPE_UINT,
	SC_TYPE_LONG,
	SC_TYPE_ULONG,
	SC_TYPE_FLOAT,
	SC_TYPE_DOUBLE,
	SC_TYPE_SYMBOL,
	SC_TYPE_QUEUE,
	SC_TYPE_STACK,
	SC_TYPE_MAP,
	SC_TYPE_REFERENCE,
	SC_TYPE_DEVICE,
	SC_TYPE_MEM,
	SC_TYPE_PROGRAM,
	SC_TYPE_KERNEL,
	SC_TYPE_LOG,
	SC_TYPE_MEMORY_INFO,
	SC_TYPE_LOGGING_INFO,
	SC_TYPE_PROFILING_INFO,
	SC_TYPE_UNKNOWN
} sc_type;

/* Type Methods ***********************************************************************************/

extern SC_API_EXPORT const char*
scGetTypeString(sc_type value, sc_uint* status);

/**************************************************************************************************/

SC_EXTERN_C_END

/**************************************************************************************************/

#endif /* __SC_TYPES_H__ */

