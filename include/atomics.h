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

#ifndef __SC_ATOMICS_H__
#define __SC_ATOMICS_H__

/**************************************************************************************************/

#include "platform.h"
#include "types.h"
#include "status.h"

/**************************************************************************************************/

SC_EXTERN_C_BEGIN

/**************************************************************************************************/

#if defined(SC_PLATFORM_OSX) && defined(SC_ARCH_PPC)
	#include <libkern/OSAtomic.h>
#endif

#if defined(SC_USE_PTHREADS)
	#include <pthread.h>
	#include <semaphore.h>
#endif

#if defined(SC_PLATFORM_WINDOWS)

	typedef volatile LONG 			sc_atomic_int;
	#if defined(SC_64BIT)
		typedef volatile LONGLONG 	sc_atomic_long;
	#endif

	#if _(MSC_VER >= 1300)
		extern "C" void _ReadWriteBarrier();
		#pragma intrinsic(_ReadWriteBarrier)
	#else
		#define _ReadWriteBarrier()
	#endif

#else

	typedef volatile sc_int 		sc_atomic_int;
	#if defined(SC_64BIT)
		typedef volatile sc_long 	sc_atomic_long;
	#endif

#endif

/**************************************************************************************************/

SC_INLINE sc_int 
scAtomicAddInt(
	sc_atomic_int *v, 
	sc_int delta) 
{
#if defined(SC_PLATFORM_WINDOWS)
	#if defined(SC_64BIT)
    	
    	return InterlockedAdd(v, delta);
    	
	#else
		
		sc_int result;
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
#elif defined(SC_PLATFORM_MACOSX) && defined(SC_ARCH_PPC)

    return OSAtomicAdd32Barrier(delta, v);

#else
    
    sc_int result;
    
    __asm__ __volatile__(
    	"lock\n"
		"xaddl %0,%1"
	   : "=r"(result), "=m"(*v) : "0"(delta)
	   : "memory");
                         
    return result + delta;
    
#endif
}


/**************************************************************************************************/

#if defined(SC_64BIT)

SC_INLINE sc_long 
scAtomicAddLong(
	sc_atomic_long *v, 
	sc_long delta) 
{
#if defined(SC_PLATFORM_WINDOWS)

    return InterlockedAdd64(v, delta);

#elif defined(SC_PLATFORM_MACOSX) && defined(SC_ARCH_PPC)

    return OSAtomicAdd64Barrier(delta, v);

#else

    sc_long result;
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
    
SC_INLINE sc_int 
scAtomicCompareAndSwapInt(
	sc_atomic_int *v, 
	sc_int new_value, 
	sc_int old_value) 
{

#if defined(SC_PLATFORM_WINDOWS)

    return InterlockedCompareExchange(v, new_value, old_value);

#elif defined(SC_PLATFORM_MACOSX) && defined(SC_ARCH_PPC)

    return OSAtomicCompareAndSwap32Barrier(old_value, new_value, v);

#else

    sc_int result;

    __asm__ __volatile__(
    	"lock\ncmpxchgl %2,%1"
	  : "=a"(result), "=m"(*v)
	  : "q"(new_value), "0"(old_value)
	  : "memory");

    return result;
#endif
}

/**************************************************************************************************/

#if defined(SC_64BIT)
SC_INLINE sc_long 
scAtomicCompareAndSwapLong(
	sc_atomic_long *v, 
	sc_long new_value, 
	sc_long old_value) 
{

#if defined(SC_PLATFORM_WINDOWS)

    return InterlockedCompareExchange64(v, new_value, old_value);

#elif defined(SC_PLATFORM_MACOSX) && defined(SC_ARCH_PPC)

    return OSAtomicCompareAndSwap64Barrier(old_value, new_value, v);

#else

    sc_long result;

    __asm__ __volatile__(
    	"lock\ncmpxchgq %2,%1"
 	  : "=a"(result), "=m"(*v)
	  : "q"(new_value), "0"(old_value)
	  : "memory");
	  
    return result;

#endif
}
#endif /* SC_64BIT */

/**************************************************************************************************/

SC_INLINE void*
scAtomicCompareAndSwapPtr(
	void **v, 
	void *new_value, 
	void *old_value) 
{

#if defined(SC_PLATFORM_WINDOWS)

    return InterlockedCompareExchange(v, new_value, old_value);

#elif defined(SC_PLATFORM_MACOSX) && defined(SC_ARCH_PPC)
  #if defined(SC_64BIT)

    return OSAtomicCompareAndSwap64Barrier(old_value, new_value, v);

  #else

    return OSAtomicCompareAndSwap32Barrier(old_value, new_value, v);

  #endif
#else

    void *result;

#if defined(SC_64BIT)

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

#endif /* SC_64BIT */

    return result;
    
#endif
}

/**************************************************************************************************/

SC_INLINE sc_float 
scAtomicAddFloat(
	volatile sc_float *val, 
	sc_float delta) 
{
    typedef union _sc_bits_t { sc_float f; sc_int i; } sc_bits_t;
    sc_bits_t old_value, new_value;
    do {

#if defined(SC_ARCH_X86)
        __asm__ __volatile__ ("pause\n");
#endif
        old_value.f = *val;
        new_value.f = old_value.f + delta;

    } while (scAtomicCompareAndSwapInt(((sc_atomic_int *)val), new_value.i, old_value.i) != old_value.i);
    
    return new_value.f;
}

/**************************************************************************************************/

SC_EXTERN_C_END

/**************************************************************************************************/

#endif /* __SC_ATOMICS_H__ */
