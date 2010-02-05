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
	ceSetUCharValue(value, data);
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

ce_bool
ceIsValueEqual(ce_value a, ce_value b, ce_status* status)
{
	ce_value_t* va = (ce_value_t*)a;
	ce_value_t* vb = (ce_value_t*)b;
	if(!va || !vb)
	{
		if(status) (*status) = CE_INVALID_VALUE;
		return CE_INVALID_TYPE;
	}
	
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


