/* columba-compiler.h -- Compiler-specific definitions and macros
 * Copyright (c) 2014 James Thompson <tomahack@mail2tor.com>
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef COLUMBA_COMPILER_H
#define COLUMBA_COMPILER_H

#if !defined(COLUMBA_H_INSIDE)
	#error "Only <columba/columba.h> can be included directly."
#endif

#if defined(__cplusplus)
	#define COLUMBA_BEGIN_DECLS extern "C" {
	#define COLUMBA_END_DECLS   }
#else
	#define COLUMBA_BEGIN_DECLS
	#define COLUMBA_END_DECLS
#endif

#if defined(__GNUC__) && (__GNUC__ >= 4)
	#if defined(_WIN32) || defined(__CYGWIN__)
		#define COLUMBA_PUBLIC __attribute__((__dllexport__))
		#define COLUMBA_HIDDEN __attribute__((__dllimport__))
	#else
		#define COLUMBA_PUBLIC __attribute__((visibility ("default")))
		#define COLUMBA_HIDDEN __attribute__((visibility ("hidden")))
	#endif
#else
	#if defined(_WIN32) || defined(__CYGWIN__)
		#define COLUMBA_PUBLIC __declspec (dllexport)
		#define COLUMBA_HIDDEN __declspec (dllimport)
	#else
		#define COLUMBA_PUBLIC
		#define COLUMBA_HIDDEN
	#endif
#endif

#if defined(__GNUC__)
	#define COLUMBA_GCC_VERSION_NUMBER(major, minor, patch) (((major) << 16) | ((minor) << 8) | (patch))
	#if defined(__GNUC_PATCHLEVEL__)
		#define COLUMBA_GCC_VERSION_CURRENT COLUMBA_GCC_VERSION_NUMBER (__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__)
	#else
		#define COLUMBA_GCC_VERSION_CURRENT COLUMBA_GCC_VERSION_NUMBER (__GNUC__, __GNUC_MINOR__, 0)
	#endif
	#define COLUMBA_GCC_VERSION_FULL(major, minor, patch) COLUMBA_GCC_VERSION_CURRENT >= COLUMBA_GCC_VERSION_NUMBER (major, minor, patch)
	#define COLUMBA_GCC_VERSION(major, minor) COLUMBA_GCC_VERSION_CURRENT >= COLUMBA_GCC_VERSION_NUMBER (major, minor, 0)

	#define COLUMBA_WARN_UNUSED_RESULT __attribute__((__warn_unused_result__))
	#define COLUMBA_USED               __attribute__((__used__))
	#define COLUMBA_UNUSED             __attribute__((__unused__))
	#define COLUMBA_SENTINEL           __attribute__((__sentinel__))

	#if COLUMBA_GCC_VERSION (4, 8)
		#define COLUMBA_COLD_LABEL       __attribute__((__cold__))
		#define COLUMBA_HOT_LABEL        __attribute__((__hot__))
	#else
		#define COLUMBA_COLD_LABEL
		#define COLUMBA_HOT_LABEL
	#endif

	#if COLUMBA_GCC_VERSION (4, 3)
		#define COLUMBA_COLD             __attribute__((__cold__))
		#define COLUMBA_HOT              __attribute__((__hot__))
	#else
		#define COLUMBA_COLD
		#define COLUMBA_HOT
	#endif

	#if COLUMBA_GCC_VERSION (2, 96)
		#define COLUMBA_PURE             __attribute__((__pure__))
	#else
		#define COLUMBA_PURE
	#endif

	#if COLUMBA_GCC_VERSION (2, 5)
		#define COLUMBA_NO_RETURN        __attribute__((__noreturn__))
	#else
		#define COLUMBA_NO_RETURN
	#endif

	#if COLUMBA_GCC_VERSION (4, 9) && defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
		#if defined(NDEBUG)
			#define COLUMBA_NON_NULL(...)  __attribute__((nonnull (__VA_ARGS__)))
		#else
			#define COLUMBA_NON_NULL(...)  __attribute__((nonnull (__VA_ARGS__))) __attribute__((optimize ("no-isolate-erroneous-paths-attribute")))
		#endif
	#else
		#define COLUMBA_NON_NULL(...)
	#endif

	#define COLUMBA_MALLOC             __attribute__((__malloc__))
	#define COLUMBA_CONST              __attribute__((__const__))
	#define COLUMBA_ALLOC_SIZE(arg)    __attribute__((__alloc_size__ (arg)))

	#define COLUMBA_LIKELY(expr)       __builtin_expect ((expr), true)
	#define COLUMBA_UNLIKELY(expr)     __builtin_expect ((expr), false)
	#define COLUMBA_TRAP()             __builtin_trap ()
#else
	#define COLUMBA_WARN_UNUSED_RESULT
	#define COLUMBA_USED
	#define COLUMBA_UNUSED
	#define COLUMBA_SENTINEL
	#define COLUMBA_COLD
	#define COLUMBA_HOT
	#define COLUMBA_COLD_LABEL
	#define COLUMBA_HOT_LABEL
	#define COLUMBA_PURE
	#define COLUMBA_NO_RETURN
	#define COLUMBA_NON_NULL(...)
	#define COLUMBA_MALLOC
	#define COLUMBA_CONST
	#define COLUMBA_ALLOC_SIZE(arg)

	#define COLUMBA_LIKELY(expr) (expr)
	#define COLUMBA_UNLIKELY(expr) (expr)
	#define COLUMBA_TRAP()
#endif

#endif /* !defined(COLUMBA_COMPILER_H) */
