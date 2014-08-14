/* columba-internal.h -- Single include internal header
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

#if defined(COLUMBA_H_INSIDE)
	#error Attempted to include internal header in public header
#endif

#if !defined(COLUMBA_INTERNAL_H)
	#define COLUMBA_INTERNAL_H
	#define COLUMBA_INTERNAL_H_INSIDE

	#include "columba.h"

	#include "config.h"

	#ifdef HAVE_STDATOMIC_H
		#include <stdatomic.h>
	#else
		#include "columba-atomic-internal.h"
	#endif

	#undef COLUMBA_INTERNAL_H_INSIDE

#endif /* !defined(COLUMBA_INTERNAL_H) */
