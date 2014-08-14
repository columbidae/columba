/* columba-atomic.h
 * ----------------
 * Thin wrapper around compiler-specific atomic functionality to
 * implement the C11 stdatomic.h API.
 *
 * The API is from the C standard, and the mappings to the various
 * compiler-specific functionality are pretty straightforward, so
 * honestly I'm not sure how copyrightable this file is.  That said,
 * in case anything in here is copyrightable:
 *
 *   To the extent possible under law, the authors of this file waive
 *   all copyright and related or neighboring rights to this work.
 */

#ifndef COLUMBA_ATOMIC_H
#define COLUMBA_ATOMIC_H

#define ATOMICS_STYLE_GCC   1
#define ATOMICS_STYLE_CLANG 2

#if defined(__clang__)
	#if __has_extension (c_atomic)
		#define ATOMICS_STYLE ATOMICS_STYLE_CLANG
	#endif
#elif defined(__GNUC__) && ((__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7))
	#define ATOMICS_STYLE ATOMICS_STYLE_GCC
#endif

#if !defined(ATOMICS_STYLE)
	#error No known atomic implementation
#endif

#include <stdint.h>
#include <stddef.h>

#if ATOMICS_STYLE == ATOMICS_STYLE_GCC
	#define _Atomic(T) T
#endif

typedef _Atomic (_Bool)              atomic_bool;
typedef _Atomic (char)               atomic_char;
typedef _Atomic (unsigned char)      atomic_uchar;
typedef _Atomic (short)              atomic_short;
typedef _Atomic (unsigned short)     atomic_ushort;
typedef _Atomic (int)                atomic_int;
typedef _Atomic (unsigned int)       atomic_uint;
typedef _Atomic (long)               atomic_long;
typedef _Atomic (unsigned long)      atomic_ulong;
typedef _Atomic (long long)          atomic_llong;
typedef _Atomic (unsigned long long) atomic_ullong;
/* typedef _Atomic (char16_t)           atomic_char16_t; */
/* typedef _Atomic (char32_t)           atomic_char32_t; */
typedef _Atomic (wchar_t)            atomic_wchar_t;
typedef _Atomic (int_least8_t)       atomic_int_least8_t;
typedef _Atomic (uint_least8_t)      atomic_uint_least8_t;
typedef _Atomic (int_least16_t)      atomic_int_least16_t;
typedef _Atomic (uint_least16_t)     atomic_uint_least16_t;
typedef _Atomic (int_least32_t)      atomic_int_least32_t;
typedef _Atomic (uint_least32_t)     atomic_uint_least32_t;
typedef _Atomic (int_least64_t)      atomic_int_least64_t;
typedef _Atomic (uint_least64_t)     atomic_uint_least64_t;
typedef _Atomic (int_fast8_t)        atomic_int_fast8_t;
typedef _Atomic (uint_fast8_t)       atomic_uint_fast8_t;
typedef _Atomic (int_fast16_t)       atomic_int_fast16_t;
typedef _Atomic (uint_fast16_t)      atomic_uint_fast16_t;
typedef _Atomic (int_fast32_t)       atomic_int_fast32_t;
typedef _Atomic (uint_fast32_t)      atomic_uint_fast32_t;
typedef _Atomic (int_fast64_t)       atomic_int_fast64_t;
typedef _Atomic (uint_fast64_t)      atomic_uint_fast64_t;
typedef _Atomic (intptr_t)           atomic_intptr_t;
typedef _Atomic (uintptr_t)          atomic_uintptr_t;
typedef _Atomic (size_t)             atomic_size_t;
typedef _Atomic (ptrdiff_t)          atomic_ptrdiff_t;
typedef _Atomic (intmax_t)           atomic_intmax_t;
typedef _Atomic (uintmax_t)          atomic_uintmax_t;

#if ATOMICS_STYLE == ATOMICS_STYLE_CLANG
	#define atomic_is_lock_free(obj)            __c11_atomic_is_lock_free (sizeof (*obj))
	#define columba_atomic_type_is_lock_free(T) __c11_atomic_is_lock_free (sizeof (T))
#elif ATOMICS_STYLE == ATOMICS_STYLE_GCC
	#define atomic_is_lock_free(obj)            __atomic_is_lock_free (sizeof (*obj), obj)
	#define columba_atomic_type_is_lock_free(T) __atomic_is_lock_free (sizeof (T), NULL)
#endif

#define ATOMIC_BOOL_LOCK_FREE     columba_atomic_type_is_lock_free (_Bool)
#define ATOMIC_CHAR_LOCK_FREE     columba_atomic_type_is_lock_free (char)
#define ATOMIC_CHAR16_T_LOCK_FREE columba_atomic_type_is_lock_free (char16_t)
#define ATOMIC_CHAR32_T_LOCK_FREE columba_atomic_type_is_lock_free (char32_t)
#define ATOMIC_WCHAR_T_LOCK_FREE  columba_atomic_type_is_lock_free (wchar_t)
#define ATOMIC_SHORT_LOCK_FREE    columba_atomic_type_is_lock_free (short)
#define ATOMIC_INT_LOCK_FREE      columba_atomic_type_is_lock_free (int)
#define ATOMIC_LONG_LOCK_FREE     columba_atomic_type_is_lock_free (long)
#define ATOMIC_LLONG_LOCK_FREE    columba_atomic_type_is_lock_free (long long)
#define ATOMIC_POINTER_LOCK_FREE  columba_atomic_type_is_lock_free (void*)

typedef enum memory_order {
	memory_order_relaxed = __ATOMIC_RELAXED,
	memory_order_consume = __ATOMIC_CONSUME,
	memory_order_acquire = __ATOMIC_ACQUIRE,
	memory_order_release = __ATOMIC_RELEASE,
	memory_order_acq_rel = __ATOMIC_ACQ_REL,
	memory_order_seq_cst = __ATOMIC_SEQ_CST
} memory_order;

typedef atomic_bool atomic_flag;

#if ATOMICS_STYLE == ATOMICS_STYLE_CLANG
	#define ATOMIC_VAR_INIT(value) (value)
	#define atomic_init(obj, value) __c11_atomic_init (obj, (value))
#elif ATOMICS_STYLE == ATOMICS_STYLE_GCC
	#define ATOMIC_VAR_INIT(value) (value)
	#define atomic_init(obj, value) (*obj = (value))
#endif

#define ATOMIC_FLAG_INIT ATOMIC_VAR_INIT (false)

#if ATOMICS_STYLE == ATOMICS_STYLE_CLANG
	#define  atomic_thread_fence(order)  __c11_atomic_thread_fence (order)
	#define  atomic_signal_fence(order)  __c11_atomic_signal_fence (order)
#elif ATOMICS_STYLE == ATOMICS_STYLE_GCC
	#define  atomic_thread_fence(order)  __atomic_thread_fence (order)
	#define  atomic_signal_fence(order)  __atomic_signal_fence (order)
#endif

#if ATOMICS_STYLE == ATOMICS_STYLE_CLANG
	#define atomic_is_lock_free(obj) __c11_atomic_is_lock_free (sizeof (*obj))
#elif ATOMICS_STYLE == ATOMICS_STYLE_GCC
	#define atomic_is_lock_free(obj) __atomic_is_lock_free (sizeof (*obj), obj)
#endif

#if ATOMICS_STYLE == ATOMICS_STYLE_CLANG
	#define atomic_store_explicit(object, desired, order) \
	__c11_atomic_store (object, desired, order)
	#define atomic_load_explicit(object, order) \
	__c11_atomic_load (object, order)
	#define atomic_exchange_explicit(object, desired, order) \
	__c11_atomic_exchange (object, desired, order)
	#define atomic_compare_exchange_strong_explicit(object, expected, desired, success, failure) \
	__c11_atomic_compare_exchange_strong (object, expected, desired, success, failure)
	#define atomic_compare_exchange_weak_explicit(object, expected, desired, success, failure) \
	__c11_atomic_compare_exchange_weak (object, expected, desired, success, failure)
	#define  atomic_fetch_add_explicit(object, operand, order) \
	__c11_atomic_fetch_add (object, operand, order)
	#define  atomic_fetch_sub_explicit(object, operand, order) \
	__c11_atomic_fetch_sub (object, operand, order)
	#define  atomic_fetch_or_explicit(object, operand, order) \
	__c11_atomic_fetch_or (object, operand, order)
	#define  atomic_fetch_xor_explicit(object, operand, order) \
	__c11_atomic_fetch_xor (object, operand, order)
	#define  atomic_fetch_and_explicit(object, operand, order) \
	__c11_atomic_fetch_and (object, operand, order)
	#define atomic_flag_test_and_set_explicit(object, order) \
	__c11_atomic_compare_exchange_strong (object, 0, 1, order, order)
	#define atomic_flag_clear_explicit(object, order) \
	__c11_atomic_store (object, 0, order)
#elif ATOMICS_STYLE == ATOMICS_STYLE_GCC
	#define  atomic_store_explicit(object, desired, order) \
	__atomic_store_n (object, desired, order)
	#define atomic_load_explicit(object, order) \
	__atomic_load_n (object, order)
	#define atomic_exchange_explicit(object, desired, order) \
	__atomic_exchange_n (object, desired, order)
	#define atomic_compare_exchange_strong_explicit(object, expected, desired, success, failure) \
	__atomic_compare_exchange_n (object, expected, desired, false, success, failure)
	#define atomic_compare_exchange_weak_explicit(object, expected, desired, success, failure) \
	__atomic_compare_exchange_n (object, expected, desired, true, success, failure)
	#define  atomic_fetch_add_explicit(object, operand, order) \
	__atomic_fetch_add (object, operand, order)
	#define  atomic_fetch_sub_explicit(object, operand, order) \
	__atomic_fetch_sub (object, operand, order)
	#define  atomic_fetch_and_explicit(object, operand, order) \
	__atomic_fetch_and (object, operand, order)
	#define  atomic_fetch_xor_explicit(object, operand, order) \
	__atomic_fetch_xor (object, operand, order)
	#define  atomic_fetch_or_explicit(object, operand, order) \
	__atomic_fetch_or (object, operand, order)
	#define atomic_flag_test_and_set_explicit(object, order) \
	__atomic_flag_test_and_set (object, order)
	#define atomic_flag_clear_explicit(object, order) \
	__atomic_clear (object, order)
#endif

#define atomic_store(object, desired) \
	atomic_store_explicit (object, desired, memory_order_seq_cst)
#define atomic_load(object) \
	atomic_load_explicit (object, memory_order_seq_cst)
#define atomic_exchange(object, desired) \
	atomic_exchange_explicit (object, desired, memory_order_seq_cst)
#define atomic_compare_exchange_strong(object, expected, desired) \
	atomic_compare_exchange_strong_explicit (object, expected, desired, memory_order_seq_cst, memory_order_seq_cst)
#define atomic_compare_exchange_weak(object, expected, desired) \
	atomic_compare_exchange_weak_explicit (object, expected, desired, memory_order_seq_cst, memory_order_seq_cst)
#define  atomic_fetch_add(object, operand) \
	atomic_fetch_add_explicit (object, operand, memory_order_seq_cst)
#define  atomic_fetch_sub(object, operand) \
	atomic_fetch_sub_explicit (object, operand, memory_order_seq_cst)
#define  atomic_fetch_and(object, operand) \
	atomic_fetch_and_explicit (object, operand, memory_order_seq_cst)
#define  atomic_fetch_or(object, operand) \
	atomic_fetch_or_explicit (object, operand, memory_order_seq_cst)
#define  atomic_fetch_xor(object, operand) \
	atomic_fetch_xor_explicit (object, operand, memory_order_seq_cst)

#endif /* COLUMBA_ATOMIC_H */
