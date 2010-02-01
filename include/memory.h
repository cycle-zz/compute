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

#ifndef __CE_MEMORY_H__
#define __CE_MEMORY_H__

#include "compute.h"
 
/**************************************************************************************************/

CE_EXTERN_C_BEGIN

/**************************************************************************************************/

#define ceAllocate(session, size) 	ceAllocateHostMemory(session, size, __FILE__ , __LINE__)
#define ceDeallocate(session, ptr) 	ceDeallocateHostMemory(session, ptr)

extern CE_API_EXPORT 
void* ceAllocateHostMemory(ce_session session, size_t bytes, char* filename, unsigned int line);

extern CE_API_EXPORT  
void ceDeallocateHostMemory(ce_session session, void* ptr);

/**************************************************************************************************/

extern CE_API_EXPORT cl_int
ceEnableHostMemoryTracking(ce_session session);

extern CE_API_EXPORT cl_int
ceDisableHostMemoryTracking(ce_session session);

extern CE_API_EXPORT void 
ceLogHostMemoryInfo(ce_session session);

extern CE_API_EXPORT cl_int
ceEnableDeviceMemoryTracking(ce_session session, cl_device_id device);

extern CE_API_EXPORT cl_int
ceDisableDeviceMemoryTracking(ce_session session, cl_device_id device);

extern CE_API_EXPORT void 
ceLogDeviceMemoryInfo(ce_session session, cl_device_id device);

extern CE_API_EXPORT ce_reference
ceCreateReference(ce_session, void* ptr);

extern CE_API_EXPORT void
ceRetain(ce_session session, ce_reference reference);

extern CE_API_EXPORT void
ceRelease(ce_session session, ce_reference reference);

/**************************************************************************************************/

CE_EXTERN_C_END

/**************************************************************************************************/

#endif /* __CE_MEMORY_H__ */
