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

#include "status.h"

/**************************************************************************************************/

const char*
scGetStatusString(
	sc_status value, sc_status *status)
{

	if(status)
		(*status) = SC_SUCCESS;

	switch(value)
	{
		case SC_SUCCESS:					return "success";
		case SC_INVALID_TYPE:				return "invalid type";
		case SC_INVALID_DATA:				return "invalid data";
		case SC_INVALID_VALUE:				return "invalid value";
		case SC_INVALID_REFERENCE:			return "invalid reference";
		case SC_INVALID_SYMBOL:				return "invalid symbol";
		case SC_INVALID_STACK:				return "invalid stack";
		case SC_INVALID_QUEUE:				return "invalid queue";
		case SC_INVALID_MAP:				return "invalid map";
		case SC_INVALID_LOG:				return "invalid log";
		case SC_INVALID_LOG_FILE:			return "invalid log file";
		case SC_INVALID_SESSION:			return "invalid session";
		case SC_INVALID_CONTEXT:			return "invalid context";
		case SC_INVALID_HOST:				return "invalid host";
		case SC_INVALID_DEVICE:				return "invalid device";
		case SC_INVALID_DEVICE_TYPE:		return "invalid device type";
		case SC_INVALID_DEVICE_INFO:		return "invalid device info";
		case SC_INVALID_COMMAND_QUEUE:		return "invalid command queue";
		case SC_INVALID_MEMORY_INFO:		return "invalid memory info";
		case SC_INVALID_LOGGING_INFO:		return "invalid logging info";
		case SC_INVALID_PROFILING_INFO:		return "invalid profiling info";
		case SC_DEVICE_NOT_AVAILABLE:		return "device not available";
		case SC_OUT_OF_HOST_MEMORY:			return "out of host memory";
		case SC_TYPE_MISMATCH:				return "type mismatch";
		case SC_SIZE_MISMATCH:				return "size mismatch";
		case SC_INVALID_STATUS:				return "invalid status";
		default:
		{
			if(status)
				(*status) = SC_INVALID_STATUS;

			return "unknown status";
		}
	};

	if(status)
		(*status) = SC_INVALID_STATUS;

	return "unknown";
}

sc_bool
scIsError(
	sc_status value)
{
	if(value == SC_SUCCESS)
		return SC_FALSE;
	return SC_TRUE;
}

/**************************************************************************************************/

