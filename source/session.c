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

#include "session.h"

/**************************************************************************************************/

typedef struct _sc_session_t {
    cl_platform_id											platform;
    cl_context 												context;
	sc_uint													units;
	cl_device_id *											devices;
	cl_command_queue *										queues;
    sc_map													programs;
	sc_map													kernels;
    sc_map													mem;
	sc_memory_info											memory;
	sc_profiling_info										profiling;
	sc_logging_info											logging;
	sc_bool													valid;
} sc_session_t;

/**************************************************************************************************/

static void 
NotifyCallback(
	const char *message, const void *handle, size_t handle_size, void *user_data)
{
	sc_session s = (sc_session)user_data;
	
	scError(s, CL_INVALID_VALUE, "Notified of unexpected error: %s\n", message);
	if (handle_size > 0) 
	{
		int ii;
		scWarning(s, "  %d bytes of vendor data.", handle_size);
		for (ii = 0; ii < handle_size; ii++) 
		{
			char c = ((const char *) handle)[ii];
			if (ii % 10 == 0) {
				scWarning(s, "\n   %3d:", ii);
			}
			scWarning(s, " 0x%02x %c", c, isprint(c) ? c : '.');
		}
		scWarning(s, "\n");
	}
}


static cl_device_type
GetOpenCLDeviceType(
	sc_device_type type, sc_status* status)
{
	if(status)
		(*status) = SC_SUCCESS;
	
	switch(type)
	{
		case SC_DEVICE_TYPE_DEFAULT:		return CL_DEVICE_TYPE_DEFAULT;
		case SC_DEVICE_TYPE_ALL:			return CL_DEVICE_TYPE_ALL;
		case SC_DEVICE_TYPE_GPU:			return CL_DEVICE_TYPE_GPU;
		case SC_DEVICE_TYPE_CPU:			return CL_DEVICE_TYPE_CPU;
		case SC_DEVICE_TYPE_ACCELERATOR:	return CL_DEVICE_TYPE_ACCELERATOR;
		default:
		{
			if(status)
				(*status) = SC_INVALID_TYPE;
		
			return 0;
		}
	};

	if(status)
		(*status) = SC_INVALID_TYPE;
	
	return 0;
}

static sc_status
SetupOpenCLSessionForDeviceType(
	sc_session handle,
	sc_device_type device_type, 
	sc_uint device_count)
{
    sc_status status = 0;
    cl_int system_status = 0;
    cl_device_type system_device_type = 0;
	cl_uint system_device_count = 0;
	cl_uint system_platform_count = 0;
	cl_device_id *system_device_list = 0;
	cl_platform_id system_platform = 0;
    size_t return_size = 0;
    
    sc_session_t *session = (sc_session_t*)handle;
	
	system_status = clGetPlatformIDs(1, &system_platform, &system_platform_count); 
	if (system_status != CL_SUCCESS || system_platform_count < 1)
	{
		scError(NULL, SC_INVALID_PLATFORM, "Failed to locate platform!\n");
		return SC_INVALID_PLATFORM;
	}

	system_device_type = GetOpenCLDeviceType(device_type, &status);
	if (status != CL_SUCCESS)
	{
		scError(NULL, SC_INVALID_DEVICE_TYPE, "Invalid compute device type specified!\n");
		return SC_INVALID_DEVICE_TYPE;
	}
	
	system_status = clGetDeviceIDs(system_platform, system_device_type, 0, NULL, &system_device_count);
	if (system_status != CL_SUCCESS || system_device_count < 1)
	{
		scError(NULL, SC_DEVICE_NOT_AVAILABLE, "Failed to locate compute device!\n");
		return SC_DEVICE_NOT_AVAILABLE;
	}
	
	if(device_count)
		system_device_count = system_device_count > device_count ? device_count : system_device_count;
	
	system_device_list = scAllocate(NULL, sizeof(cl_device_id) * device_count);
	memset(system_device_list, 0, sizeof(cl_device_id) * device_count);
	
	system_status = clGetDeviceIDs(system_platform, system_device_type, system_device_count, system_device_list, &system_device_count);
	if (system_status != CL_SUCCESS)
	{
		scError(NULL, SC_DEVICE_NOT_AVAILABLE, "Failed to locate compute device!\n");
		return SC_DEVICE_NOT_AVAILABLE;
	}
	
	session->context = clCreateContext(NULL, system_device_count, system_device_list, NotifyCallback, session, &system_status);
    if (!session->context)
    {
        scError(handle, SC_INVALID_CONTEXT, "Failed to create compute context!\n");
        return SC_INVALID_CONTEXT;
    }

	session->platform = system_platform;
    system_status = clGetContextInfo(session->context, CL_CONTEXT_DEVICES, sizeof(cl_device_id) * system_device_count, system_device_list, &return_size);
    if(system_status != CL_SUCCESS || return_size < 1)
    {
        scError(handle, SC_INVALID_DEVICE, "Failed to retrieve compute devices for context!\n");
        return SC_INVALID_DEVICE;
    }
    
    session->devices = system_device_list;
    session->units = system_device_count;
    session->queues = scAllocate(handle, sizeof(cl_command_queue) * session->units);
    if(!session->queues)
    {
        scError(handle, SC_OUT_OF_HOST_MEMORY, "Failed to allocate command queues!\n");
        return SC_OUT_OF_HOST_MEMORY;
    }

    for(sc_uint i = 0; i < session->units; i++)
    {
        cl_char vendor_name[256] = {0};
        cl_char device_name[256] = {0};

        system_status = clGetDeviceInfo(session->devices[i], CL_DEVICE_VENDOR, sizeof(vendor_name), vendor_name, &return_size);
        system_status|= clGetDeviceInfo(session->devices[i], CL_DEVICE_NAME, sizeof(device_name), device_name, &return_size);
        if (system_status != CL_SUCCESS)
        {
            scError(NULL, SC_INVALID_DEVICE_INFO, "Failed to retrieve device info!\n");
            return SC_INVALID_DEVICE_INFO;
        }

        scInfo(handle, "Adding device '%s' '%s' to compute session.\n", vendor_name, device_name);

        session->queues[i] = clCreateCommandQueue(session->context, session->devices[i], 0, &system_status);
        if (!session->queues[i])
        {
            scError(handle, SC_INVALID_COMMAND_QUEUE, "Failed to create a command queue!\n");
            return SC_INVALID_COMMAND_QUEUE;
        }
    }

	session->programs = scCreateMap(handle, SC_DEFAULT_MAP_SIZE);
	session->kernels = scCreateMap(handle, SC_DEFAULT_MAP_SIZE);
	session->mem = scCreateMap(handle, SC_DEFAULT_MAP_SIZE);
	session->valid = SC_TRUE;

    return SC_SUCCESS;
}

/**************************************************************************************************/

static sc_session
scCreateSession(void)
{
	sc_session_t* session = scAllocate(NULL, sizeof(sc_session_t));
	memset(session, 0, sizeof(sc_session_t));
	return (sc_session)session;
}

cl_bool
scIsValidSession(
	sc_session handle)
{
	sc_session_t* session = (sc_session_t*)handle;
	if(session && session->valid)
		return CL_TRUE;

	return CL_FALSE;
}

cl_bool
scIsValidDeviceIndexForSession(
	sc_session handle, sc_uint device_index)
{
	sc_session_t* session = (sc_session_t*)handle;

	if(!scIsValidSession(handle))
		return CL_FALSE;

	if(device_index >= session->units)
		return CL_FALSE;

	return CL_TRUE;
}

sc_session
scCreateSessionForHost(void)
{
	sc_session handle = scCreateSession();
	sc_session_t* session = (sc_session_t*)handle;
	if(session)
		session->valid = SC_TRUE;

    scInfo(handle, "Adding host to compute session.\n");

	return (sc_session)session;
}

sc_session
scCreateSessionForDeviceType(
	sc_device_type device_type, 
	sc_uint device_count, 
	sc_status *status)
{
	sc_status result;
	sc_session handle = scCreateSession();
	result = SetupOpenCLSessionForDeviceType(handle, device_type, device_count);
	if(status) (*status) = result;
	return handle;
}

sc_status
scReleaseSession(
	sc_session handle)
{
	sc_session_t* session = (sc_session_t*)handle;

	if(!scIsValidSession(handle))
		return SC_INVALID_SESSION;

    sc_uint i;
    if(session->context)
    {
        for(i = 0; i < session->units; i++)
            clFinish(session->queues[i]);
    }

	if(session->mem) 
		scReleaseMap(session->mem);

	if(session->kernels)
	    scReleaseMap(session->kernels);
    
    if(session->programs)
	    scReleaseMap(session->programs);

    if(session->queues)
    {
        for(i = 0; i < session->units; i++)
            clReleaseCommandQueue(session->queues[i]);

        scDeallocate(handle, session->queues);
        session->queues = 0;
    }

    if(session->devices)
    {
        scDeallocate(handle, session->devices);
        session->devices = 0;
    }

    if(session->context)
    {
        clReleaseContext(session->context);
        session->context = 0;
    }

    session->units = 0;
    scDeallocate(handle, session);
    return SC_SUCCESS;
}

sc_logging_info
scGetLoggingInfo(
	sc_session handle, sc_status *status)
{
	sc_session_t* session = (sc_session_t*)handle;

	if(!scIsValidSession(handle))
	{
		if(status)
			(*status) = SC_INVALID_SESSION;	

		return NULL;
	}

	return session->logging;
}

sc_memory_info
scGetMemoryInfo(
	sc_session handle, sc_status *status)
{
	sc_session_t* session = (sc_session_t*)handle;

	if(!scIsValidSession(handle))
	{
		if(status)
			(*status) = SC_INVALID_SESSION;	

		return NULL;
	}

	return session->memory;
}

sc_profiling_info
scGetProfilingInfo(
	sc_session handle, sc_status *status)
{
	sc_session_t* session = (sc_session_t*)handle;

	if(!scIsValidSession(handle))
	{
		if(status)
			(*status) = SC_INVALID_SESSION;	

		return NULL;
	}

	return session->profiling;
}
