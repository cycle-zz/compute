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

#ifndef __SC_STATUS_H__
#define __SC_STATUS_H__

/**************************************************************************************************/

#include "platform.h"
#include "types.h"

/**************************************************************************************************/

SC_EXTERN_C_BEGIN

/* Status Enumerations ****************************************************************************/

typedef enum _sc_status {
	SC_SUCCESS,
	SC_INVALID_TYPE,
	SC_INVALID_DATA,
	SC_INVALID_VALUE,
	SC_INVALID_REFERENCE,
	SC_INVALID_SYMBOL,
	SC_INVALID_STACK,
	SC_INVALID_QUEUE,
	SC_INVALID_MAP,
	SC_INVALID_LOG,
	SC_INVALID_LOG_FILE,
	SC_INVALID_PLATFORM,
	SC_INVALID_SESSION,
	SC_INVALID_CONTEXT,
	SC_INVALID_HOST,
	SC_INVALID_DEVICE,
	SC_INVALID_DEVICE_TYPE,
	SC_INVALID_DEVICE_INFO,
	SC_INVALID_COMMAND_QUEUE,
	SC_INVALID_MEMORY_INFO,
	SC_INVALID_LOGGING_INFO,
	SC_INVALID_PROFILING_INFO,
	SC_DEVICE_NOT_AVAILABLE,
	SC_OUT_OF_HOST_MEMORY,
	SC_TYPE_MISMATCH,
	SC_SIZE_MISMATCH,
	SC_INVALID_STATUS,
} sc_status;

/* Status Methods *********************************************************************************/

extern SC_API_EXPORT const char*
scGetStatusString(sc_status value, sc_status* status);

extern SC_API_EXPORT sc_bool
scIsError(sc_status value);

/**************************************************************************************************/

SC_EXTERN_C_END

/**************************************************************************************************/

#endif /* __SC_STATUS_H__ */

