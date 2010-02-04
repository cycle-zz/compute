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

#include "session.h"

/**************************************************************************************************/

static ce_session
ceCreateSession(void)
{
	ce_session_t* session = ceAllocate(NULL, sizeof(ce_session_t));
	memset(session, 0, sizeof(ce_session_t));
	return (ce_session)session;
}

static void 
ceNotifyCallback(const char *message, const void *handle, size_t handle_size, void *user_data)
{
	ce_session s = (ce_session)user_data;
	
	ceError(s, CL_INVALID_VALUE, "Notified of unexpected error: %s\n", message);
	if (handle_size > 0) 
	{
		int ii;
		ceWarning(s, "  %d bytes of vendor data.", handle_size);
		for (ii = 0; ii < handle_size; ii++) 
		{
			char c = ((const char *) handle)[ii];
			if (ii % 10 == 0) {
				ceWarning(s, "\n   %3d:", ii);
			}
			ceWarning(s, " 0x%02x %c", c, isprint(c) ? c : '.');
		}
		ceWarning(s, "\n");
	}
}

cl_bool
ceIsValidSession(ce_session session)
{
	ce_session_t* s = (ce_session_t*)session;
	if(!s)
		return CL_FALSE;

	if(!s->context)
		return CL_FALSE;
		
	if(s->units < 1)
		return CL_FALSE;

	return CL_TRUE;
}

cl_bool
ceIsValidDeviceIndexForSession(ce_session session, cl_uint device_index)
{
	ce_session_t* s = (ce_session_t*)session;

	if(!ceIsValidSession(session))
		return CL_FALSE;

	if(device_index >= s->units)
		return CL_FALSE;

	return CL_TRUE;
}

ce_session
ceCreateSessionForHost(void)
{
	return ceCreateSession();
}

ce_session
ceCreateSessionForDeviceType(cl_device_type device_type, cl_uint device_count)
{
    cl_int status = 0;
	cl_uint system_device_count = 0;
	cl_uint system_platform_count = 0;
	cl_device_id *device_list = 0;
	cl_platform_id platform = 0;
    size_t return_size = 0;
    ce_session_t *session = 0;

	status = clGetPlatformIDs(1, &platform, &system_platform_count); 
	if (status != CL_SUCCESS || system_platform_count < 1)
	{
		ceError(NULL, status, "Failed to locate platform!\n");
		return NULL;
	}

	status = clGetDeviceIDs(platform, device_type, 0, NULL, &system_device_count);
	if (status != CL_SUCCESS || system_device_count < 1)
	{
		ceError(NULL, status, "Failed to locate compute device!\n");
		return NULL;
	}
	
	device_count = system_device_count > device_count ? device_count : system_device_count;
	device_list = ceAllocate(NULL, sizeof(cl_device_id) * device_count);
	memset(device_list, 0, sizeof(cl_device_id) * device_count);
	
	status = clGetDeviceIDs(platform, device_type, device_count, device_list, &system_device_count);
	if (status != CL_SUCCESS)
	{
		ceError(NULL, status, "Failed to locate compute device!\n");
		return NULL;
	}

	session = (ce_session_t*)ceCreateSession();
	session->context = clCreateContext(NULL, device_count, device_list, ceNotifyCallback, session, &status);
    if (!session->context)
    {
        ceError(NULL, status, "Failed to create compute context!\n");
        return NULL;
    }

	session->platform = platform;
    status = clGetContextInfo(session->context, CL_CONTEXT_DEVICES, sizeof(cl_device_id) * device_count, device_list, &return_size);
    if(status != CL_SUCCESS || return_size < 1)
    {
        ceError(NULL, status, "Failed to retrieve compute devices for context!\n");
        return NULL;
    }
    
    session->devices = device_list;
    session->units = device_count;
    session->queues = ceAllocate((ce_session)session, sizeof(cl_command_queue) * session->units);
    if(session->queues)
    {
        ceError(NULL, status, "Failed to allocate device ids!\n");
        return NULL;
    }

    for(uint i = 0; i < session->units; i++)
    {
        cl_char vendor_name[256] = {0};
        cl_char device_name[256] = {0};

        status = clGetDeviceInfo(session->devices[i], CL_DEVICE_VENDOR, sizeof(vendor_name), vendor_name, &return_size);
        status|= clGetDeviceInfo(session->devices[i], CL_DEVICE_NAME, sizeof(device_name), device_name, &return_size);
        if (status != CL_SUCCESS)
        {
            ceError(NULL, status, "Failed to retrieve device info!\n");
            return CL_FALSE;
        }

        ceInfo(NULL, "Creating command queue for '%s' '%s'...\n", vendor_name, device_name);

        session->queues[i] = clCreateCommandQueue(session->context, session->devices[i], 0, &status);
        if (!session->queues[i] || status)
        {
            ceError(NULL, status, "Failed to create a command queue!\n");
            return CL_FALSE;
        }
    }

	session->programs = ceCreateMap((ce_session)session, CE_DEFAULT_MAP_SIZE);
	session->kernels = ceCreateMap((ce_session)session, CE_DEFAULT_MAP_SIZE);
	session->mem = ceCreateMap((ce_session)session, CE_DEFAULT_MAP_SIZE);

    return (ce_session)session;
}

void
ceReleaseSession(ce_session session)
{
	ce_session_t* s = (ce_session_t*)session;

	if(!ceIsValidSession(session))
		return;

    cl_uint i;
    if(s->context)
    {
        for(i = 0; i < s->units; i++)
            clFinish(s->queues[i]);
    }

	if(s->mem) 
		ceReleaseMap(s->mem);

	if(s->kernels)
	    ceReleaseMap(s->kernels);
    
    if(s->programs)
	    ceReleaseMap(s->programs);

    if(s->queues)
    {
        for(i = 0; i < s->units; i++)
            clReleaseCommandQueue(s->queues[i]);

        ceDeallocate(session, s->queues);
        s->queues = 0;
    }

    if(s->devices)
    {
        ceDeallocate(session, s->devices);
        s->devices = 0;
    }

    if(s->context)
    {
        clReleaseContext(s->context);
        s->context = 0;
    }

    s->units = 0;
    ceDeallocate(NULL, s);
    return;
}