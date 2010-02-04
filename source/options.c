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
ParseOption(const char* arg, ce_option_t *option)
{
	if(!arg || !option)
		return CE_INVALID_VALUE;

	ce_value_t* value = (ce_value_t*)option->value;
	switch(value->type)
	{
		case (CE_TYPE_BOOL):
			if(isdigit(arg[0]))
			{
				value->data.as_bool = (atoi(arg) > 0) ? (CE_TRUE) : (CE_FALSE);
				option->updated = CE_TRUE;
				return CE_SUCCESS;
			}
			else if(strcmp(arg, "true") == 0)
			{
				value->data.as_bool = CE_TRUE;
				option->updated = CE_TRUE;
				return CE_SUCCESS;
			}
			else if(strcmp(arg, "True") == 0)
			{
				value->data.as_bool = CE_TRUE;
				option->updated = CE_TRUE;
				return CE_SUCCESS;
			}
			else if(strcmp(arg, "false") == 0)
			{
				value->data.as_bool = CE_FALSE;
				option->updated = CE_TRUE;
				return CE_SUCCESS;
			}
			else if(strcmp(arg, "False") == 0)
			{
				value->data.as_bool = CE_FALSE;
				option->updated = CE_TRUE;
				return CE_SUCCESS;
			}
			break;
		case (CE_TYPE_CHAR):
			if(isdigit(arg[0]))
			{
				value->data.as_char = (ce_char)atoi(arg);
				option->updated = CE_TRUE;
				return CE_SUCCESS;
			}
			break;
		case (CE_TYPE_UCHAR):
			if(isdigit(arg[0]))
			{
				value->data.as_uchar = (ce_uchar)atoi(arg);
				option->updated = CE_TRUE;
				return CE_SUCCESS;
			}
			break;
		case (CE_TYPE_SHORT):
			if(isdigit(arg[0]))
			{
				value->data.as_short = (ce_short)atoi(arg);
				option->updated = CE_TRUE;
				return CE_SUCCESS;
			}
			break;
		case (CE_TYPE_USHORT):
			if(isdigit(arg[0]))
			{
				value->data.as_ushort = (ce_ushort)atoi(arg);
				option->updated = CE_TRUE;
				return CE_SUCCESS;
			}
			break;
		case (CE_TYPE_INT):
			if(isdigit(arg[0]))
			{
				value->data.as_int = (ce_int)atoi(arg);
				option->updated = CE_TRUE;
				return CE_SUCCESS;
			}
			break;
		case (CE_TYPE_UINT):
			if(isdigit(arg[0]))
			{
				value->data.as_uint = (ce_uint)atoi(arg);
				option->updated = CE_TRUE;
				return CE_SUCCESS;
			}
			break;
		case (CE_TYPE_LONG):
			if(isdigit(arg[0]))
			{
				value->data.as_long = (ce_long)atol(arg);
				option->updated = CE_TRUE;
				return CE_SUCCESS;
			}
			break;
		case (CE_TYPE_ULONG):
			if(isdigit(arg[0]))
			{
				value->data.as_ulong = (ce_ulong)atol(arg);
				option->updated = CE_TRUE;
				return CE_SUCCESS;
			}
			break;
		case (CE_TYPE_FLOAT):
			{
				char* end = 0;
				value->data.as_float = (ce_float)strtof(arg, &end);
				ceDebug(CE_DEFAULT_SESSION, "'%s' -> '%f'\n", arg, value->data.as_float);
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
				value->data.as_double = strtod(arg, &end);
				if(end != arg)
				{
					option->updated = CE_TRUE;
					return CE_SUCCESS;
				}
			}
			break;
		case (CE_TYPE_SYMBOL):
			{
				value->data.as_symbol = ceCreateSymbol(NULL, arg, strlen(arg));
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
	{
		ce_value_t* value = (ce_value_t*)value;
		if(value->type == CE_TYPE_SYMBOL && value->data.as_symbol)
			ceReleaseSymbol(value->data.as_symbol);

	}		
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

ce_value
ceCreateValue(
	ce_session session, ce_type type)
{
	ce_value_t* value = ceAllocate(session, sizeof(ce_value_t));
	if(!value)
		return NULL;
		
	memset(value, 0, sizeof(ce_value_t));
	value->type = type;
	return (ce_value)value;
}

ce_value
ceCreateBoolValue(
	ce_session session, ce_bool data)
{
	ce_value value = ceCreateValue(session, CE_TYPE_BOOL);
	ceSetBoolValue(value, data);
	return value;
}

ce_value
ceCreateCharValue(
	ce_session session, ce_char data)
{
	ce_value value = ceCreateValue(session, CE_TYPE_CHAR);
	ceSetCharValue(value, data);
	return value;
}

ce_value
ceCreateUCharValue(
	ce_session session, ce_uchar data)
{
	ce_value value = ceCreateValue(session, CE_TYPE_UCHAR);
	ceSetCharValue(value, data);
	return value;
}

ce_value
ceCreateShortValue(
	ce_session session, ce_short data)
{
	ce_value value = ceCreateValue(session, CE_TYPE_SHORT);
	ceSetShortValue(value, data);
	return value;
}

ce_value
ceCreateUShortValue(
	ce_session session, ce_ushort data)
{
	ce_value value = ceCreateValue(session, CE_TYPE_USHORT);
	ceSetUShortValue(value, data);
	return value;
}

ce_value
ceCreateIntValue(
	ce_session session, ce_int data)
{
	ce_value value = ceCreateValue(session, CE_TYPE_INT);
	ceSetIntValue(value, data);
	return value;
}

ce_value
ceCreateUIntValue(
	ce_session session, ce_uint data)
{
	ce_value value = ceCreateValue(session, CE_TYPE_UINT);
	ceSetUIntValue(value, data);
	return value;
}

ce_value
ceCreateLongValue(
	ce_session session, ce_long data)
{
	ce_value value = ceCreateValue(session, CE_TYPE_LONG);
	ceSetLongValue(value, data);
	return value;
}

ce_value
ceCreateULongValue(
	ce_session session, ce_ulong data)
{
	ce_value value = ceCreateValue(session, CE_TYPE_ULONG);
	ceSetULongValue(value, data);
	return value;
}

ce_value
ceCreateFloatValue(
	ce_session session, ce_float data)
{
	ce_value value = ceCreateValue(session, CE_TYPE_FLOAT);
	ceSetFloatValue(value, data);
	return value;
}

ce_value
ceCreateDoubleValue(
	ce_session session, ce_double data)
{
	ce_value value = ceCreateValue(session, CE_TYPE_DOUBLE);
	ceSetDoubleValue(value, data);
	return value;
}

ce_value
ceCreateSymbolValue(
	ce_session session, ce_symbol data)
{
	ce_value value = ceCreateValue(session, CE_TYPE_SYMBOL);
	ceSetSymbolValue(value, data);
	return value;
}

ce_status
ceReleaseValue(
	ce_value handle)
{
	ce_value_t* value = (ce_value_t*)handle;
	if(!value)
		return CE_INVALID_VALUE;
		
	if(value->type == CE_TYPE_SYMBOL && value->data.as_symbol)
		ceReleaseSymbol(value->data.as_symbol);

	return ceDeallocate(CE_DEFAULT_SESSION, handle);
}

ce_status
ceSetBoolValue(
	ce_value handle, ce_bool data)
{
	ce_value_t* value = (ce_value_t*)handle;
	if(!value) 
		return CE_INVALID_VALUE;
		
	if(value->type != CE_TYPE_BOOL)
		return CE_TYPE_MISMATCH;
		
	value->data.as_bool = data;
	return CE_SUCCESS;
}

ce_bool
ceGetBoolValue(
	ce_value handle, ce_status *status)
{
	ce_value_t* value = (ce_value_t*)handle;
	if(!value)
	{
		if(status) (*status) = CE_INVALID_VALUE;
		return -1;
	}
	
	if(value->type != CE_TYPE_BOOL)
	{
		if(status) (*status) = CE_TYPE_MISMATCH;
		return -1;
	}
	
	return value->data.as_bool;
}

ce_status
ceSetCharValue(
	ce_value handle, ce_char data)
{
	ce_value_t* value = (ce_value_t*)handle;
	if(!value) 
		return CE_INVALID_VALUE;
		
	if(value->type != CE_TYPE_CHAR)
		return CE_TYPE_MISMATCH;
		
	value->data.as_char = data;
	return CE_SUCCESS;
}

ce_char
ceGetCharValue(
	ce_value handle, ce_status *status)
{
	ce_value_t* value = (ce_value_t*)handle;
	if(!value)
	{
		if(status) (*status) = CE_INVALID_VALUE;
		return -1;
	}
	
	if(value->type != CE_TYPE_CHAR)
	{
		if(status) (*status) = CE_TYPE_MISMATCH;
		return -1;
	}
	
	return value->data.as_char;
}

ce_status
ceSetUCharValue(
	ce_value handle, ce_uchar data)
{
	ce_value_t* value = (ce_value_t*)handle;
	if(!value) 
		return CE_INVALID_VALUE;
		
	if(value->type != CE_TYPE_UCHAR)
		return CE_TYPE_MISMATCH;
		
	value->data.as_uchar = data;
	return CE_SUCCESS;
}

ce_uchar
ceGetUCharValue(
	ce_value handle, ce_status *status)
{
	ce_value_t* value = (ce_value_t*)handle;
	if(!value)
	{
		if(status) (*status) = CE_INVALID_VALUE;
		return -1;
	}
	
	if(value->type != CE_TYPE_UCHAR)
	{
		if(status) (*status) = CE_TYPE_MISMATCH;
		return -1;
	}

	return value->data.as_uchar;
}


ce_status
ceSetShortValue(
	ce_value handle, ce_short data)
{
	ce_value_t* value = (ce_value_t*)handle;
	if(!value) 
		return CE_INVALID_VALUE;
		
	if(value->type != CE_TYPE_SHORT)
		return CE_TYPE_MISMATCH;
		
	value->data.as_short = data;
	return CE_SUCCESS;
}

ce_short
ceGetShortValue(
	ce_value handle, ce_status *status)
{
	ce_value_t* value = (ce_value_t*)handle;
	if(!value)
	{
		if(status) (*status) = CE_INVALID_VALUE;
		return -1;
	}
	
	if(value->type != CE_TYPE_SHORT)
	{
		if(status) (*status) = CE_TYPE_MISMATCH;
		return -1;
	}
	
	return value->data.as_short;
}

ce_status
ceSetUShortValue(
	ce_value handle, ce_ushort data)
{
	ce_value_t* value = (ce_value_t*)handle;
	if(!value) 
		return CE_INVALID_VALUE;
		
	if(value->type != CE_TYPE_USHORT)
		return CE_TYPE_MISMATCH;
		
	value->data.as_ushort = data;
	return CE_SUCCESS;
}

ce_ushort
ceGetUShortValue(
	ce_value handle, ce_status *status)
{
	ce_value_t* value = (ce_value_t*)handle;
	if(!value)
	{
		if(status) (*status) = CE_INVALID_VALUE;
		return -1;
	}
	
	if(value->type != CE_TYPE_USHORT)
	{
		if(status) (*status) = CE_TYPE_MISMATCH;
		return -1;
	}
	
	return value->data.as_ushort;
}

ce_status
ceSetIntValue(
	ce_value handle, ce_int data)
{
	ce_value_t* value = (ce_value_t*)handle;
	if(!value) 
		return CE_INVALID_VALUE;
		
	if(value->type != CE_TYPE_INT)
		return CE_TYPE_MISMATCH;
		
	value->data.as_int = data;
	return CE_SUCCESS;
}

ce_int
ceGetIntValue(	
	ce_value handle, ce_status *status)
{
	ce_value_t* value = (ce_value_t*)handle;
	if(!value)
	{
		if(status) (*status) = CE_INVALID_VALUE;
		return -1;
	}
	
	if(value->type != CE_TYPE_INT)
	{
		if(status) (*status) = CE_TYPE_MISMATCH;
		return -1;
	}
	
	return value->data.as_int;
}


ce_status
ceSetUIntValue(
	ce_value handle, ce_uint data)
{
	ce_value_t* value = (ce_value_t*)handle;
	if(!value) 
		return CE_INVALID_VALUE;
		
	if(value->type != CE_TYPE_UINT)
		return CE_TYPE_MISMATCH;
		
	value->data.as_uint = data;
	return CE_SUCCESS;
}

ce_uint
ceGetUIntValue(
	ce_value handle, ce_status *status)
{
	ce_value_t* value = (ce_value_t*)handle;
	if(!value)
	{
		if(status) (*status) = CE_INVALID_VALUE;
		return -1;
	}
	
	if(value->type != CE_TYPE_UINT)
	{
		if(status) (*status) = CE_TYPE_MISMATCH;
		return -1;
	}
	
	return value->data.as_uint;
}

ce_status
ceSetLongValue(
	ce_value handle, ce_long data)
{
	ce_value_t* value = (ce_value_t*)handle;
	if(!value) 
		return CE_INVALID_VALUE;
		
	if(value->type != CE_TYPE_LONG)
		return CE_TYPE_MISMATCH;
		
	value->data.as_long = data;
	return CE_SUCCESS;
}

ce_long
ceGetLongValue(
	ce_value handle, ce_status *status)
{
	ce_value_t* value = (ce_value_t*)handle;
	if(!value)
	{
		if(status) (*status) = CE_INVALID_VALUE;
		return -1;
	}
	
	if(value->type != CE_TYPE_LONG)
	{
		if(status) (*status) = CE_TYPE_MISMATCH;
		return -1;
	}
	
	return value->data.as_long;
}

ce_status
ceSetULongValue(
	ce_value handle, ce_ulong data)
{
	ce_value_t* value = (ce_value_t*)handle;
	if(!value) 
		return CE_INVALID_VALUE;
		
	if(value->type != CE_TYPE_ULONG)
		return CE_TYPE_MISMATCH;
		
	value->data.as_ulong = data;
	return CE_SUCCESS;
}

ce_ulong
ceGetULongValue(
	ce_value handle, ce_status *status)
{
	ce_value_t* value = (ce_value_t*)handle;
	if(!value)
	{
		if(status) (*status) = CE_INVALID_VALUE;
		return -1;
	}
	
	if(value->type != CE_TYPE_ULONG)
	{
		if(status) (*status) = CE_TYPE_MISMATCH;
		return -1;
	}
	
	return value->data.as_ulong;
}

ce_status
ceSetFloatValue(
	ce_value handle, ce_float data)
{
	ce_value_t* value = (ce_value_t*)handle;
	if(!value) 
		return CE_INVALID_VALUE;
		
	if(value->type != CE_TYPE_FLOAT)
		return CE_TYPE_MISMATCH;
		
	value->data.as_float = data;
	return CE_SUCCESS;
}

ce_float
ceGetFloatValue(
	ce_value handle, ce_status *status)
{
	ce_value_t* value = (ce_value_t*)handle;
	if(!value)
	{
		if(status) (*status) = CE_INVALID_VALUE;
		return -1;
	}
	
	if(value->type != CE_TYPE_FLOAT)
	{
		if(status) (*status) = CE_TYPE_MISMATCH;
		return -1;
	}
	
	return value->data.as_float;
}

ce_status
ceSetDoubleValue(
	ce_value handle, ce_double data)
{
	ce_value_t* value = (ce_value_t*)handle;
	if(!value) 
		return CE_INVALID_VALUE;
		
	if(value->type != CE_TYPE_DOUBLE)
		return CE_TYPE_MISMATCH;
		
	value->data.as_double = data;
	return CE_SUCCESS;
}

ce_double
ceGetDoubleValue(
	ce_value handle, ce_status *status)
{
	ce_value_t* value = (ce_value_t*)handle;
	if(!value)
	{
		if(status) (*status) = CE_INVALID_VALUE;
		return -1;
	}
	
	if(value->type != CE_TYPE_DOUBLE)
	{
		if(status) (*status) = CE_TYPE_MISMATCH;
		return -1;
	}
	
	return value->data.as_double;
}


ce_status
ceSetSymbolValue(
	ce_value handle, ce_symbol data)
{
	ce_value_t* value = (ce_value_t*)handle;
	if(!value) 
		return CE_INVALID_VALUE;
		
	if(value->type != CE_TYPE_SYMBOL)
		return CE_TYPE_MISMATCH;

	if(value->data.as_symbol)
		ceReleaseSymbol(value->data.as_symbol);
		
	value->data.as_symbol = ceCreateSymbol(CE_DEFAULT_SESSION, ceGetSymbolName(data), ceGetSymbolLength(data));
	return CE_SUCCESS;
}

ce_symbol
ceGetSymbolValue(
	ce_value handle, ce_status *status)
{
	ce_value_t* value = (ce_value_t*)handle;
	if(!value)
	{
		if(status) (*status) = CE_INVALID_VALUE;
		return 0;
	}
	
	if(value->type != CE_TYPE_SYMBOL)
	{
		if(status) (*status) = CE_TYPE_MISMATCH;
		return 0;
	}

	return value->data.as_symbol;
}

ce_type
ceGetValueType(
	ce_value handle, ce_status* status)
{
	ce_value_t* value = (ce_value_t*)handle;
	if(!value)
	{
		if(status) (*status) = CE_INVALID_VALUE;
		return CE_INVALID_TYPE;
	}
	
	return value->type;
}

ce_symbol
ceCreateSymbolFromValue(
	ce_session session, ce_value handle, ce_status* status)
{
	char buffer[256] = {0};
	ce_value_t* value = (ce_value_t*)handle;
	if(!value)
	{
		if(status) (*status) = CE_INVALID_VALUE;
		return NULL;
	}
	
	int count = 0;
	switch(value->type)
	{
		case (CE_TYPE_BOOL):
		{
			count = snprintf(buffer, sizeof(buffer), "%s", value->data.as_bool == CE_TRUE ? "true" : "false");
			break;
		}
		case (CE_TYPE_CHAR):
		{
			count = snprintf(buffer, sizeof(buffer), "%c", value->data.as_char);
			break;
		}
		case (CE_TYPE_UCHAR):
		{
			count = snprintf(buffer, sizeof(buffer), "%c", value->data.as_uchar);
			break;
		}
		case (CE_TYPE_SHORT):
		{
			count = snprintf(buffer, sizeof(buffer), "%d", value->data.as_short);
			break;
		}
		case (CE_TYPE_USHORT):
		{
			count = snprintf(buffer, sizeof(buffer), "%u", value->data.as_ushort);
			break;
		}
		case (CE_TYPE_INT):
		{
			count = snprintf(buffer, sizeof(buffer), "%d", value->data.as_int);
			break;
		}
		case (CE_TYPE_UINT):
		{
			count = snprintf(buffer, sizeof(buffer), "%u", value->data.as_uint);
			break;
		}
		case (CE_TYPE_LONG):
		{
			count = snprintf(buffer, sizeof(buffer), "%ld", value->data.as_long);
			break;
		}
		case (CE_TYPE_ULONG):
		{
			count = snprintf(buffer, sizeof(buffer), "%lu", value->data.as_ulong);
			break;
		}
		case (CE_TYPE_FLOAT):
		{
			count = snprintf(buffer, sizeof(buffer), "%f", value->data.as_float);
			break;
		}
		case (CE_TYPE_DOUBLE):
		{
			count = snprintf(buffer, sizeof(buffer), "%f", value->data.as_double);
			break;
		}
		case (CE_TYPE_SYMBOL):
		{
			return ceCreateSymbol(session, ceGetSymbolName(value->data.as_symbol), ceGetSymbolLength(value->data.as_symbol));
			break;
		}
		default:
			return NULL;	
	}
	
	if(count)
		return ceCreateSymbol(session, buffer, strlen(buffer));

	return NULL;	
}

ce_bool
ceIsValueEqual(ce_value a, ce_value b, ce_status* status)
{
	ce_value_t* va = (ce_value_t*)a;
	ce_value_t* vb = (ce_value_t*)b;
	
	if(va->type != vb->type)
	{
		if(status) (*status) = CE_TYPE_MISMATCH;
		return CE_FALSE;
	}
		
	switch(va->type)
	{
		case (CE_TYPE_BOOL):
			{
				return (va->data.as_bool == vb->data.as_bool) ? CE_TRUE : CE_FALSE;
			}
		case (CE_TYPE_CHAR):
			{
				return (va->data.as_char == vb->data.as_char) ? CE_TRUE : CE_FALSE;
			}
		case (CE_TYPE_UCHAR):
			{
				return (va->data.as_uchar == vb->data.as_uchar) ? CE_TRUE : CE_FALSE;
			}
		case (CE_TYPE_SHORT):
			{
				return (va->data.as_short == vb->data.as_short) ? CE_TRUE : CE_FALSE;
			}
		case (CE_TYPE_USHORT):
			{
				return (va->data.as_ushort == vb->data.as_ushort) ? CE_TRUE : CE_FALSE;
			}
		case (CE_TYPE_INT):
			{
				return (va->data.as_int == vb->data.as_int) ? CE_TRUE : CE_FALSE;
			}
		case (CE_TYPE_UINT):
			{
				return (va->data.as_uint == vb->data.as_uint) ? CE_TRUE : CE_FALSE;
			}
		case (CE_TYPE_LONG):
			{
				return (va->data.as_long == vb->data.as_long) ? CE_TRUE : CE_FALSE;
			}
		case (CE_TYPE_ULONG):
			{
				return (va->data.as_ulong == vb->data.as_ulong) ? CE_TRUE : CE_FALSE;
			}
		case (CE_TYPE_FLOAT):
			{
				return (va->data.as_float == vb->data.as_float) ? CE_TRUE : CE_FALSE;
			}
		case (CE_TYPE_DOUBLE):
			{
				return (va->data.as_double == vb->data.as_double) ? CE_TRUE : CE_FALSE;
			}
		case (CE_TYPE_SYMBOL):
			{
				return ceIsSymbolNameEqual(va->data.as_symbol, vb->data.as_symbol);
			}
			break;
		default:
			return CE_FALSE;	
	}
	
	return CE_FALSE;	
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

			ce_option handle = options[o];
			ce_option_t* opt = (ce_option_t*)handle;
			const char* name = ceGetSymbolName(opt->name);

			ceDebug(session, "ceParseCommandLineOptions: Parsing '%s'\n", name);

    		const char* str = strstr(argv[i], name);
    		if(str && str - start <= 2)
    		{
				const char* assign = strstr(str, "=");
				if(assign)
				{
					ceDebug(session, "ceParseCommandLineOptions: Parsed '%s' as '%s'\n", name, &assign[1]);
	        	    ParseOption(&assign[1], opt);
					found[o] = CE_TRUE;
				}
				else if(i + 1 < argc && argv[i + 1])
				{
					ceDebug(session, "ceParseCommandLineOptions: Parsed '%s' as '%s'\n", name, argv[i]);
	        	    ParseOption(argv[i], opt);
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

