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

#include "symbol.h"

/**************************************************************************************************/

typedef struct _ce_symbol_t {
	ce_session					session;
	ce_uint						hash;
	char* 						name;
	ce_uint						length;
} ce_symbol_t;

/**************************************************************************************************/

static ce_uint
HashString(const char *str)
{
    int c;
    ce_uint hash = 5381;
    
    while ((c = (*str++)) != 0)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

/**************************************************************************************************/

ce_symbol
ceCreateSymbol(
	ce_session session, 
	const char* name, 
	size_t length)
{
	ce_symbol_t* symbol;
	symbol = ceAllocate(session, sizeof(ce_symbol_t));
	if(!symbol)
		return NULL;

	memset(symbol, 0, sizeof(ce_symbol_t));

	length++;
	symbol->name = ceAllocate(session, sizeof(char) * length);
    if(symbol->name) 
    {
	    snprintf(symbol->name, length, "%s", name);
    	symbol->length = length;
		symbol->hash = HashString(name);    
		symbol->session = session;

	    ceDebug(symbol->session, "Created symbol: name='%s' length='%d' hash='%u' session='%p'\n",
	    		symbol->name, symbol->length, symbol->hash, symbol->session);
	}
    
    return (ce_symbol)symbol;
}

void 
ceReleaseSymbol(
	ce_symbol handle)
{
	ce_session session = NULL;
	ce_symbol_t* symbol = (ce_symbol_t*)handle;
	if(symbol && symbol->name) 
	{
		session = symbol->session;
		ceDeallocate(session, symbol->name);
		symbol->name = NULL;
	}
	ceDeallocate(session, symbol);
}

ce_bool
ceIsSymbolNameEqual(
	ce_symbol a, ce_symbol b)
{
	if(!a || !b)
		return CE_FALSE;
		
	return ( ceGetSymbolHash(a) == ceGetSymbolHash(b) );
}

const char*
ceGetSymbolName(
	ce_symbol handle)
{
	if(handle == NULL) return NULL;
	ce_symbol_t* symbol = (ce_symbol_t*)handle;
	return symbol->name;
}

ce_uint 
ceGetSymbolHash(
	ce_symbol handle)
{
	if(handle == NULL) return 0;
	ce_symbol_t* symbol = (ce_symbol_t*)handle;
	return symbol->hash;
}

size_t
ceGetSymbolLength(
	ce_symbol handle)
{
	if(handle == NULL) return 0;
	ce_symbol_t* symbol = (ce_symbol_t*)handle;
	return symbol->length;
}

/**************************************************************************************************/

