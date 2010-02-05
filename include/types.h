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

#ifndef __CE_TYPES_H__
#define __CE_TYPES_H__

#include "platform.h"

/**************************************************************************************************/

CE_EXTERN_C_BEGIN

/* Basic types ************************************************************************************/

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

/* Scalar Types (aligned) *************************************************************************/

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

/* Vector Types (aligned) *************************************************************************/

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

/* Boolean Values *********************************************************************************/

#define CE_TRUE				(1)
#define CE_FALSE			(0)

/* Opaque Types ***********************************************************************************/

typedef struct _ce_session *    			ce_session;
typedef struct _ce_reference*	 			ce_reference;
typedef struct _ce_log* 		 			ce_log;
typedef struct _ce_map* 		 			ce_map;
typedef struct _ce_stack* 		 			ce_stack;
typedef struct _ce_symbol* 	 				ce_symbol;
typedef struct _ce_option* 	 				ce_option;
typedef struct _ce_value*					ce_value;
typedef struct _ce_session_memory_info* 	ce_session_memory_info;
typedef struct _ce_memory_info*  		 	ce_memory_info;
typedef struct _ce_logging_info*  		 	ce_logging_info;
typedef struct _ce_profiling_info*    	 	ce_profiling_info;

/* Type Enumerations *******************************************************************************/

typedef enum _ce_type {
	CE_TYPE_INVALID	= 0,
	CE_TYPE_BOOL,
	CE_TYPE_CHAR,
	CE_TYPE_UCHAR,
	CE_TYPE_SHORT,
	CE_TYPE_USHORT,
	CE_TYPE_INT,
	CE_TYPE_UINT,
	CE_TYPE_LONG,
	CE_TYPE_ULONG,
	CE_TYPE_FLOAT,
	CE_TYPE_DOUBLE,
	CE_TYPE_SYMBOL,
	CE_TYPE_QUEUE,
	CE_TYPE_STACK,
	CE_TYPE_MAP,
	CE_TYPE_REFERENCE,
	CE_TYPE_DEVICE,
	CE_TYPE_MEM,
	CE_TYPE_PROGRAM,
	CE_TYPE_KERNEL,
	CE_TYPE_LOG,
	CE_TYPE_MEMORY_INFO,
	CE_TYPE_LOGGING_INFO,
	CE_TYPE_PROFILING_INFO,
	CE_TYPE_UNKNOWN
} ce_type;

/* Status Enumerations ****************************************************************************/

typedef enum _ce_status {
	CE_SUCCESS,
	CE_INVALID_TYPE,
	CE_INVALID_DATA,
	CE_INVALID_VALUE,
	CE_INVALID_REFERENCE,
	CE_INVALID_SYMBOL,
	CE_INVALID_STACK,
	CE_INVALID_QUEUE,
	CE_INVALID_MAP,
	CE_INVALID_LOG,
	CE_INVALID_SESSION,
	CE_INVALID_CONTEXT,
	CE_INVALID_HOST,
	CE_INVALID_MEMORY_INFO,
	CE_INVALID_LOGGING_INFO,
	CE_INVALID_PROFILING_INFO,
	CE_OUT_OF_HOST_MEMORY,
	CE_TYPE_MISMATCH,
	CE_SIZE_MISMATCH,
	CE_INVALID_STATUS,
} ce_status;
	

/**************************************************************************************************/

extern CE_API_EXPORT const char*
ceGetTypeString(ce_type value, ce_status* status);

extern CE_API_EXPORT const char*
ceGetStatusString(ce_status value, ce_status* status);

/**************************************************************************************************/

CE_EXTERN_C_END

/**************************************************************************************************/

#endif /* __CE_TYPES_H__ */

