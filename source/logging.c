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

#include "logging.h"

/**************************************************************************************************/

#define CE_LOG_LEVEL_DEBUG					(1)
#define CE_LOG_LEVEL_TEST					(2)
#define CE_LOG_LEVEL_INFO					(3)
#define CE_LOG_LEVEL_WARNING				(4)
#define CE_LOG_LEVEL_CRITICAL				(5)
#define CE_LOG_LEVEL_ERROR					(6)

#define CE_LOG_PREFIX_TEST					"[test]"
#define CE_LOG_PREFIX_DEBUG					"[debug]"
#define CE_LOG_PREFIX_INFO					"[info]"
#define CE_LOG_PREFIX_WARNING				"[warning]"
#define CE_LOG_PREFIX_CRITICAL				"[critical]"
#define CE_LOG_PREFIX_ERROR					"[error]"

/**************************************************************************************************/

typedef struct _ce_logging_info_t {
	ce_session	session;
	char* 		filename;
	FILE*		stream;
	cl_uint 	flags;
} ce_logging_info_t;

/**************************************************************************************************/

static cl_bool
CloseLogFile(
	ce_logging_info_t* log)
{
	if (log && log->stream)
	{
		fclose (log->stream);
		log->stream = NULL;
		return CL_TRUE;
	}
	return CL_FALSE;
}

static cl_bool
OpenLogFile(
	ce_logging_info_t* log, const char* filename)
{
	static const char *replace = "w";
	static const char *append = "a+";
	const char *mode;
	
	if (log && log->stream)
		CloseLogFile(log);
	
	if (filename == NULL)
		filename = CE_DEFAULT_LOG_FILENAME;

	size_t length = strlen(filename) + 1;
	log->filename = ceAllocate(log->session, length * sizeof(char));
	snprintf(log->filename, length, "%s", filename);
	
	mode = (log->flags & CE_LOG_APPEND) ? append : replace;
	log->stream = fopen(log->filename, mode);
	if (log->stream == NULL)
	{
		CloseLogFile(log);
		return CL_FALSE;
	}
	return CL_TRUE;
}

static cl_int
LogHandler(
	ce_logging_info_t* log, cl_int level, cl_int status, const char* format, va_list arg_list)
{
	static char buffer[4096] = {0};
	const char *prefix = 0;
	const char *error = 0;
		
	switch(level)
	{
		case CE_LOG_LEVEL_DEBUG: 	 { prefix = CE_LOG_PREFIX_DEBUG; 		break; }
		case CE_LOG_LEVEL_TEST: 	 { prefix = CE_LOG_PREFIX_TEST; 		break; }
		case CE_LOG_LEVEL_INFO: 	 { prefix = CE_LOG_PREFIX_INFO; 		break; }
		case CE_LOG_LEVEL_WARNING:   { prefix = CE_LOG_PREFIX_WARNING; 		break; }
		case CE_LOG_LEVEL_CRITICAL:  { prefix = CE_LOG_PREFIX_CRITICAL; 	break; }
		case CE_LOG_LEVEL_ERROR: 	 { prefix = CE_LOG_PREFIX_ERROR; 		break; }
		default:	 				 { prefix = CE_LOG_PREFIX_INFO; 		break; }
	};
	
    if ((log->stream == NULL) && (log->flags & CE_LOG_FILE))
    {
    	OpenLogFile(log, log->filename);
    }

   	(void)vsnprintf(buffer, sizeof(buffer), format, arg_list);
    if (log->flags & CE_LOG_CONSOLE) 
    {
    	fprintf(stdout, "%s %s", prefix, buffer);
		if(status != CL_SUCCESS)
		{
			error = ceGetErrorString(status);
			fprintf(stdout, "%s %s", prefix, error);
		}
        fflush(stdout);
    }

    if (log->flags & CE_LOG_FILE)
    {
    	fprintf(log->stream, "%s %s", prefix, buffer);
		if(status != CL_SUCCESS)
		{
			error = ceGetErrorString(status);
			fprintf(log->stream, "%s %s", prefix, error);
		}
        fflush (log->stream);
    }
    
    return CL_SUCCESS;
}

static cl_int
SystemLogHandler(
	cl_int level, cl_int status, const char* format, va_list arg_list)
{
	static char buffer[4096] = {0};
	const char *prefix = 0;
	const char *error = 0;
		
	switch(level)
	{
#if defined(CE_DEBUG_BUILD)
		case CE_LOG_LEVEL_DEBUG: 	 { prefix = CE_LOG_PREFIX_DEBUG; 		break; }
#else
		case CE_LOG_LEVEL_DEBUG: 	 { return CL_SUCCESS; }
#endif
		case CE_LOG_LEVEL_TEST: 	 { prefix = CE_LOG_PREFIX_TEST; 		break; }
		case CE_LOG_LEVEL_INFO: 	 { prefix = CE_LOG_PREFIX_INFO; 		break; }
		case CE_LOG_LEVEL_WARNING: 	 { prefix = CE_LOG_PREFIX_WARNING; 		break; }
		case CE_LOG_LEVEL_CRITICAL:  { prefix = CE_LOG_PREFIX_CRITICAL; 	break; }
		case CE_LOG_LEVEL_ERROR: 	 { prefix = CE_LOG_PREFIX_ERROR; 		break; }
		default:	 				 { prefix = CE_LOG_PREFIX_INFO; 		break; }
	};
	
   	(void)vsnprintf(buffer, sizeof(buffer), format, arg_list);
	fprintf(stderr, "%s %s", prefix, buffer);
	if(status != CL_SUCCESS)
	{
		error = ceGetErrorString(status);
		fprintf(stderr, "%s %s", prefix, error);
	}
	fflush(stderr);
    
	if(level == CE_LOG_LEVEL_CRITICAL)
	{
#if defined(CE_PLATFORM_WINDOWS)
		__debugbreak();
#else
		abort();
#endif
	}
    return CL_SUCCESS;
}


/**************************************************************************************************/

cl_int
ceEnableLogging(
	ce_session session, cl_bitfield flags, const char* filename)
{
	cl_int status;
	ce_session_t* s = (ce_session_t*)session;
	ce_logging_info_t* log = s ? (ce_logging_info_t*)s->logging : 0;
	if(s == NULL || log == NULL)
		return CL_INVALID_VALUE;
	
	log->flags = flags;
	
	status = OpenLogFile(log, filename);
	return status;
}

cl_int
ceDisableLogging(
	ce_session session)
{
	ce_session_t* s = (ce_session_t*)session;
	ce_logging_info_t* log = s ? (ce_logging_info_t*)s->logging : 0;
	if(s == NULL || log == NULL)
		return CL_INVALID_VALUE;
	
	CloseLogFile(log);
	
	if(log->filename)
	{
		ceDeallocate(session, log->filename);
		log->filename = NULL;
	}
	
	return 0;
}

cl_int 
ceSetLoggingMode(
	ce_session session, cl_bitfield flags, const char* filename)
{
	ce_session_t* s = (ce_session_t*)session;
	ce_logging_info_t* log = s ? (ce_logging_info_t*)s->logging : 0;
	if(s == NULL || log == NULL)
		return CL_INVALID_VALUE;
		
	cl_bool append = log->flags & CE_LOG_APPEND;
	
	log->flags = flags;
	
	if(log->filename != filename || (log->flags & CE_LOG_APPEND) != append)
		return OpenLogFile(log, filename);
	
	return CL_SUCCESS;
}

cl_int 
ceDebug(
	ce_session session, const char* format, ...)
{
	ce_session_t* s = (ce_session_t*)session;
	ce_logging_info_t* log = s ? (ce_logging_info_t*)s->logging : 0;

    cl_int status;
    va_list arg_list;
    va_start(arg_list, format);
	if(s == NULL || log == NULL)
	{
	    status = SystemLogHandler(CE_LOG_LEVEL_DEBUG, 0, format, arg_list);
	}
	else
	{
	    status = LogHandler(log, CE_LOG_LEVEL_DEBUG, 0, format, arg_list);
    }
    va_end(arg_list);
    return status;
}

cl_int 
ceTest(
	ce_session session, const char* format, ...)
{
	ce_session_t* s = (ce_session_t*)session;
	ce_logging_info_t* log = s ? (ce_logging_info_t*)s->logging : 0;

    cl_int status;
    va_list arg_list;
    va_start(arg_list, format);
	if(s == NULL || log == NULL)
	{
	    status = SystemLogHandler(CE_LOG_LEVEL_TEST, 0, format, arg_list);
	}
	else
	{
	    status = LogHandler(log, CE_LOG_LEVEL_TEST, 0, format, arg_list);
    }
    va_end(arg_list);
    return status;
}

cl_int 
ceInfo(
	ce_session session, const char* format, ...)
{
	ce_session_t* s = (ce_session_t*)session;
	ce_logging_info_t* log = s ? (ce_logging_info_t*)s->logging : 0;

    cl_int status;
    va_list arg_list;
    va_start(arg_list, format);
	if(s == NULL || log == NULL)
	{
	    status = SystemLogHandler(CE_LOG_LEVEL_INFO, 0, format, arg_list);
	}
	else
	{
	    status = LogHandler(log, CE_LOG_LEVEL_INFO, 0, format, arg_list);
    }
    va_end(arg_list);
    return status;
}

cl_int 
ceWarning(
	ce_session session, const char* format, ...)
{
	ce_session_t* s = (ce_session_t*)session;
	ce_logging_info_t* log = s ? (ce_logging_info_t*)s->logging : 0;
	
	cl_int status;
    va_list arg_list;
    va_start(arg_list, format);
	if(s == NULL || log == NULL)
	{
	    status = SystemLogHandler(CE_LOG_LEVEL_WARNING, 0, format, arg_list);
	}
	else
	{
	    status = LogHandler(log, CE_LOG_LEVEL_WARNING, 0, format, arg_list);
    }
    va_end(arg_list);
    return status;
}

cl_int 
ceCritical(
	ce_session session, const char* format, ...)
{
	ce_session_t* s = (ce_session_t*)session;
	ce_logging_info_t* log = s ? (ce_logging_info_t*)s->logging : 0;
	
	cl_int status;
    va_list arg_list;
    va_start(arg_list, format);
	if(s == NULL || log == NULL)
	{
	    status = SystemLogHandler(CE_LOG_LEVEL_CRITICAL, 0, format, arg_list);
	}
	else
	{
	    status = LogHandler(log, CE_LOG_LEVEL_CRITICAL, 0, format, arg_list);
    }
    va_end(arg_list);
    return status;
}

cl_int 
ceError(
	ce_session session, cl_int error, const char* format, ...)
{
	ce_session_t* s = (ce_session_t*)session;
	ce_logging_info_t* log = s ? (ce_logging_info_t*)s->logging : 0;

	cl_int status;
    va_list arg_list;
    va_start(arg_list, format);
	if(s == NULL || log == NULL)
	{
	    status = SystemLogHandler(CE_LOG_LEVEL_ERROR, error, format, arg_list);
	}
	else
	{
	    status = LogHandler(log, CE_LOG_LEVEL_ERROR, error, format, arg_list);
	}
    va_end(arg_list);
	return status;
}

const char * 
ceGetErrorString(
	cl_int status)
{
    const char* error_string = 0;

    switch(status) {
    case(CL_SUCCESS):
        break;
    case(CL_DEVICE_NOT_FOUND):
        error_string = "Device not found";
        break;
    case(CL_DEVICE_NOT_AVAILABLE):
        error_string = "Device not available";
        break;
    case(CL_DEVICE_COMPILER_NOT_AVAILABLE):
        error_string = "Device compiler not available";
        break;
    case(CL_MEM_OBJECT_ALLOCATION_FAILURE):
        error_string = "Memory object allocation failure";
        break;
    case(CL_OUT_OF_RESOURCES):
        error_string = "Out of resources";
        break;
    case(CL_OUT_OF_HOST_MEMORY):
        error_string = "Out of host memory";
        break;
    case(CL_PROFILING_INFO_NOT_AVAILABLE):
        error_string = "Profiling information not available";
        break;
    case(CL_MEM_COPY_OVERLAP):
        error_string = "Overlap detected in memory copy operation";
        break;
    case(CL_IMAGE_FORMAT_MISMATCH):
        error_string = "Image format mismatch detected";
        break;
    case(CL_IMAGE_FORMAT_NOT_SUPPORTED):
        error_string = "Image format not supported";
        break;
    case(CL_INVALID_VALUE):
        error_string = "Invalid value";
        break;
    case(CL_INVALID_DEVICE_TYPE):
        error_string = "Invalid device type";
        break;
    case(CL_INVALID_DEVICE):
        error_string = "Invalid device";
        break;
    case(CL_INVALID_CONTEXT):
        error_string = "Invalid context";
        break;
    case(CL_INVALID_QUEUE_PROPERTIES):
        error_string = "Invalid queue properties";
        break;
    case(CL_INVALID_COMMAND_QUEUE):
        error_string = "Invalid command queue";
        break;
    case(CL_INVALID_HOST_PTR):
        error_string = "Invalid host pointer address";
        break;
    case(CL_INVALID_MEM_OBJECT):
        error_string = "Invalid memory object";
        break;
    case(CL_INVALID_IMAGE_FORMAT_DESCRIPTOR):
        error_string = "Invalid image format descriptor";
        break;
    case(CL_INVALID_IMAGE_SIZE):
        error_string = "Invalid image size";
        break;
    case(CL_INVALID_SAMPLER):
        error_string = "Invalid sampler";
        break;
    case(CL_INVALID_BINARY):
        error_string = "Invalid binary";
        break;
    case(CL_INVALID_BUILD_OPTIONS):
        error_string = "Invalid build options";
        break;
    case(CL_INVALID_PROGRAM):
        error_string = "Invalid program object";
        break;
    case(CL_INVALID_PROGRAM_EXECUTABLE):
        error_string = "Invalid program executable";
        break;
    case(CL_INVALID_KERNEL_NAME):
        error_string = "Invalid kernel name";
        break;
    case(CL_INVALID_KERNEL):
        error_string = "Invalid kernel object";
        break;
    case(CL_INVALID_ARG_INDEX):
        error_string = "Invalid index for kernel argument";
        break;
    case(CL_INVALID_ARG_VALUE):
        error_string = "Invalid value for kernel argument";
        break;
    case(CL_INVALID_ARG_SIZE):
        error_string = "Invalid size for kernel argument";
        break;
    case(CL_INVALID_KERNEL_ARGS):
        error_string = "Invalid kernel arguments";
        break;
    case(CL_INVALID_WORK_DIMENSION):
        error_string = "Invalid work dimension";
        break;
    case(CL_INVALID_WORK_GROUP_SIZE):
        error_string = "Invalid work group size";
        break;
    case(CL_INVALID_GLOBAL_OFFSET):
        error_string = "Invalid global offset";
        break;
    case(CL_INVALID_EVENT_WAIT_LIST):
        error_string = "Invalid event wait list";
        break;
    case(CL_INVALID_EVENT):
        error_string = "Invalid event";
        break;
    case(CL_INVALID_OPERATION):
        error_string = "Invalid operation";
        break;
    case(CL_INVALID_GL_OBJECT):
        error_string = "Invalid OpenGL object";
        break;
    case(CL_INVALID_BUFFER_SIZE):
        error_string = "Invalid buffer size";
        break;
    case(CL_INVALID_MIP_LEVEL):
        error_string = "Invalid mip level";
        break;
    default:
        error_string = "Unknown error";
        break;
    };

    return error_string;
}

/**************************************************************************************************/


