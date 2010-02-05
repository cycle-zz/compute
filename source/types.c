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

#include "types.h"

/**************************************************************************************************/

const char*
ceGetTypeString(ce_type type, ce_status* status)
{
	if(status)
		(*status) = CE_SUCCESS;
	
	switch(type)
	{
	case CE_TYPE_INVALID: 				return "ce_invalid";
	case CE_TYPE_BOOL: 					return "ce_bool";
	case CE_TYPE_CHAR: 					return "ce_char";
	case CE_TYPE_UCHAR: 				return "ce_uchar";
	case CE_TYPE_SHORT: 				return "ce_short";
	case CE_TYPE_USHORT: 				return "ce_ushort";
	case CE_TYPE_INT: 					return "ce_int";
	case CE_TYPE_UINT: 					return "ce_uint";
	case CE_TYPE_LONG: 					return "ce_long";
	case CE_TYPE_ULONG: 				return "ce_ulong";
	case CE_TYPE_FLOAT: 				return "ce_float";
	case CE_TYPE_DOUBLE: 				return "ce_double";
	case CE_TYPE_SYMBOL: 				return "ce_symbol";
	case CE_TYPE_QUEUE: 				return "ce_queue";
	case CE_TYPE_STACK: 				return "ce_stack";
	case CE_TYPE_MAP: 					return "ce_map";
	case CE_TYPE_REFERENCE: 			return "ce_reference";
	case CE_TYPE_DEVICE: 				return "ce_device";
	case CE_TYPE_MEM: 					return "ce_mem";
	case CE_TYPE_PROGRAM: 				return "ce_program";
	case CE_TYPE_KERNEL: 				return "ce_kernel";
	case CE_TYPE_LOG: 					return "ce_log";
	case CE_TYPE_MEMORY_INFO: 			return "ce_memory_info";
	case CE_TYPE_LOGGING_INFO: 			return "ce_logging_info";
	case CE_TYPE_PROFILING_INFO: 		return "ce_profiling_info";
	case CE_TYPE_UNKNOWN:
	default:
		{
			if(status)
				(*status) = CE_INVALID_TYPE;
		
			return "unknown type";
		}
	};

	if(status)
		(*status) = CE_INVALID_TYPE;
	
	return "unknown type";
}

const char*
ceGetStatusString(ce_status value, ce_status *status)
{

	if(status)
		(*status) = CE_SUCCESS;

	switch(value)
	{
	case CE_SUCCESS:					return "success";
	case CE_INVALID_TYPE:				return "invalid type";
	case CE_INVALID_DATA:				return "invalid data";
	case CE_INVALID_VALUE:				return "invalid value";
	case CE_INVALID_REFERENCE:			return "invalid reference";
	case CE_INVALID_SYMBOL:				return "invalid symbol";
	case CE_INVALID_STACK:				return "invalid stack";
	case CE_INVALID_QUEUE:				return "invalid queue";
	case CE_INVALID_MAP:				return "invalid map";
	case CE_INVALID_LOG:				return "invalid log";
	case CE_INVALID_SESSION:			return "invalid session";
	case CE_INVALID_CONTEXT:			return "invalid context";
	case CE_INVALID_HOST:				return "invalid host";
	case CE_INVALID_MEMORY_INFO:		return "invalid memory info";
	case CE_INVALID_LOGGING_INFO:		return "invalid logging info";
	case CE_INVALID_PROFILING_INFO:		return "invalid profiling info";
	case CE_OUT_OF_HOST_MEMORY:			return "out of host memory";
	case CE_TYPE_MISMATCH:				return "type mismatch";
	case CE_SIZE_MISMATCH:				return "size mismatch";
	case CE_INVALID_STATUS:				return "invalid status";
	default:
		{
			if(status)
				(*status) = CE_INVALID_STATUS;
			
			return "unknown status";
		}
	};

	if(status)
		(*status) = CE_INVALID_STATUS;
	
	return "unknown";

}

/**************************************************************************************************/

