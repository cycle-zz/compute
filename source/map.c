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

#include "map.h"
#include "symbol.h"

/**************************************************************************************************/

typedef unsigned short ce_map_index_t;

typedef struct _ce_map_entry_t {
    struct _ce_map_entry_t* 	next; 
    ce_symbol 					key;
    void*						data;
   	size_t						size;
    ce_type_t					type;
} ce_map_entry_t;

typedef struct _ce_map_t {
	ce_session					session;
	ce_map_entry_t** 			bins;
	ce_map_index_t 			size;
} ce_map_t;

/**************************************************************************************************/

#define ceMapCompare(a,b) ((a) && (b) && ((a) == (b)))

/**************************************************************************************************/

ce_map
ceCreateMap(
	ce_session session, size_t size)
{
	ce_map_t *map;
	size_t bytes;
	
	map = ceAllocate(session, sizeof(ce_map_t));
	map->size = 0;
	
	bytes = size * sizeof(ce_map_entry_t *);
	map->bins = ceAllocate(session, bytes);
	if(!map->bins)
		return 0;
    
    memset(map->bins, 0, bytes);
    map->size = size;
    map->session = session;
	return (ce_map)map;
}

void
ceReleaseMap(
	ce_map map)
{
    ce_map_entry_t *n = 0;
    ce_map_entry_t *nn = 0;
    ce_map_index_t index = 0;

	ce_map_t* m = (ce_map_t*)map;
	ce_session s = m->session;
	
	while(index < m->size)
	{
		n = m->bins[index++];
		while(n)
		{
			nn = n;
			if(nn)
				ceDeallocate(s, nn);
			n = n->next;
		}
    }
	
	ceDeallocate(s, m->bins);
	ceDeallocate(s, m);
    memset(map, 0, sizeof(ce_map_t));
}

cl_int
ceMapInsert(
	ce_map map, ce_symbol key, ce_type_t type, size_t size, void* data)
{
    ce_map_entry_t *n = 0;
    ce_map_entry_t *nn = 0;
    ce_map_index_t index = 0;

	ce_map_t* m = (ce_map_t*)map;
	ce_symbol_t* sym = (ce_symbol_t*)key;
    if(sym == 0) 
    	return 0;

    index = sym->hash % m->size;

    if ((n = ceAllocate(m->session, sizeof(ce_map_entry_t))) == 0) 
    	return 0;

    nn = m->bins[index];
    m->bins[index] = n;

    n->next = nn;
    n->key = key;
    n->type = type;
    n->data = data;
    n->size = size;
    return CL_SUCCESS;
}

int 
ceMapRemove(
	ce_map map, ce_symbol key)
{
    ce_map_entry_t *n = 0;
    ce_map_entry_t *nn = 0;
    ce_map_index_t index = 0;

	ce_map_t* m = (ce_map_t*)map;
	ce_symbol_t* sym = (ce_symbol_t*)key;
    if(sym == 0) 
    	return 0;

    index = sym->hash % m->size;
    n = m->bins[index];

    while (n && !ceMapCompare(n->key, key)) {
        nn = n;
        n = n->next;
    }

    if (!n) 
    	return -1;

    if (nn)
        nn->next = n->next;

    else
        m->bins[index] = n->next;

	ceReleaseSymbol(n->key);
    ceDeallocate(m->session, n);
    return 1;
}

cl_int
ceGetMapEntry(
	ce_map map, ce_symbol key, ce_type_t type, size_t size, void* value, size_t *returned_size)
{
    ce_map_entry_t *n = 0;
    ce_map_index_t index = 0;

	ce_map_t* m = (ce_map_t*)map;
	ce_symbol_t* sym = (ce_symbol_t*)key;

    index = sym->hash % m->size;
    n = m->bins[index];

    while (n && !ceMapCompare(n->key, key)) 
    {
		if(n && n->type == type && n->size == size && n->data)
		{
			char* ptr = (char*)value;
			memcpy(ptr, (char*)n->data, size);
			(*returned_size) = n->size;
			return CL_SUCCESS;
		}
    	n = n->next;
	}

 	return CL_INVALID_VALUE;
}

