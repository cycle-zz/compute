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

#ifndef __CE_ATOMICS_H__
#define __CE_ATOMICS_H__

/**************************************************************************************************/

#include "platform.h"
#include "types.h"
#include "status.h"

/**************************************************************************************************/

CE_EXTERN_C_BEGIN

/**************************************************************************************************/

#if defined(CE_PLATFORM_OSX) && defined(CE_ARCH_PPC)
	#include <libkern/OSAtomic.h>
#endif

#if defined(CE_USE_PTHREADS)
	#include <pthread.h>
	#include <semaphore.h>
#endif

#if defined(CE_PLATFORM_WINDOWS)

	typedef volatile LONG 			ce_atomic_int;
	#if defined(CE_64BIT)
		typedef volatile LONGLONG 	ce_atomic_long;
	#endif

	#if _(MSC_VER >= 1300)
		extern "C" void _ReadWriteBarrier();
		#pragma intrinsic(_ReadWriteBarrier)
	#else
		#define _ReadWriteBarrier()
	#endif

#else

	typedef volatile ce_int 		ce_atomic_int;
	#if defined(CE_64BIT)
		typedef volatile ce_long 	ce_atomic_long;
	#endif

#endif

/**************************************************************************************************/

CE_INLINE ce_int 
ceAtomicAddInt(
	ce_atomic_int *v, 
	ce_int delta) 
{
#if defined(CE_PLATFORM_WINDOWS)
	#if defined(CE_64BIT)
    	
    	return InterlockedAdd(v, delta);
    	
	#else
		
		ce_int result;
		_ReadWriteBarrier();
		__asm {
			__asm mov edx, v
			__asm mov eax, delta
			__asm lock xadd [edx], eax
			__asm mov result, eax
		}
		
		_ReadWriteBarrier();
		return result + delta;
	
	#endif
#elif defined(CE_PLATFORM_MACOSX) && defined(CE_ARCH_PPC)

    return OSAtomicAdd32Barrier(delta, v);

#else
    
    ce_int result;
    
    __asm__ __volatile__(
    	"lock\n"
		"xaddl %0,%1"
	   : "=r"(result), "=m"(*v) : "0"(delta)
	   : "memory");
                         
    return result + delta;
    
#endif
}


/**************************************************************************************************/

#if defined(CE_64BIT)

CE_INLINE ce_long 
ceAtomicAddLong(
	ce_atomic_long *v, 
	ce_long delta) 
{
#if defined(CE_PLATFORM_WINDOWS)

    return InterlockedAdd64(v, delta);

#elif defined(CE_PLATFORM_MACOSX) && defined(CE_ARCH_PPC)

    return OSAtomicAdd64Barrier(delta, v);

#else

    ce_long result;
    __asm__ __volatile__(
	 	"lock\nxaddq %0,%1"
	  : "=r"(result), "=m"(*v)
	  : "0"(delta)
	  : "memory");

   return result + delta;

#endif
}
#endif

/**************************************************************************************************/
    
CE_INLINE ce_int 
ceAtomicCompareAndSwapInt(
	ce_atomic_int *v, 
	ce_int new_value, 
	ce_int old_value) 
{

#if defined(CE_PLATFORM_WINDOWS)

    return InterlockedCompareExchange(v, new_value, old_value);

#elif defined(CE_PLATFORM_MACOSX) && defined(CE_ARCH_PPC)

    return OSAtomicCompareAndSwap32Barrier(old_value, new_value, v);

#else

    ce_int result;

    __asm__ __volatile__(
    	"lock\ncmpxchgl %2,%1"
	  : "=a"(result), "=m"(*v)
	  : "q"(new_value), "0"(old_value)
	  : "memory");

    return result;
#endif
}

/**************************************************************************************************/

#if defined(CE_64BIT)
CE_INLINE ce_long 
ceAtomicCompareAndSwapLong(
	ce_atomic_long *v, 
	ce_long new_value, 
	ce_long old_value) 
{

#if defined(CE_PLATFORM_WINDOWS)

    return InterlockedCompareExchange64(v, new_value, old_value);

#elif defined(CE_PLATFORM_MACOSX) && defined(CE_ARCH_PPC)

    return OSAtomicCompareAndSwap64Barrier(old_value, new_value, v);

#else

    ce_long result;

    __asm__ __volatile__(
    	"lock\ncmpxchgq %2,%1"
 	  : "=a"(result), "=m"(*v)
	  : "q"(new_value), "0"(old_value)
	  : "memory");
	  
    return result;

#endif
}
#endif /* CE_64BIT */

/**************************************************************************************************/

CE_INLINE void*
ceAtomicCompareAndSwapPtr(
	void **v, 
	void *new_value, 
	void *old_value) 
{

#if defined(CE_PLATFORM_WINDOWS)

    return InterlockedCompareExchange(v, new_value, old_value);

#elif defined(CE_PLATFORM_MACOSX) && defined(CE_ARCH_PPC)
  #if defined(CE_64BIT)

    return OSAtomicCompareAndSwap64Barrier(old_value, new_value, v);

  #else

    return OSAtomicCompareAndSwap32Barrier(old_value, new_value, v);

  #endif
#else

    void *result;

#if defined(CE_64BIT)

    __asm__ __volatile__(
    	"lock\ncmpxchgq %2,%1"
	  : "=a"(result), "=m"(*v)
	  : "q"(new_value), "0"(old_value)
	  : "memory");

#else

    __asm__ __volatile__(
    	"lock\ncmpxchgl %2,%1"
	  : "=a"(result), "=m"(*v)
	  : "q"(new_value), "0"(old_value)
	  : "memory");

#endif /* CE_64BIT */

    return result;
    
#endif
}

/**************************************************************************************************/

CE_INLINE ce_float 
ceAtomicAddFloat(
	volatile ce_float *val, 
	ce_float delta) 
{
    typedef union _ce_bits_t { ce_float f; ce_int i; } ce_bits_t;
    ce_bits_t old_value, new_value;
    do {

#if defined(CE_ARCH_X86)
        __asm__ __volatile__ ("pause\n");
#endif
        old_value.f = *val;
        new_value.f = old_value.f + delta;

    } while (ceAtomicCompareAndSwapInt(((ce_atomic_int *)val), new_value.i, old_value.i) != old_value.i);
    
    return new_value.f;
}

/**************************************************************************************************/

CE_EXTERN_C_END

/**************************************************************************************************/

#endif /* __CE_ATOMICS_H__ */
