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

#ifndef __CE_SESSION_H__
#define __CE_SESSION_H__

/**************************************************************************************************/

#include "compute.h"

/**************************************************************************************************/

CE_EXTERN_C_BEGIN

/**************************************************************************************************/

#define CE_DEFAULT_SESSION	(NULL)

typedef struct _ce_session_t {
    cl_platform_id											platform;
    cl_context 												context;
	cl_uint													units;
	cl_device_id *											devices;
	cl_command_queue *										queues;
    ce_map													programs;
	ce_map													kernels;
    ce_map													mem;
	ce_session_memory_info									memory;
	ce_profiling_info										profiling;
	ce_logging_info											logging;
	ce_bool													valid;
} ce_session_t;


/**************************************************************************************************/

extern CE_API_EXPORT ce_session
ceCreateSessionForHost(void);

extern CE_API_EXPORT ce_session
ceCreateSessionForDeviceType(cl_device_type device_type, cl_uint device_count);

extern CE_API_EXPORT ce_status
ceReleaseSession(ce_session session);

extern CE_API_EXPORT cl_bool
ceIsValidSession(ce_session session);

extern CE_API_EXPORT cl_bool
ceIsValidDeviceIndexForSession(ce_session session, cl_uint device_index);

/**************************************************************************************************/

CE_EXTERN_C_END

#endif /* __CE_SESSION_H__ */
