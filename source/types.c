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

#include "types.h"
#include "status.h"

/**************************************************************************************************/

const char*
scGetTypeString(sc_type type, sc_uint* status)
{
	if(status)
		(*status) = SC_SUCCESS;
	
	switch(type)
	{
		case SC_TYPE_INVALID: 				return "sc_invalid";
		case SC_TYPE_BOOL: 					return "sc_bool";
		case SC_TYPE_CHAR: 					return "sc_char";
		case SC_TYPE_UCHAR: 				return "sc_uchar";
		case SC_TYPE_SHORT: 				return "sc_short";
		case SC_TYPE_USHORT: 				return "sc_ushort";
		case SC_TYPE_INT: 					return "sc_int";
		case SC_TYPE_UINT: 					return "sc_uint";
		case SC_TYPE_LONG: 					return "sc_long";
		case SC_TYPE_ULONG: 				return "sc_ulong";
		case SC_TYPE_FLOAT: 				return "sc_float";
		case SC_TYPE_DOUBLE: 				return "sc_double";
		case SC_TYPE_SYMBOL: 				return "sc_symbol";
		case SC_TYPE_QUEUE: 				return "sc_queue";
		case SC_TYPE_STACK: 				return "sc_stack";
		case SC_TYPE_MAP: 					return "sc_map";
		case SC_TYPE_REFERENCE: 			return "sc_reference";
		case SC_TYPE_DEVICE: 				return "sc_device";
		case SC_TYPE_MEM: 					return "sc_mem";
		case SC_TYPE_PROGRAM: 				return "sc_program";
		case SC_TYPE_KERNEL: 				return "sc_kernel";
		case SC_TYPE_LOG: 					return "sc_log";
		case SC_TYPE_MEMORY_INFO: 			return "sc_memory_info";
		case SC_TYPE_LOGGING_INFO: 			return "sc_logging_info";
		case SC_TYPE_PROFILING_INFO: 		return "sc_profiling_info";
		case SC_TYPE_UNKNOWN:
		default:
		{
			if(status)
				(*status) = SC_INVALID_TYPE;
		
			return "unknown type";
		}
	};

	if(status)
		(*status) = SC_INVALID_TYPE;
	
	return "unknown type";
}

/**************************************************************************************************/

