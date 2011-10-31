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

#include "memory.h"
#include "atomics.h"
#include "session.h"

/**************************************************************************************************/

typedef sc_type							sc_type_t;

typedef struct _sc_reference_t {
	sc_session							session;
	void*								ptr;
	sc_type_t 							type;
#if defined(SC_64BIT)
	sc_atomic_long						count;
#else
	sc_atomic_int						count;
#endif
} sc_reference_t;

typedef struct _sc_memory_block_t
{
	sc_reference_t*						reference;
	size_t 								size;
	const char* 						filename;
	cl_uint 							line;
	struct _sc_memory_block_t* 			prev;
	struct _sc_memory_block_t* 			next;
} sc_memory_block_t;

typedef struct _sc_memory_info_t
{
	size_t 								allocations;
	size_t 								deallocations;
	size_t 								max_allowed_bytes;
	size_t 								block_count;
	size_t 								byte_count;
	sc_memory_block_t* 					head_block;
	sc_memory_block_t* 					tail_block;
	sc_bool 							track_sizes;
	size_t 								max_allocated_bytes;
	size_t 								max_block_size;
	size_t 								histogram[32];
} sc_memory_info_t;

/**************************************************************************************************/

static void
InsertBlock (
	sc_memory_info_t *info,
	sc_memory_block_t* block)
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
	sc_memory_info_t *info,
	sc_memory_block_t* block)
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
		scCritical(SC_DEFAULT_SESSION, "Out of host memory!");
		return 0;
	}

	scDebug(SC_DEFAULT_SESSION, "HostMalloc: Allocated '%p' '%04d' bytes\n", ptr, size);
	return ptr;
}

static void
HostFree(void* ptr)
{
	scDebug(SC_DEFAULT_SESSION, "HostFree: Freeing '%p'\n", ptr);
	free(ptr);
}

/**************************************************************************************************/

void*
scAllocateHostMemory (
	sc_session handle,
	size_t bytes,
	char* filename,
	unsigned int line)
{
	sc_memory_info_t* memory = handle ? (sc_memory_info_t*)scGetMemoryInfo(handle, NULL) : NULL;
	sc_memory_info_t* info = memory ? ((sc_memory_info_t*)&memory[0]) : NULL;
	if(info == NULL)
	{
		return HostMalloc(bytes);
	}

    info->allocations++;
    size_t extended = sizeof(sc_memory_block_t) + bytes;

    char* ptr = (char*)HostMalloc(extended);
    if(!ptr)
        return 0;

    sc_memory_block_t* block = (sc_memory_block_t*)ptr;
	block->reference = NULL;
	block->size = bytes;
    block->filename = filename;
    block->line = line;
    InsertBlock(info, block);

    ptr += sizeof(sc_memory_block_t);

    info->block_count++;
    info->byte_count += bytes;

    if (info->max_allowed_bytes > 0 && info->byte_count > info->max_allowed_bytes)
    {
        scWarning(handle, "Allocation has exceeded the maximum number of allowed bytes!");
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

sc_status
scDeallocateHostMemory(
	sc_session handle,
	void* ptr)
{
	sc_memory_info_t* memory = handle ? (sc_memory_info_t*)scGetMemoryInfo(handle, NULL) : NULL;
	sc_memory_info_t* info = memory ? ((sc_memory_info_t*)&memory[0]) : NULL;
	if(info == NULL)
	{
		HostFree(ptr);
		return SC_SUCCESS;
	}

    if (ptr == NULL)
    {
        return SC_INVALID_VALUE;
    }

    ptr -= sizeof(sc_memory_block_t);
    sc_memory_block_t* block = (sc_memory_block_t*)ptr;
    if(block->reference)
    {
    	if(block->reference->count)
		{
	#if defined(SC_64BIT)
			scAtomicAddLong(&block->reference->count, -1);
	#else
			scAtomicAddInt(&block->reference->count, -1);
	#endif
		}

    	if(block->reference->count)
			return SC_SUCCESS;

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
        scWarning(handle, "Deallocation size mismatch for memory block!");
		return SC_SIZE_MISMATCH;
	}
	return SC_SUCCESS;
}

sc_reference
scCreateReference(
	sc_session handle,
	void* ptr)
{
	sc_memory_info_t* memory = handle ? (sc_memory_info_t*)scGetMemoryInfo(handle, NULL) : NULL;
	sc_memory_info_t* info = memory ? ((sc_memory_info_t*)&memory[0]) : NULL;
	if(info == NULL)
		return NULL;

    volatile sc_memory_block_t* block = (sc_memory_block_t*)(ptr - sizeof(sc_memory_block_t));
    if(block == NULL)
    	return NULL;

    if(!block->reference)
    {
    	block->reference = (sc_reference_t*)malloc(sizeof(sc_reference_t));
		memset(block->reference, 0, sizeof(sc_reference_t));

		block->reference->session = handle;
		block->reference->ptr = ptr;
    }

#if defined(SC_64BIT)
	scAtomicAddLong(&(block->reference->count), 1);
#else
	scAtomicAddInt(&(block->reference->count), 1);
#endif
	return (sc_reference)block->reference;
}

sc_status
scRetain(
	sc_session handle,
	sc_reference reference)
{
	sc_memory_info_t* memory = handle ? (sc_memory_info_t*)scGetMemoryInfo(handle, NULL) : NULL;
	sc_memory_info_t* info = memory ? ((sc_memory_info_t*)&memory[0]) : NULL;

	void* ptr = reference ? ((sc_reference_t*)reference)->ptr : NULL;

	if(info == NULL || ptr == NULL)
		return SC_INVALID_MEMORY_INFO;

    volatile sc_memory_block_t* block = (sc_memory_block_t*)(ptr - sizeof(sc_memory_block_t));
	if(!block)
		return SC_INVALID_MEMORY_INFO;

    if(!block->reference)
    {
    	block->reference = (sc_reference_t*)malloc(sizeof(sc_reference_t));
		memset(block->reference, 0, sizeof(sc_reference_t));

		block->reference->session = handle;
		block->reference->ptr = ptr;
    }

#if defined(SC_64BIT)
	scAtomicAddLong(&(block->reference->count), 1);
#else
	scAtomicAddInt(&(block->reference->count), 1);
#endif

	return SC_SUCCESS;
}

sc_status
scRelease(
	sc_session handle,
	sc_reference reference)
{
	void* ptr = reference ? ((sc_reference_t*)reference)->ptr : NULL;
	return scDeallocateHostMemory(handle, ptr);
}

sc_status
scLogHostMemoryInfo(
	sc_session handle)
{
	sc_memory_info_t* memory = handle ? (sc_memory_info_t*)scGetMemoryInfo(handle, NULL) : NULL;
	sc_memory_info_t* info = memory ? ((sc_memory_info_t*)&memory[0]) : NULL;
	if(info == NULL)
	{
		scWarning(handle, "Host memory tracking not enabled!");
		return SC_INVALID_MEMORY_INFO;
	}

	size_t index = 0;
    size_t named_block_count = 0;
    size_t named_byte_count = 0;
    size_t anonymous_block_count = 0;
    size_t anonymous_byte_count = 0;
    sc_memory_block_t* block = 0;

	scInfo(handle, "Total number of host memory allocations: %d", info->allocations);
	scInfo(handle, "Total number of host memory deallocations: %d", info->deallocations);
	scInfo(handle, "Maximum number of bytes allocated in host memory: %d", info->max_allocated_bytes);
	scInfo(handle, "Number of blocks in host memory still allocated: %d", info->block_count);
	scInfo(handle, "Number of bytes in host memory still allocated: %d", info->byte_count);

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

	scInfo(handle, "Number of named blocks in host memory: %d", named_block_count);
	scInfo(handle, "Number of named bytes in host memory: %d", named_byte_count);

	scInfo(handle, "Number of anonymous blocks in host memory: %d", anonymous_block_count);
	scInfo(handle, "Number of anonymous bytes in host memory: %d", anonymous_byte_count);

    block = info->head_block;
    index = 0;
    while (block)
    {

        if (block->filename)
        {
			scInfo(handle, "block[%08d] : %08d bytes -- file: '%s' line '%04d'", index, block->size, block->filename, block->line);
        }
        else
        {
			scInfo(handle, "block[%08d] : %08d bytes -- file: 'unknown' line 'unknown'", index, block->size, block->filename, block->line);
        }
        block = block->next;
        index++;
    }
	return SC_SUCCESS;
}
