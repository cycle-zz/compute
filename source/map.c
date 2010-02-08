/***************************************************************************************************

Scalable Compute Library - $SC_VERSION_TAG$ <$SC_ID_TAG$>

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

typedef unsigned short sc_map_index_t;

typedef struct _sc_map_node_t {
    struct _sc_map_node_t* 	next; 
    sc_symbol 					key;
    sc_reference				item;
} sc_map_node_t;

typedef struct _sc_map_t {
	sc_session					session;
	sc_map_node_t** 			bins;
	sc_map_index_t 				size;
} sc_map_t;

/**************************************************************************************************/

#define scMapCompare(a,b) ((a) && (b) && ((a) == (b)))

/**************************************************************************************************/

sc_map
scCreateMap(
	sc_session session, size_t size)
{
	sc_map_t *map;
	size_t bytes;
	
	map = scAllocate(session, sizeof(sc_map_t));
	map->size = 0;
	
	bytes = size * sizeof(sc_map_node_t *);
	map->bins = scAllocate(session, bytes);
	if(!map->bins)
		return 0;
    
    memset(map->bins, 0, bytes);
    map->size = size;
    map->session = session;
	return (sc_map)map;
}

void
scReleaseMap(
	sc_map handle)
{
    sc_map_node_t *node = NULL;
    sc_map_node_t *next = NULL;
    sc_map_index_t index = 0;

	sc_map_t* map = (sc_map_t*)handle;
	sc_session session = map->session;
	
	while(index < map->size)
	{
		node = map->bins[index++];
		while(node)
		{
			next = node;
			if(next)
				scDeallocate(session, next);
			node = node->next;
		}
    }
	
	scDeallocate(session, map->bins);
	scDeallocate(session, map);
}

cl_int
scMapInsert(
	sc_map handle, 
	sc_symbol key, 
	sc_reference item)
{
    sc_map_node_t *node = NULL;
    sc_map_node_t *next = NULL;
    sc_map_index_t index = 0;

	sc_map_t* map = (sc_map_t*)handle;
	
	index = scGetSymbolHash(key) % map->size;

    if ((node = scAllocate(map->session, sizeof(sc_map_node_t))) == 0) 
    	return 0;

    scRetain(map->session, item);

    next = map->bins[index];
    map->bins[index] = node;
    node->next = next;
    node->key = key;
    node->item = item;

    return CL_SUCCESS;
}

sc_reference 
scMapRemove(
	sc_map handle, 
	sc_symbol key)
{
    sc_map_node_t *node = NULL;
    sc_map_node_t *next = NULL;
    sc_map_index_t index = 0;
    sc_reference item = NULL;

	sc_map_t* map = (sc_map_t*)handle;
	
    index = scGetSymbolHash(key) % map->size;
    node = map->bins[index];

    while (node && !scMapCompare(node->key, key)) {
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
	scRelease(map->session, node->item);
	scReleaseSymbol(node->key);
    scDeallocate(map->session, node);
    return item;
}

sc_reference
scGetMapItem(
	sc_map handle, 
	sc_symbol key)
{
    sc_map_node_t *node = NULL;
    sc_map_index_t index = 0;

	sc_map_t* map = (sc_map_t*)handle;

	index = scGetSymbolHash(key) % map->size;
    node = map->bins[index];

    while (node && !scMapCompare(node->key, key)) 
    {
		if(node && node->item)
		{
			return node->item;
		}
    	node = node->next;
	}

 	return NULL;
}

