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

#include "values.h"
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

/**************************************************************************************************/

sc_value
scCreateValue(
	sc_session session, sc_type type)
{
	sc_value_t* value = scAllocate(session, sizeof(sc_value_t));
	if(!value)
		return NULL;
		
	memset(value, 0, sizeof(sc_value_t));
	value->type = type;
	return (sc_value)value;
}

sc_value
scCreateBoolValue(
	sc_session session, sc_bool data)
{
	sc_value value = scCreateValue(session, SC_TYPE_BOOL);
	scSetBoolValue(value, data);
	return value;
}

sc_value
scCreateCharValue(
	sc_session session, sc_char data)
{
	sc_value value = scCreateValue(session, SC_TYPE_CHAR);
	scSetCharValue(value, data);
	return value;
}

sc_value
scCreateUCharValue(
	sc_session session, sc_uchar data)
{
	sc_value value = scCreateValue(session, SC_TYPE_UCHAR);
	scSetUCharValue(value, data);
	return value;
}

sc_value
scCreateShortValue(
	sc_session session, sc_short data)
{
	sc_value value = scCreateValue(session, SC_TYPE_SHORT);
	scSetShortValue(value, data);
	return value;
}

sc_value
scCreateUShortValue(
	sc_session session, sc_ushort data)
{
	sc_value value = scCreateValue(session, SC_TYPE_USHORT);
	scSetUShortValue(value, data);
	return value;
}

sc_value
scCreateIntValue(
	sc_session session, sc_int data)
{
	sc_value value = scCreateValue(session, SC_TYPE_INT);
	scSetIntValue(value, data);
	return value;
}

sc_value
scCreateUIntValue(
	sc_session session, sc_uint data)
{
	sc_value value = scCreateValue(session, SC_TYPE_UINT);
	scSetUIntValue(value, data);
	return value;
}

sc_value
scCreateLongValue(
	sc_session session, sc_long data)
{
	sc_value value = scCreateValue(session, SC_TYPE_LONG);
	scSetLongValue(value, data);
	return value;
}

sc_value
scCreateULongValue(
	sc_session session, sc_ulong data)
{
	sc_value value = scCreateValue(session, SC_TYPE_ULONG);
	scSetULongValue(value, data);
	return value;
}

sc_value
scCreateFloatValue(
	sc_session session, sc_float data)
{
	sc_value value = scCreateValue(session, SC_TYPE_FLOAT);
	scSetFloatValue(value, data);
	return value;
}

sc_value
scCreateDoubleValue(
	sc_session session, sc_double data)
{
	sc_value value = scCreateValue(session, SC_TYPE_DOUBLE);
	scSetDoubleValue(value, data);
	return value;
}

sc_value
scCreateSymbolValue(
	sc_session session, sc_symbol data)
{
	sc_value value = scCreateValue(session, SC_TYPE_SYMBOL);
	scSetSymbolValue(value, data);
	return value;
}

sc_status
scReleaseValue(
	sc_value handle)
{
	sc_value_t* value = (sc_value_t*)handle;
	if(!value)
		return SC_INVALID_VALUE;
		
	if(value->type == SC_TYPE_SYMBOL && value->data.as_symbol)
		scReleaseSymbol(value->data.as_symbol);

	return scDeallocate(SC_DEFAULT_SESSION, handle);
}

sc_status
scSetBoolValue(
	sc_value handle, sc_bool data)
{
	sc_value_t* value = (sc_value_t*)handle;
	if(!value) 
		return SC_INVALID_VALUE;
		
	if(value->type != SC_TYPE_BOOL)
		return SC_TYPE_MISMATCH;
		
	value->data.as_bool = data;
	return SC_SUCCESS;
}

sc_bool
scGetBoolValue(
	sc_value handle, sc_status *status)
{
	sc_value_t* value = (sc_value_t*)handle;
	if(!value)
	{
		if(status) (*status) = SC_INVALID_VALUE;
		return -1;
	}
	
	if(value->type != SC_TYPE_BOOL)
	{
		if(status) (*status) = SC_TYPE_MISMATCH;
		return -1;
	}
	
	return value->data.as_bool;
}

sc_status
scSetCharValue(
	sc_value handle, sc_char data)
{
	sc_value_t* value = (sc_value_t*)handle;
	if(!value) 
		return SC_INVALID_VALUE;
		
	if(value->type != SC_TYPE_CHAR)
		return SC_TYPE_MISMATCH;
		
	value->data.as_char = data;
	return SC_SUCCESS;
}

sc_char
scGetCharValue(
	sc_value handle, sc_status *status)
{
	sc_value_t* value = (sc_value_t*)handle;
	if(!value)
	{
		if(status) (*status) = SC_INVALID_VALUE;
		return -1;
	}
	
	if(value->type != SC_TYPE_CHAR)
	{
		if(status) (*status) = SC_TYPE_MISMATCH;
		return -1;
	}
	
	return value->data.as_char;
}

sc_status
scSetUCharValue(
	sc_value handle, sc_uchar data)
{
	sc_value_t* value = (sc_value_t*)handle;
	if(!value) 
		return SC_INVALID_VALUE;
		
	if(value->type != SC_TYPE_UCHAR)
		return SC_TYPE_MISMATCH;
		
	value->data.as_uchar = data;
	return SC_SUCCESS;
}

sc_uchar
scGetUCharValue(
	sc_value handle, sc_status *status)
{
	sc_value_t* value = (sc_value_t*)handle;
	if(!value)
	{
		if(status) (*status) = SC_INVALID_VALUE;
		return -1;
	}
	
	if(value->type != SC_TYPE_UCHAR)
	{
		if(status) (*status) = SC_TYPE_MISMATCH;
		return -1;
	}

	return value->data.as_uchar;
}


sc_status
scSetShortValue(
	sc_value handle, sc_short data)
{
	sc_value_t* value = (sc_value_t*)handle;
	if(!value) 
		return SC_INVALID_VALUE;
		
	if(value->type != SC_TYPE_SHORT)
		return SC_TYPE_MISMATCH;
		
	value->data.as_short = data;
	return SC_SUCCESS;
}

sc_short
scGetShortValue(
	sc_value handle, sc_status *status)
{
	sc_value_t* value = (sc_value_t*)handle;
	if(!value)
	{
		if(status) (*status) = SC_INVALID_VALUE;
		return -1;
	}
	
	if(value->type != SC_TYPE_SHORT)
	{
		if(status) (*status) = SC_TYPE_MISMATCH;
		return -1;
	}
	
	return value->data.as_short;
}

sc_status
scSetUShortValue(
	sc_value handle, sc_ushort data)
{
	sc_value_t* value = (sc_value_t*)handle;
	if(!value) 
		return SC_INVALID_VALUE;
		
	if(value->type != SC_TYPE_USHORT)
		return SC_TYPE_MISMATCH;
		
	value->data.as_ushort = data;
	return SC_SUCCESS;
}

sc_ushort
scGetUShortValue(
	sc_value handle, sc_status *status)
{
	sc_value_t* value = (sc_value_t*)handle;
	if(!value)
	{
		if(status) (*status) = SC_INVALID_VALUE;
		return -1;
	}
	
	if(value->type != SC_TYPE_USHORT)
	{
		if(status) (*status) = SC_TYPE_MISMATCH;
		return -1;
	}
	
	return value->data.as_ushort;
}

sc_status
scSetIntValue(
	sc_value handle, sc_int data)
{
	sc_value_t* value = (sc_value_t*)handle;
	if(!value) 
		return SC_INVALID_VALUE;
		
	if(value->type != SC_TYPE_INT)
		return SC_TYPE_MISMATCH;
		
	value->data.as_int = data;
	return SC_SUCCESS;
}

sc_int
scGetIntValue(	
	sc_value handle, sc_status *status)
{
	sc_value_t* value = (sc_value_t*)handle;
	if(!value)
	{
		if(status) (*status) = SC_INVALID_VALUE;
		return -1;
	}
	
	if(value->type != SC_TYPE_INT)
	{
		if(status) (*status) = SC_TYPE_MISMATCH;
		return -1;
	}
	
	return value->data.as_int;
}


sc_status
scSetUIntValue(
	sc_value handle, sc_uint data)
{
	sc_value_t* value = (sc_value_t*)handle;
	if(!value) 
		return SC_INVALID_VALUE;
		
	if(value->type != SC_TYPE_UINT)
		return SC_TYPE_MISMATCH;
		
	value->data.as_uint = data;
	return SC_SUCCESS;
}

sc_uint
scGetUIntValue(
	sc_value handle, sc_status *status)
{
	sc_value_t* value = (sc_value_t*)handle;
	if(!value)
	{
		if(status) (*status) = SC_INVALID_VALUE;
		return -1;
	}
	
	if(value->type != SC_TYPE_UINT)
	{
		if(status) (*status) = SC_TYPE_MISMATCH;
		return -1;
	}
	
	return value->data.as_uint;
}

sc_status
scSetLongValue(
	sc_value handle, sc_long data)
{
	sc_value_t* value = (sc_value_t*)handle;
	if(!value) 
		return SC_INVALID_VALUE;
		
	if(value->type != SC_TYPE_LONG)
		return SC_TYPE_MISMATCH;
		
	value->data.as_long = data;
	return SC_SUCCESS;
}

sc_long
scGetLongValue(
	sc_value handle, sc_status *status)
{
	sc_value_t* value = (sc_value_t*)handle;
	if(!value)
	{
		if(status) (*status) = SC_INVALID_VALUE;
		return -1;
	}
	
	if(value->type != SC_TYPE_LONG)
	{
		if(status) (*status) = SC_TYPE_MISMATCH;
		return -1;
	}
	
	return value->data.as_long;
}

sc_status
scSetULongValue(
	sc_value handle, sc_ulong data)
{
	sc_value_t* value = (sc_value_t*)handle;
	if(!value) 
		return SC_INVALID_VALUE;
		
	if(value->type != SC_TYPE_ULONG)
		return SC_TYPE_MISMATCH;
		
	value->data.as_ulong = data;
	return SC_SUCCESS;
}

sc_ulong
scGetULongValue(
	sc_value handle, sc_status *status)
{
	sc_value_t* value = (sc_value_t*)handle;
	if(!value)
	{
		if(status) (*status) = SC_INVALID_VALUE;
		return -1;
	}
	
	if(value->type != SC_TYPE_ULONG)
	{
		if(status) (*status) = SC_TYPE_MISMATCH;
		return -1;
	}
	
	return value->data.as_ulong;
}

sc_status
scSetFloatValue(
	sc_value handle, sc_float data)
{
	sc_value_t* value = (sc_value_t*)handle;
	if(!value) 
		return SC_INVALID_VALUE;
		
	if(value->type != SC_TYPE_FLOAT)
		return SC_TYPE_MISMATCH;
		
	value->data.as_float = data;
	return SC_SUCCESS;
}

sc_float
scGetFloatValue(
	sc_value handle, sc_status *status)
{
	sc_value_t* value = (sc_value_t*)handle;
	if(!value)
	{
		if(status) (*status) = SC_INVALID_VALUE;
		return -1;
	}
	
	if(value->type != SC_TYPE_FLOAT)
	{
		if(status) (*status) = SC_TYPE_MISMATCH;
		return -1;
	}
	
	return value->data.as_float;
}

sc_status
scSetDoubleValue(
	sc_value handle, sc_double data)
{
	sc_value_t* value = (sc_value_t*)handle;
	if(!value) 
		return SC_INVALID_VALUE;
		
	if(value->type != SC_TYPE_DOUBLE)
		return SC_TYPE_MISMATCH;
		
	value->data.as_double = data;
	return SC_SUCCESS;
}

sc_double
scGetDoubleValue(
	sc_value handle, sc_status *status)
{
	sc_value_t* value = (sc_value_t*)handle;
	if(!value)
	{
		if(status) (*status) = SC_INVALID_VALUE;
		return -1;
	}
	
	if(value->type != SC_TYPE_DOUBLE)
	{
		if(status) (*status) = SC_TYPE_MISMATCH;
		return -1;
	}
	
	return value->data.as_double;
}


sc_status
scSetSymbolValue(
	sc_value handle, sc_symbol data)
{
	sc_value_t* value = (sc_value_t*)handle;
	if(!value) 
		return SC_INVALID_VALUE;
		
	if(value->type != SC_TYPE_SYMBOL)
		return SC_TYPE_MISMATCH;

	if(value->data.as_symbol)
		scReleaseSymbol(value->data.as_symbol);
		
	value->data.as_symbol = scCreateSymbol(SC_DEFAULT_SESSION, scGetSymbolName(data), scGetSymbolLength(data));
	return SC_SUCCESS;
}

sc_symbol
scGetSymbolValue(
	sc_value handle, sc_status *status)
{
	sc_value_t* value = (sc_value_t*)handle;
	if(!value)
	{
		if(status) (*status) = SC_INVALID_VALUE;
		return 0;
	}
	
	if(value->type != SC_TYPE_SYMBOL)
	{
		if(status) (*status) = SC_TYPE_MISMATCH;
		return 0;
	}

	return value->data.as_symbol;
}

sc_type
scGetValueType(
	sc_value handle, sc_status* status)
{
	sc_value_t* value = (sc_value_t*)handle;
	if(!value)
	{
		if(status) (*status) = SC_INVALID_VALUE;
		return SC_INVALID_TYPE;
	}
	
	return value->type;
}

sc_bool
scIsValueEqual(sc_value a, sc_value b, sc_status* status)
{
	sc_value_t* va = (sc_value_t*)a;
	sc_value_t* vb = (sc_value_t*)b;
	if(!va || !vb)
	{
		if(status) (*status) = SC_INVALID_VALUE;
		return SC_INVALID_TYPE;
	}
	
	if(va->type != vb->type)
	{
		if(status) (*status) = SC_TYPE_MISMATCH;
		return SC_FALSE;
	}
		
	switch(va->type)
	{
		case (SC_TYPE_BOOL):
			{
				return (va->data.as_bool == vb->data.as_bool) ? SC_TRUE : SC_FALSE;
			}
		case (SC_TYPE_CHAR):
			{
				return (va->data.as_char == vb->data.as_char) ? SC_TRUE : SC_FALSE;
			}
		case (SC_TYPE_UCHAR):
			{
				return (va->data.as_uchar == vb->data.as_uchar) ? SC_TRUE : SC_FALSE;
			}
		case (SC_TYPE_SHORT):
			{
				return (va->data.as_short == vb->data.as_short) ? SC_TRUE : SC_FALSE;
			}
		case (SC_TYPE_USHORT):
			{
				return (va->data.as_ushort == vb->data.as_ushort) ? SC_TRUE : SC_FALSE;
			}
		case (SC_TYPE_INT):
			{
				return (va->data.as_int == vb->data.as_int) ? SC_TRUE : SC_FALSE;
			}
		case (SC_TYPE_UINT):
			{
				return (va->data.as_uint == vb->data.as_uint) ? SC_TRUE : SC_FALSE;
			}
		case (SC_TYPE_LONG):
			{
				return (va->data.as_long == vb->data.as_long) ? SC_TRUE : SC_FALSE;
			}
		case (SC_TYPE_ULONG):
			{
				return (va->data.as_ulong == vb->data.as_ulong) ? SC_TRUE : SC_FALSE;
			}
		case (SC_TYPE_FLOAT):
			{
				return (va->data.as_float == vb->data.as_float) ? SC_TRUE : SC_FALSE;
			}
		case (SC_TYPE_DOUBLE):
			{
				return (va->data.as_double == vb->data.as_double) ? SC_TRUE : SC_FALSE;
			}
		case (SC_TYPE_SYMBOL):
			{
				return scIsSymbolNameEqual(va->data.as_symbol, vb->data.as_symbol);
			}
			break;
		default:
			return SC_FALSE;	
	}
	
	return SC_FALSE;	
}

/**************************************************************************************************/


