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

#ifndef __CE_STATUS_H__
#define __CE_STATUS_H__

/**************************************************************************************************/

#include "platform.h"
#include "types.h"

/**************************************************************************************************/

CE_EXTERN_C_BEGIN

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

/* Status Methods *********************************************************************************/

extern CE_API_EXPORT const char*
ceGetStatusString(ce_status value, ce_status* status);

extern CE_API_EXPORT ce_bool
ceIsError(ce_status value);

/**************************************************************************************************/

CE_EXTERN_C_END

/**************************************************************************************************/

#endif /* __CE_STATUS_H__ */

