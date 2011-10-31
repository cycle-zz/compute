/***************************************************************************************************

Scalable Compute Library - $SC_VERSION_TAG$ <$SC_ID_TAG$>

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

typedef struct _sc_value_t {
	sc_type 	type;
	union _sc_value_data {
		sc_bool 	as_bool;
		sc_char 	as_char;
		sc_uchar 	as_uchar;
		sc_short	as_short;
		sc_ushort 	as_ushort;
		sc_int 		as_int;
		sc_uint 	as_uint;
		sc_long 	as_long;
		sc_ulong 	as_ulong;
		sc_float 	as_float;
		sc_double 	as_double;
		sc_symbol	as_symbol;
	} data;
} sc_value_t;

typedef struct _sc_option_t {
	sc_session		session;
	sc_symbol		name;
	sc_symbol		usage;
	sc_bool			updated;
	sc_value		value;
} sc_option_t;

/**************************************************************************************************/

static sc_int
ParseOption(const char* arg, sc_option handle)
{
	if(!arg || !handle)
		return SC_INVALID_VALUE;

	sc_status status;
	sc_option_t* option = (sc_option_t*)handle;
	sc_value value = (sc_value)option->value;
	switch(scGetValueType(value, &status))
	{
		case (SC_TYPE_BOOL):
			if(isdigit(arg[0]))
			{
				scSetBoolValue(value, (atoi(arg) > 0) ? (SC_TRUE) : (SC_FALSE));
				option->updated = SC_TRUE;
				return SC_SUCCESS;
			}
			else if(strcmp(arg, "true") == 0)
			{
				scSetBoolValue(value, SC_TRUE);
				option->updated = SC_TRUE;
				return SC_SUCCESS;
			}
			else if(strcmp(arg, "True") == 0)
			{
				scSetBoolValue(value, SC_TRUE);
				option->updated = SC_TRUE;
				return SC_SUCCESS;
			}
			else if(strcmp(arg, "false") == 0)
			{
				scSetBoolValue(value, SC_FALSE);
				option->updated = SC_TRUE;
				return SC_SUCCESS;
			}
			else if(strcmp(arg, "False") == 0)
			{
				scSetBoolValue(value, SC_FALSE);
				option->updated = SC_TRUE;
				return SC_SUCCESS;
			}
			break;
		case (SC_TYPE_CHAR):
			if(isdigit(arg[0]))
			{
				scSetCharValue(value, (sc_char)atoi(arg));
				option->updated = SC_TRUE;
				return SC_SUCCESS;
			}
			break;
		case (SC_TYPE_UCHAR):
			if(isdigit(arg[0]))
			{
				scSetUCharValue(value, (sc_char)atoi(arg));
				option->updated = SC_TRUE;
				return SC_SUCCESS;
			}
			break;
		case (SC_TYPE_SHORT):
			if(isdigit(arg[0]))
			{
				scSetShortValue(value, (sc_short)atoi(arg));
				option->updated = SC_TRUE;
				return SC_SUCCESS;
			}
			break;
		case (SC_TYPE_USHORT):
			if(isdigit(arg[0]))
			{
				scSetUShortValue(value, (sc_short)atoi(arg));
				option->updated = SC_TRUE;
				return SC_SUCCESS;
			}
			break;
		case (SC_TYPE_INT):
			if(isdigit(arg[0]))
			{
				scSetIntValue(value, (sc_int)atoi(arg));
				option->updated = SC_TRUE;
				return SC_SUCCESS;
			}
			break;
		case (SC_TYPE_UINT):
			if(isdigit(arg[0]))
			{
				scSetUIntValue(value, (sc_uint)atoi(arg));
				option->updated = SC_TRUE;
				return SC_SUCCESS;
			}
			break;
		case (SC_TYPE_LONG):
			if(isdigit(arg[0]))
			{
				scSetLongValue(value, (sc_long)atol(arg));
				option->updated = SC_TRUE;
				return SC_SUCCESS;
			}
			break;
		case (SC_TYPE_ULONG):
			if(isdigit(arg[0]))
			{
				scSetULongValue(value, (sc_ulong)atol(arg));
				option->updated = SC_TRUE;
				return SC_SUCCESS;
			}
			break;
		case (SC_TYPE_FLOAT):
			{
				char* end = 0;
				scSetFloatValue(value, (sc_float)strtof(arg, &end));
				if(end != arg)
				{
					option->updated = SC_TRUE;
					return SC_SUCCESS;
				}
			}
			break;
		case (SC_TYPE_DOUBLE):
			{
				char* end = 0;
				scSetDoubleValue(value, (sc_double)strtod(arg, &end));
				if(end != arg)
				{
					option->updated = SC_TRUE;
					return SC_SUCCESS;
				}
			}
			break;
		case (SC_TYPE_SYMBOL):
			{
				scSetSymbolValue(value, scCreateSymbol(NULL, arg, strlen(arg)));
				option->updated = SC_TRUE;
				return SC_SUCCESS;
			}
			break;
		default:
			return SC_INVALID_VALUE;
	}

	return SC_INVALID_VALUE;
}

/**************************************************************************************************/

sc_option
scCreateOption(
	sc_session session, const char* name, sc_type type, const char* usage)
{
	sc_option_t* option = scAllocate(session, sizeof(sc_option_t));
	if(!option)
		return NULL;

	memset(option, 0, sizeof(sc_option_t));

	if(name)
		option->name = scCreateSymbol(session, name, strlen(name));

	if(usage)
		option->usage = scCreateSymbol(session, usage, strlen(usage));

	option->session = session;
	option->value = scCreateValue(session, type);
	return (sc_option)option;
}

sc_status
scReleaseOption(
	sc_option handle)
{
	sc_option_t* option = (sc_option_t*)handle;
	if(!option)
		return SC_INVALID_VALUE;

	if(option->name)
		scReleaseSymbol(option->name);

	if(option->usage)
		scReleaseSymbol(option->usage);

	if(option->value)
		scReleaseValue(option->value);


	return scDeallocate(option->session, option);

}

const char*
scGetOptionName(
	sc_option handle, sc_status* status)
{
	sc_option_t* option = (sc_option_t*)handle;
	if(!option)
	{
		if(status) (*status) = SC_INVALID_VALUE;
		return NULL;
	}
	return scGetSymbolName(option->name);
}

const char*
scGetOptionUsage(
	sc_option handle, sc_status* status)
{
	sc_option_t* option = (sc_option_t*)handle;
	if(!option)
	{
		if(status) (*status) = SC_INVALID_VALUE;
		return NULL;
	}
	return scGetSymbolName(option->usage);
}

sc_value
scGetOptionValue(
	sc_option handle, sc_status* status)
{
	sc_option_t* option = (sc_option_t*)handle;
	if(!option)
	{
		if(status) (*status) = SC_INVALID_VALUE;
		return 0;
	}
	return option->value;
}

/**************************************************************************************************/

sc_status
scParseCommandLineOptions(
	sc_session session, sc_uint argc, const char **argv, sc_uint expected, sc_option* options)
{
    int i, o;
    cl_bool *found = scAllocate(session, sizeof(sc_bool) * expected);
    if(!found)
    	return SC_OUT_OF_HOST_MEMORY;

	memset(found, SC_FALSE, sizeof(sc_bool) * expected);

    for (i = 0; i < argc && argv && argv[i]; i++)
    {
		for(o = 0; o < expected; o++)
    	{
			if(found[o] == SC_TRUE)
    			continue;

    		const char* start = argv[i];
    		if(start[0] != '-')
    			continue;

			const char* name = scGetOptionName(options[o], NULL);

			scDebug(session, "ceParseCommandLineOptions: Parsing '%s'\n", name);

    		const char* str = strstr(argv[i], name);
    		if(str && str - start <= 2)
    		{
				const char* assign = strstr(str, "=");
				if(assign)
				{
					scDebug(session, "ceParseCommandLineOptions: Parsed '%s' as '%s'\n", name, &assign[1]);

	        	    ParseOption(&assign[1], options[o]);

	        	    sc_value value = scGetOptionValue(options[o], NULL);

					scDebug(session, "ceParseCommandLineOptions: Parsed '%s' as '%s' -> '%s' (%s)\n", name, argv[i],
						scGetSymbolName(scCreateSymbolFromValue(session, value, NULL) ),
						scGetTypeString(scGetValueType(value, NULL), NULL));

					found[o] = SC_TRUE;
				}
				else if(i + 1 < argc && argv[i + 1])
				{
					scDebug(session, "ceParseCommandLineOptions: Parsed '%s' as '%s'\n", name, argv[i]);

	        	    ParseOption(argv[i], options[o]);

					sc_value value = scGetOptionValue(options[o], NULL);

					scDebug(session, "ceParseCommandLineOptions: Parsed '%s' as '%s' -> '%s' (%s)\n", name, argv[i],
						scGetSymbolName(scCreateSymbolFromValue(session, value, NULL) ),
						scGetTypeString(scGetValueType(value, NULL), NULL));

					found[o] = SC_TRUE;
				}

				if(found[o] == SC_TRUE)
	        	    break;
    		}
    	}

		if(found[o] == SC_TRUE)
			continue;

    }

	scDeallocate(session, found);
    return SC_SUCCESS;
}

/**************************************************************************************************/

