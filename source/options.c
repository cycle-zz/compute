/***************************************************************************************************

Compute Engine - $CE_VERSION_TAG$ <$CE_ID_TAG$>

Copyright (c) 2010, Derek Gerstmann <derek.gerstmann[|AT|]uwva->edu.au> 
The University of Western Australiva-> All rights reserved.

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

#include "options.h"
#include "symbol.h"

/**************************************************************************************************/

typedef struct _ce_value_t {
	ce_type 	type;
	union _ce_value_data {
		ce_bool 	as_bool;
		ce_char 	as_char;
		ce_uchar 	as_uchar;
		ce_short	as_short;
		ce_ushort 	as_ushort;
		ce_int 		as_int;
		ce_uint 	as_uint;
		ce_long 	as_long;
		ce_ulong 	as_ulong;
		ce_float 	as_float;
		ce_double 	as_double;
		ce_symbol	as_symbol;
	} data;
} ce_value_t;

typedef struct _ce_option_t {
	ce_session		session;
	ce_symbol		name;
	ce_symbol		usage;
	ce_bool			updated;
	ce_value		value;
} ce_option_t;

/**************************************************************************************************/

static ce_int
ParseOption(const char* arg, ce_option handle)
{
	if(!arg || !handle)
		return CE_INVALID_VALUE;

	ce_status status;
	ce_option_t* option = (ce_option_t*)handle;
	ce_value value = (ce_value)option->value;
	switch(ceGetValueType(value, &status))
	{
		case (CE_TYPE_BOOL):
			if(isdigit(arg[0]))
			{
				ceSetBoolValue(value, (atoi(arg) > 0) ? (CE_TRUE) : (CE_FALSE));
				option->updated = CE_TRUE;
				return CE_SUCCESS;
			}
			else if(strcmp(arg, "true") == 0)
			{
				ceSetBoolValue(value, CE_TRUE);
				option->updated = CE_TRUE;
				return CE_SUCCESS;
			}
			else if(strcmp(arg, "True") == 0)
			{
				ceSetBoolValue(value, CE_TRUE);
				option->updated = CE_TRUE;
				return CE_SUCCESS;
			}
			else if(strcmp(arg, "false") == 0)
			{
				ceSetBoolValue(value, CE_FALSE);
				option->updated = CE_TRUE;
				return CE_SUCCESS;
			}
			else if(strcmp(arg, "False") == 0)
			{
				ceSetBoolValue(value, CE_FALSE);
				option->updated = CE_TRUE;
				return CE_SUCCESS;
			}
			break;
		case (CE_TYPE_CHAR):
			if(isdigit(arg[0]))
			{
				ceSetCharValue(value, (ce_char)atoi(arg));
				option->updated = CE_TRUE;
				return CE_SUCCESS;
			}
			break;
		case (CE_TYPE_UCHAR):
			if(isdigit(arg[0]))
			{
				ceSetUCharValue(value, (ce_char)atoi(arg));
				option->updated = CE_TRUE;
				return CE_SUCCESS;
			}
			break;
		case (CE_TYPE_SHORT):
			if(isdigit(arg[0]))
			{
				ceSetShortValue(value, (ce_short)atoi(arg));
				option->updated = CE_TRUE;
				return CE_SUCCESS;
			}
			break;
		case (CE_TYPE_USHORT):
			if(isdigit(arg[0]))
			{
				ceSetUShortValue(value, (ce_short)atoi(arg));
				option->updated = CE_TRUE;
				return CE_SUCCESS;
			}
			break;
		case (CE_TYPE_INT):
			if(isdigit(arg[0]))
			{
				ceSetIntValue(value, (ce_int)atoi(arg));
				option->updated = CE_TRUE;
				return CE_SUCCESS;
			}
			break;
		case (CE_TYPE_UINT):
			if(isdigit(arg[0]))
			{
				ceSetUIntValue(value, (ce_uint)atoi(arg));
				option->updated = CE_TRUE;
				return CE_SUCCESS;
			}
			break;
		case (CE_TYPE_LONG):
			if(isdigit(arg[0]))
			{
				ceSetLongValue(value, (ce_long)atol(arg));
				option->updated = CE_TRUE;
				return CE_SUCCESS;
			}
			break;
		case (CE_TYPE_ULONG):
			if(isdigit(arg[0]))
			{
				ceSetULongValue(value, (ce_ulong)atol(arg));
				option->updated = CE_TRUE;
				return CE_SUCCESS;
			}
			break;
		case (CE_TYPE_FLOAT):
			{
				char* end = 0;
				ceSetFloatValue(value, (ce_float)strtof(arg, &end));
				if(end != arg)
				{
					option->updated = CE_TRUE;
					return CE_SUCCESS;
				}
			}
			break;
		case (CE_TYPE_DOUBLE):
			{
				char* end = 0;
				ceSetDoubleValue(value, (ce_double)strtod(arg, &end));
				if(end != arg)
				{
					option->updated = CE_TRUE;
					return CE_SUCCESS;
				}
			}
			break;
		case (CE_TYPE_SYMBOL):
			{
				ceSetSymbolValue(value, ceCreateSymbol(NULL, arg, strlen(arg)));
				option->updated = CE_TRUE;
				return CE_SUCCESS;
			}
			break;
		default:
			return CE_INVALID_VALUE;	
	}
	
	return CE_INVALID_VALUE;	
}

/**************************************************************************************************/

ce_option
ceCreateOption(
	ce_session session, const char* name, ce_type type, const char* usage)
{
	ce_option_t* option = ceAllocate(session, sizeof(ce_option_t));
	if(!option)
		return NULL;
		
	memset(option, 0, sizeof(ce_option_t));
	
	if(name)
		option->name = ceCreateSymbol(session, name, strlen(name));
		
	if(usage)
		option->usage = ceCreateSymbol(session, usage, strlen(usage));

	option->session = session;	
	option->value = ceCreateValue(session, type);
	return (ce_option)option;
}
	
ce_status
ceReleaseOption(
	ce_option handle)
{
	ce_option_t* option = (ce_option_t*)handle;
	if(!option)
		return CE_INVALID_VALUE;
		
	if(option->name)
		ceReleaseSymbol(option->name);

	if(option->usage)
		ceReleaseSymbol(option->usage);

	if(option->value)
		ceReleaseValue(option->value);


	return ceDeallocate(option->session, option);
	
}

const char*
ceGetOptionName(
	ce_option handle, ce_status* status)
{
	ce_option_t* option = (ce_option_t*)handle;
	if(!option)
	{
		if(status) (*status) = CE_INVALID_VALUE;
		return NULL;
	}
	return ceGetSymbolName(option->name);
}

const char*
ceGetOptionUsage(
	ce_option handle, ce_status* status)
{
	ce_option_t* option = (ce_option_t*)handle;
	if(!option)
	{
		if(status) (*status) = CE_INVALID_VALUE;
		return NULL;
	}
	return ceGetSymbolName(option->usage);
}

ce_value
ceGetOptionValue(
	ce_option handle, ce_status* status)
{
	ce_option_t* option = (ce_option_t*)handle;
	if(!option)
	{
		if(status) (*status) = CE_INVALID_VALUE;
		return 0;
	}
	return option->value;
}

/**************************************************************************************************/

ce_status
ceParseCommandLineOptions(
	ce_session session, ce_uint argc, const char **argv, ce_uint expected, ce_option* options)
{
    int i, o;
    cl_bool *found = ceAllocate(session, sizeof(ce_bool) * expected);
    if(!found)
    	return CE_OUT_OF_HOST_MEMORY;

	memset(found, CE_FALSE, sizeof(ce_bool) * expected);
	
    for (i = 0; i < argc && argv && argv[i]; i++) 
    {
		for(o = 0; o < expected; o++)
    	{
			if(found[o] == CE_TRUE)
    			continue;
    			
    		const char* start = argv[i];
    		if(start[0] != '-')
    			continue;

			const char* name = ceGetOptionName(options[o], NULL);

			ceDebug(session, "ceParseCommandLineOptions: Parsing '%s'\n", name);

    		const char* str = strstr(argv[i], name);
    		if(str && str - start <= 2)
    		{
				const char* assign = strstr(str, "=");
				if(assign)
				{
					ceDebug(session, "ceParseCommandLineOptions: Parsed '%s' as '%s'\n", name, &assign[1]);
	        	    
	        	    ParseOption(&assign[1], options[o]);
	        	    
	        	    ce_value value = ceGetOptionValue(options[o], NULL);
	        	    
					ceDebug(session, "ceParseCommandLineOptions: Parsed '%s' as '%s' -> '%s' (%s)\n", name, argv[i], 
						ceGetSymbolName(ceCreateSymbolFromValue(session, value, NULL) ),
						ceGetTypeString(ceGetValueType(value, NULL), NULL));
					
					found[o] = CE_TRUE;
				}
				else if(i + 1 < argc && argv[i + 1])
				{
					ceDebug(session, "ceParseCommandLineOptions: Parsed '%s' as '%s'\n", name, argv[i]);
	        	    
	        	    ParseOption(argv[i], options[o]);
					
					ce_value value = ceGetOptionValue(options[o], NULL);
	        	    
					ceDebug(session, "ceParseCommandLineOptions: Parsed '%s' as '%s' -> '%s' (%s)\n", name, argv[i], 
						ceGetSymbolName(ceCreateSymbolFromValue(session, value, NULL) ),
						ceGetTypeString(ceGetValueType(value, NULL), NULL));
					
					found[o] = CE_TRUE;
				}
				
				if(found[o] == CE_TRUE)
	        	    break;
    		}
    	}

		if(found[o] == CE_TRUE)
			continue;

    }

	ceDeallocate(session, found);
    return CE_SUCCESS;
}

/**************************************************************************************************/

