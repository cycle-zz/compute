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

#ifndef __CE_LOGGING_H__
#define __CE_LOGGING_H__
 
#include "compute.h"
 
/**************************************************************************************************/

CE_EXTERN_C_BEGIN

/**************************************************************************************************/

#define CE_LOG_NONE										(1 << 0)
#define CE_LOG_FILE					                	(1 << 1)
#define CE_LOG_CONSOLE                            		(1 << 2)
#define CE_LOG_ALL                         				(1 << 3)
#define CE_LOG_REPLACE                    				(1 << 4)
#define CE_LOG_APPEND                        			(1 << 5)

#define CE_LOG_SRC_INFO									"file %s, line %i\n\n" , __FILE__ , __LINE__
#define CE_DEFAULT_LOG_FILENAME 						"ce_session.log"
#define CE_LOG_SEPARATOR "------------------------------------------------------------------------"

/**************************************************************************************************/

extern CE_API_EXPORT cl_int
ceEnableLogging(ce_session session, cl_bitfield flags, const char* filename);

extern CE_API_EXPORT cl_int
ceDisableLogging(ce_session session);

extern CE_API_EXPORT cl_int 
ceSetLoggingMode(ce_session session, cl_bitfield flags,  const char* filename);

extern CE_API_EXPORT cl_int 
ceTest(ce_session session, const char* format, ...);

extern CE_API_EXPORT cl_int 
ceDebug(ce_session session, const char* format, ...);

extern CE_API_EXPORT cl_int 
ceWarning(ce_session session, const char* format, ...);

extern CE_API_EXPORT cl_int 
ceInfo(ce_session session, const char* format, ...);

extern CE_API_EXPORT cl_int 
ceCritical(ce_session session, const char* format, ...);

extern CE_API_EXPORT cl_int 
ceError(ce_session session, cl_int status, const char* format, ...);

extern CE_API_EXPORT const char* 
ceGetErrorString(cl_int status);

/**************************************************************************************************/

CE_EXTERN_C_END

#endif	/* __CE_LOG_H__ */
