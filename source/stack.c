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

#include "stack.h"
#include "atomics.h"

/**************************************************************************************************/

typedef struct _ce_stack_node_t {
  struct _ce_stack_node_t*		next; 
  ce_reference	 				item;
  ce_reference	 				reference;
} ce_stack_node_t;

typedef struct _ce_stack_t {
	ce_stack_node_t*			head;
	ce_session					session;
} ce_stack_t;

/**************************************************************************************************/

static ce_stack_node_t* 
CreateStackNode(
	ce_session session,
	ce_reference item)
{
	ce_stack_node_t* node = ceAllocate(session, sizeof(ce_stack_node_t));
	memset(node, 0, sizeof(ce_stack_node_t));
	node->reference = ceCreateReference(session, node);
	node->item = item;
	return node;
}

ce_stack 
ceCreateStack(
	ce_session session)
{
	ce_stack_t* stack = ceAllocate(session, sizeof(ce_stack_t));
	memset(stack, 0, sizeof(ce_stack_t));
	
	stack->session = session;
	stack->head = CreateStackNode(session, NULL);
	return (ce_stack)stack;
}

ce_status
ceReleaseStack(
   ce_stack handle)
{
	ce_uint count = 0;
	ce_stack_t* stack = (ce_stack_t*)(handle);

	while(cePopStack(handle) != NULL) { count++; }

	return ceDeallocate(stack->session, stack);
}

void 
cePushStack(
	ce_stack handle,
	ce_reference item)
{
	ce_stack_t* stack = (ce_stack_t*)(handle);
	ce_stack_node_t* node = CreateStackNode((ce_session)stack->session, item);
	do {
		node = stack->head->next;
	}
	while( !ceAtomicCompareAndSwapPtr((void**)&(stack->head->next), node->next, node) ); 
}

ce_reference 
cePopStack(
	ce_stack handle)
{
	ce_stack_t* stack = (ce_stack_t*)(handle);
	ce_stack_node_t* node = NULL;
	ce_reference item = NULL;
	
	do {
		node = stack->head->next;
		if(node == NULL) 
			return item;
	} 
	while( !ceAtomicCompareAndSwapPtr((void**)&(stack->head->next), node, node->next) ); 
	
	item = node->item;
	ceRelease(stack->session, node->reference);	
	return item;
}

/**************************************************************************************************/

