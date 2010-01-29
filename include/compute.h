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

#ifndef __COMPUTE_H__
#define __COMPUTE_H__

#include "platform.h"

/**************************************************************************************************/

CE_EXTERN_C_BEGIN

/**************************************************************************************************/

typedef enum _ce_type_t {
	CE_TYPE_INVALID,
	CE_TYPE_DEVICE,
	CE_TYPE_QUEUE,
	CE_TYPE_MEM,
	CE_TYPE_PROGRAM,
	CE_TYPE_KERNEL,
	CE_TYPE_MEMORY_INFO,
	CE_TYPE_LOGGING_INFO,
	CE_TYPE_PROFILING_INFO,
	CE_TYPE_UNKNOWN
} ce_type_t;

/**************************************************************************************************/

typedef struct _ce_session *    			ce_session;
typedef struct _ce_log* 		 			ce_log;
typedef struct _ce_map* 		 			ce_map;
typedef struct _ce_symbol* 	 				ce_symbol;
typedef struct _ce_session_memory_info* 	ce_session_memory_info;
typedef struct _ce_memory_info*  		 	ce_memory_info;
typedef struct _ce_logging_info*  		 	ce_logging_info;
typedef struct _ce_profiling_info*    	 	ce_profiling_info;

/* Memory *****************************************************************************************/

#include "memory.h"

/* Logging ****************************************************************************************/

#include "logging.h"

/* Symbol *****************************************************************************************/

#include "symbol.h"
#include "map.h"

/* Session ****************************************************************************************/

#include "session.h"

/** Profiling *************************************************************************************/

#include "profiling.h"

/**************************************************************************************************/

CE_EXTERN_C_END

#endif /* __COMPUTE_H__ */
