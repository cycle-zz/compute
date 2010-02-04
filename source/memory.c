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

#include "memory.h"
#include "atomics.h"

/**************************************************************************************************/

typedef ce_type							ce_type_t;

typedef struct _ce_reference_t {
	ce_session							session;
	void*								ptr;
	ce_type_t 							type;
#if defined(CE_64BIT)
	ce_atomic_long						count;
#else
	ce_atomic_int						count;
#endif
} ce_reference_t;

typedef struct _ce_memory_block_t
{
	ce_reference_t*						reference;
	size_t 								size;
	const char* 						filename;
	cl_uint 							line;
	struct _ce_memory_block_t* 			prev;
	struct _ce_memory_block_t* 			next;
} ce_memory_block_t;

typedef struct _ce_memory_info_t 
{
	size_t 								allocations;
	size_t 								deallocations;
	size_t 								max_allowed_bytes;
	size_t 								block_count;
	size_t 								byte_count;
	ce_memory_block_t* 					head_block;
	ce_memory_block_t* 					tail_block;
	cl_bool 							track_sizes;
	size_t 								max_allocated_bytes;
	size_t 								max_block_size;
	size_t 								histogram[32];
} ce_memory_info_t;

typedef struct _ce_session_memory_t {
	ce_memory_info*						host;
	ce_memory_info*						device;
} ce_session_memory_info_t;

/**************************************************************************************************/

static void 
InsertBlock (
	ce_memory_info_t *info, 
	ce_memory_block_t* block)
{
    if (info->tail_block)
    {
        block->prev = info->tail_block;
        block->next = 0;
        info->tail_block->next = block;
        info->tail_block = block;
    }
    else
    {
        block->prev = 0;
        block->next = 0;
        info->head_block = block;
        info->tail_block = block;
    }
}


static void 
RemoveBlock (
	ce_memory_info_t *info, 
	ce_memory_block_t* block)
{
    if (block->prev)
    {
        block->prev->next = block->next;
    }
    else
    {
        info->head_block = block->next;
    }
    
    if (block->next)
    {
        block->next->prev = block->prev;
    }
    else
    {
        info->tail_block = block->prev;
    }
}


/**************************************************************************************************/

static void*
HostMalloc(size_t size)
{
	void* ptr = (void*)malloc(size);
	if(!ptr)
	{
		ceCritical(CE_DEFAULT_SESSION, "Out of host memory!");
		return 0;
	}

	ceDebug(CE_DEFAULT_SESSION, "HostMalloc: Allocated '%p' '%04d' bytes\n", ptr, size);
	return ptr;
}

static void
HostFree(void* ptr)
{
	ceDebug(CE_DEFAULT_SESSION, "HostFree: Freeing '%p'\n", ptr);
	free(ptr);
}

/**************************************************************************************************/

void* 
ceAllocateHostMemory (
	ce_session handle,
	size_t bytes, 
	char* filename, 
	unsigned int line)
{
	ce_session_t* session = (ce_session_t*)handle;
	ce_session_memory_info_t* memory = session ? (ce_session_memory_info_t*)session->memory : NULL;
	ce_memory_info_t* info = memory ? ((ce_memory_info_t*)memory->host) : NULL;

	if(!session || !memory || !info)
	{
		return HostMalloc(bytes);
	}

    info->allocations++;
    size_t extended = sizeof(ce_memory_block_t) + bytes;

    char* ptr = (char*)HostMalloc(extended);
    if(!ptr)
        return 0;

    ce_memory_block_t* block = (ce_memory_block_t*)ptr;
	block->reference = NULL;
	block->size = bytes;
    block->filename = filename;
    block->line = line;
    InsertBlock(info, block);

    ptr += sizeof(ce_memory_block_t);

    info->block_count++;
    info->byte_count += bytes;

    if (info->max_allowed_bytes > 0 && info->byte_count > info->max_allowed_bytes)
    {
        ceWarning(handle, "Allocation has exceeded the maximum number of allowed bytes!");
        return 0;
    }

    if (info->byte_count > info->max_allocated_bytes)
    {
        info->max_allocated_bytes = info->byte_count;
    }

    if (info->track_sizes)
    {
        if (bytes > info->max_block_size)
        {
            info->max_block_size = bytes;
        }

        int i;
        unsigned int two_power = 1;
        for (i = 0; i <= 30; i++, two_power <<= 1)
        {
            if (bytes <= two_power)
            {
                info->histogram[i]++;
                break;
            }
        }
        if (i == 31)
        {
            info->histogram[i]++;
        }
    }
	
    return (void*)ptr;
}

ce_status
ceDeallocateHostMemory(
	ce_session handle, 
	void* ptr)
{
	ce_session_t* session = (ce_session_t*)handle;
	ce_session_memory_info_t* memory = session ? (ce_session_memory_info_t*)session->memory : NULL;
	ce_memory_info_t* info = memory ? ((ce_memory_info_t*)memory->host) : NULL;

	if(!session || !memory || !info)
	{
		HostFree(ptr);
		return CE_SUCCESS;
	}
	
    if (!ptr)
    {
        return CE_INVALID_VALUE;
    }

    ptr -= sizeof(ce_memory_block_t);
    ce_memory_block_t* block = (ce_memory_block_t*)ptr;
    if(block->reference)
    {
    	if(block->reference->count)
		{
	#if defined(CE_64BIT) 
			ceAtomicAddLong(&block->reference->count, -1);
	#else
			ceAtomicAddInt(&block->reference->count, -1);
	#endif
		}
		
    	if(block->reference->count)
			return CE_SUCCESS;		
			
		HostFree(block->reference);
		block->reference = NULL;
	}
    	
	info->deallocations++;
    RemoveBlock(info, block);

	if(info->block_count > 0 && info->byte_count >= block->size)
	{
		info->block_count--;
		info->byte_count -= block->size;
		HostFree(ptr);
	}
	else
	{
        ceWarning(handle, "Deallocation size mismatch for memory block!");
		return CE_SIZE_MISMATCH;
	}
	return CE_SUCCESS;
}

ce_reference
ceCreateReference(
	ce_session handle,
	void* ptr)
{
	ce_session_t* session = (ce_session_t*)handle;
	ce_session_memory_info_t* memory = session ? (ce_session_memory_info_t*)session->memory : NULL;
	ce_memory_info_t* info = memory ? ((ce_memory_info_t*)memory->host) : NULL;

	if(!session || !memory || !info)
	{
		return NULL;
	}
	
    volatile ce_memory_block_t* block = (ce_memory_block_t*)(ptr - sizeof(ce_memory_block_t));
    if(!block)
    	return NULL;
    	
    if(!block->reference)
    {
    	block->reference = (ce_reference_t*)malloc(sizeof(ce_reference_t));
		memset(block->reference, 0, sizeof(ce_reference_t));

		block->reference->session = handle;
		block->reference->ptr = ptr;
    }
    
#if defined(CE_64BIT) 
	ceAtomicAddLong(&(block->reference->count), 1);
#else
	ceAtomicAddInt(&(block->reference->count), 1);
#endif
	return (ce_reference)block->reference;
}

ce_status 
ceRetain(
	ce_session handle, 
	ce_reference reference)
{
	ce_session_t* session = (ce_session_t*)handle;
	ce_session_memory_info_t* memory = session ? (ce_session_memory_info_t*)session->memory : NULL;
	ce_memory_info_t* info = memory ? ((ce_memory_info_t*)memory->host) : NULL;
	void* ptr = reference ? ((ce_reference_t*)reference)->ptr : NULL;

	if(!session || !memory || !info || !ptr)
		return CE_INVALID_MEMORY_INFO;

    volatile ce_memory_block_t* block = (ce_memory_block_t*)(ptr - sizeof(ce_memory_block_t));
	if(!block)
		return CE_INVALID_MEMORY_INFO;
    
    if(!block->reference)
    {
    	block->reference = (ce_reference_t*)malloc(sizeof(ce_reference_t));
		memset(block->reference, 0, sizeof(ce_reference_t));

		block->reference->session = handle;
		block->reference->ptr = ptr;
    }
	
#if defined(CE_64BIT) 
	ceAtomicAddLong(&(block->reference->count), 1);
#else
	ceAtomicAddInt(&(block->reference->count), 1);
#endif

	return CE_SUCCESS;
}

ce_status
ceRelease(
	ce_session handle,
	ce_reference reference)
{
	void* ptr = reference ? ((ce_reference_t*)reference)->ptr : NULL;
	return ceDeallocateHostMemory(handle, ptr);
}

ce_status
ceLogHostMemoryInfo(
	ce_session handle)
{
	ce_session_t* session = (ce_session_t*)handle;
	ce_session_memory_info_t* memory = session ? (ce_session_memory_info_t*)session->memory : NULL;
	ce_memory_info_t* info = memory ? ((ce_memory_info_t*)memory->host) : NULL;

	if(!session || !memory || !info)
	{
		ceWarning(handle, "Host memory tracking not enabled!");
		return CE_INVALID_MEMORY_INFO;
	}
		
	size_t index = 0;
    size_t named_block_count = 0;
    size_t named_byte_count = 0;
    size_t anonymous_block_count = 0;
    size_t anonymous_byte_count = 0;
    ce_memory_block_t* block = 0;

	ceInfo(handle, "Total number of host memory allocations: %d", info->allocations);
	ceInfo(handle, "Total number of host memory deallocations: %d", info->deallocations);
	ceInfo(handle, "Maximum number of bytes allocated in host memory: %d", info->max_allocated_bytes);
	ceInfo(handle, "Number of blocks in host memory still allocated: %d", info->block_count);
	ceInfo(handle, "Number of bytes in host memory still allocated: %d", info->byte_count);

	block = info->head_block;
    while (block)
    {
        if (block->filename)
        {
            named_block_count++;
            named_byte_count += block->size;
        }
        else
        {
            anonymous_block_count++;
            anonymous_byte_count += block->size;
        }
        block = block->next;
    }

	ceInfo(handle, "Number of named blocks in host memory: %d", named_block_count);
	ceInfo(handle, "Number of named bytes in host memory: %d", named_byte_count);

	ceInfo(handle, "Number of anonymous blocks in host memory: %d", anonymous_block_count);
	ceInfo(handle, "Number of anonymous bytes in host memory: %d", anonymous_byte_count);

    block = info->head_block;
    index = 0;
    while (block)
    {

        if (block->filename)
        {
			ceInfo(handle, "block[%08d] : %08d bytes -- file: '%s' line '%04d'", index, block->size, block->filename, block->line);
        }
        else
        {
			ceInfo(handle, "block[%08d] : %08d bytes -- file: 'unknown' line 'unknown'", index, block->size, block->filename, block->line);
        }
        block = block->next;
        index++;
    }
	return CE_SUCCESS;
}
