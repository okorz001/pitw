#ifndef PITW_PRIVATE_ATOMIC_H
#define PITW_PRIVATE_ATOMIC_H

#if defined(PITW_HAS_C11_ATOMIC)
#  include <stdatomic.h>
typedef _Atomic unsigned int pitw_atomic;
/* Note C11 only provides fetch_add, not add_fetch */
#  define pitw_atomic_inc(x) (atomic_fetch_add(&(x), 1) + 1)
#  define pitw_atomic_dec(x) (atomic_fetch_sub(&(x), 1) - 1)
#elif defined(PITW_HAS_GCC_ATOMIC)
typedef unsigned int pitw_atomic;
#  define pitw_atomic_inc(x) (__atomic_add_fetch(&(x), 1, __ATOMIC_SEQ_CST))
#  define pitw_atomic_dec(x) (__atomic_sub_fetch(&(x), 1, __ATOMIC_SEQ_CST))
#elif defined(PITW_HAS_GCC_SYNC)
typedef unsigned int pitw_atomic;
#  define pitw_atomic_inc(x) (__sync_add_and_fetch(&(x), 1))
#  define pitw_atomic_dec(x) (__sync_sub_and_fetch(&(x), 1))
#elif defined(PITW_HAS_WIN32_INTERLOCKED)
#  include <windows.h>
typedef volatile LONG pitw_atomic;
#  define pitw_atomic_inc(x) (InterlockedIncrement(&x))
#  define pitw_atomic_dec(x) (InterlockedDecrement(&x))
#else
#  error "No atomic support"
#endif

#endif /* PITW_PRIVATE_ATOMIC_H */
