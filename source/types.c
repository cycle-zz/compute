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
#include "status.h"

/**************************************************************************************************/

const char*
ceGetTypeString(ce_type type, ce_uint* status)
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

/**************************************************************************************************/

