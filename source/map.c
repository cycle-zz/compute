/***************************************************************************************************

Compute Engine - $CE_VERSION_TAG$ <$CE_ID_TAG$>

Copyright (c) 2010, Derek Gerstmanext <derek.gerstmanext[|AT|]uwa.edu.au> 
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

typedef struct _ce_map_node_t {
    struct _ce_map_node_t* 	next; 
    ce_symbol 					key;
    ce_reference				item;
} ce_map_node_t;

typedef struct _ce_map_t {
	ce_session					session;
	ce_map_node_t** 			bins;
	ce_map_index_t 				size;
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
	
	bytes = size * sizeof(ce_map_node_t *);
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
	ce_map handle)
{
    ce_map_node_t *node = NULL;
    ce_map_node_t *next = NULL;
    ce_map_index_t index = 0;

	ce_map_t* map = (ce_map_t*)handle;
	ce_session session = map->session;
	
	while(index < map->size)
	{
		node = map->bins[index++];
		while(node)
		{
			next = node;
			if(next)
				ceDeallocate(session, next);
			node = node->next;
		}
    }
	
	ceDeallocate(session, map->bins);
	ceDeallocate(session, map);
}

cl_int
ceMapInsert(
	ce_map handle, 
	ce_symbol key, 
	ce_reference item)
{
    ce_map_node_t *node = NULL;
    ce_map_node_t *next = NULL;
    ce_map_index_t index = 0;

	ce_map_t* map = (ce_map_t*)handle;
	
	index = ceGetSymbolHash(key) % map->size;

    if ((node = ceAllocate(map->session, sizeof(ce_map_node_t))) == 0) 
    	return 0;

    ceRetain(map->session, item);

    next = map->bins[index];
    map->bins[index] = node;
    node->next = next;
    node->key = key;
    node->item = item;

    return CL_SUCCESS;
}

ce_reference 
ceMapRemove(
	ce_map handle, 
	ce_symbol key)
{
    ce_map_node_t *node = NULL;
    ce_map_node_t *next = NULL;
    ce_map_index_t index = 0;
    ce_reference item = NULL;

	ce_map_t* map = (ce_map_t*)handle;
	
    index = ceGetSymbolHash(key) % map->size;
    node = map->bins[index];

    while (node && !ceMapCompare(node->key, key)) {
        next = node;
        node = node->next;
    }

    if (!node) 
    	return NULL;

    if (next)
        next->next = node->next;

    else
        map->bins[index] = node->next;

	item = node->item;
	ceRelease(map->session, node->item);
	ceReleaseSymbol(node->key);
    ceDeallocate(map->session, node);
    return item;
}

ce_reference
ceGetMapItem(
	ce_map handle, 
	ce_symbol key)
{
    ce_map_node_t *node = NULL;
    ce_map_index_t index = 0;

	ce_map_t* map = (ce_map_t*)handle;

	index = ceGetSymbolHash(key) % map->size;
    node = map->bins[index];

    while (node && !ceMapCompare(node->key, key)) 
    {
		if(node && node->item)
		{
			return node->item;
		}
    	node = node->next;
	}

 	return NULL;
}

