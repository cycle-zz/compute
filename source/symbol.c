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

#include "symbol.h"

/**************************************************************************************************/

typedef struct _sc_symbol_t {
	sc_session					session;
	sc_uint						hash;
	char* 						name;
	sc_uint						length;
} sc_symbol_t;

/**************************************************************************************************/

static sc_uint
HashString(const char *str)
{
    int c;
    sc_uint hash = 5381;
    
    while ((c = (*str++)) != 0)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

/**************************************************************************************************/

sc_symbol
scCreateSymbol(
	sc_session session, 
	const char* name, 
	size_t length)
{
	sc_symbol_t* symbol;
	symbol = scAllocate(session, sizeof(sc_symbol_t));
	if(!symbol)
		return NULL;

	memset(symbol, 0, sizeof(sc_symbol_t));

	length++;
	symbol->name = scAllocate(session, sizeof(char) * length);
    if(symbol->name) 
    {
	    snprintf(symbol->name, length, "%s", name);
    	symbol->length = length;
		symbol->hash = HashString(name);    
		symbol->session = session;

	    scDebug(symbol->session, "Created symbol: name='%s' length='%d' hash='%u' session='%p'\n",
	    		symbol->name, symbol->length, symbol->hash, symbol->session);
	}
    
    return (sc_symbol)symbol;
}


sc_symbol
scCreateSymbolFromValue(
	sc_session session, sc_value value, sc_status* status)
{
	char buffer[256] = {0};

	int count = 0;
	switch(scGetValueType(value, status))
	{
		case (SC_TYPE_BOOL):
		{
			count = snprintf(buffer, sizeof(buffer), "%s", scGetBoolValue(value, status) == SC_TRUE ? "true" : "false");
			break;
		}
		case (SC_TYPE_CHAR):
		{
			count = 1;
			buffer[0] = scGetCharValue(value, status);
			break;
		}
		case (SC_TYPE_UCHAR):
		{
			count = 1;
			buffer[0] = scGetUCharValue(value, status);
			break;
		}
		case (SC_TYPE_SHORT):
		{
			count = snprintf(buffer, sizeof(buffer), "%d", scGetShortValue(value, status));
			break;
		}
		case (SC_TYPE_USHORT):
		{
			count = snprintf(buffer, sizeof(buffer), "%u", scGetUShortValue(value, status));
			break;
		}
		case (SC_TYPE_INT):
		{
			count = snprintf(buffer, sizeof(buffer), "%d", scGetIntValue(value, status));
			break;
		}
		case (SC_TYPE_UINT):
		{
			count = snprintf(buffer, sizeof(buffer), "%u", scGetUIntValue(value, status));
			break;
		}
		case (SC_TYPE_LONG):
		{
			count = snprintf(buffer, sizeof(buffer), "%lld", scGetLongValue(value, status));
			break;
		}
		case (SC_TYPE_ULONG):
		{
			count = snprintf(buffer, sizeof(buffer), "%llu", scGetULongValue(value, status));
			break;
		}
		case (SC_TYPE_FLOAT):
		{
			count = snprintf(buffer, sizeof(buffer), "%f", scGetFloatValue(value, status));
			break;
		}
		case (SC_TYPE_DOUBLE):
		{
			count = snprintf(buffer, sizeof(buffer), "%f", scGetDoubleValue(value, status));
			break;
		}
		case (SC_TYPE_SYMBOL):
		{
			return scGetSymbolValue(value, status);
			break;
		}
		default:
			return NULL;	
	}
	
	if(count)
		return scCreateSymbol(session, buffer, strlen(buffer));

	return NULL;	
}


void 
scReleaseSymbol(
	sc_symbol handle)
{
	sc_session session = NULL;
	sc_symbol_t* symbol = (sc_symbol_t*)handle;
	if(symbol && symbol->name) 
	{
		session = symbol->session;
		scDeallocate(session, symbol->name);
		symbol->name = NULL;
	}
	scDeallocate(session, symbol);
}

sc_bool
scIsSymbolNameEqual(
	sc_symbol a, sc_symbol b)
{
	if(!a || !b)
		return SC_FALSE;
		
	return ( scGetSymbolHash(a) == scGetSymbolHash(b) );
}

const char*
scGetSymbolName(
	sc_symbol handle)
{
	if(handle == NULL) return NULL;
	sc_symbol_t* symbol = (sc_symbol_t*)handle;
	return symbol->name;
}

sc_uint 
scGetSymbolHash(
	sc_symbol handle)
{
	if(handle == NULL) return 0;
	sc_symbol_t* symbol = (sc_symbol_t*)handle;
	return symbol->hash;
}

size_t
scGetSymbolLength(
	sc_symbol handle)
{
	if(handle == NULL) return 0;
	sc_symbol_t* symbol = (sc_symbol_t*)handle;
	return symbol->length;
}

/**************************************************************************************************/

