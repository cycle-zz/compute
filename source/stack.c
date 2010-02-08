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

#include "stack.h"
#include "atomics.h"

/**************************************************************************************************/

typedef struct _sc_stack_node_t {
  struct _sc_stack_node_t*		next; 
  sc_reference	 				item;
  sc_reference	 				reference;
} sc_stack_node_t;

typedef struct _sc_stack_t {
	sc_stack_node_t*			head;
	sc_session					session;
} sc_stack_t;

/**************************************************************************************************/

static sc_stack_node_t* 
CreateStackNode(
	sc_session session,
	sc_reference item)
{
	sc_stack_node_t* node = scAllocate(session, sizeof(sc_stack_node_t));
	memset(node, 0, sizeof(sc_stack_node_t));
	node->reference = scCreateReference(session, node);
	node->item = item;
	return node;
}

sc_stack 
scCreateStack(
	sc_session session)
{
	sc_stack_t* stack = scAllocate(session, sizeof(sc_stack_t));
	memset(stack, 0, sizeof(sc_stack_t));
	
	stack->session = session;
	stack->head = CreateStackNode(session, NULL);
	return (sc_stack)stack;
}

sc_status
scReleaseStack(
   sc_stack handle)
{
	sc_uint count = 0;
	sc_stack_t* stack = (sc_stack_t*)(handle);

	while(cePopStack(handle) != NULL) { count++; }

	return scDeallocate(stack->session, stack);
}

void 
cePushStack(
	sc_stack handle,
	sc_reference item)
{
	sc_stack_t* stack = (sc_stack_t*)(handle);
	sc_stack_node_t* node = CreateStackNode((sc_session)stack->session, item);
	do {
		node = stack->head->next;
	}
	while( !scAtomicCompareAndSwapPtr((void**)&(stack->head->next), node->next, node) ); 
}

sc_reference 
cePopStack(
	sc_stack handle)
{
	sc_stack_t* stack = (sc_stack_t*)(handle);
	sc_stack_node_t* node = NULL;
	sc_reference item = NULL;
	
	do {
		node = stack->head->next;
		if(node == NULL) 
			return item;
	} 
	while( !scAtomicCompareAndSwapPtr((void**)&(stack->head->next), node, node->next) ); 
	
	item = node->item;
	scRelease(stack->session, node->reference);	
	return item;
}

/**************************************************************************************************/

