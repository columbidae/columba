/* columba-object.h -- Reference-counting base object type
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

#if !defined(COLUMBA_OBJECT_H)
#define COLUMBA_OBJECT_H

#if !defined (COLUMBA_H_INSIDE) && !defined (COLUMBA_COMPILATION)
	#error "Only <columba/columba.h> can be included directly."
#endif

#include <stddef.h>

COLUMBA_BEGIN_DECLS

typedef void (* ColumbaDestroyNotify) (void* value);

COLUMBA_AVAILABLE_IN_ALL void
columba_object_init          (void*                obj,
                              bool                 is_floating,
                              ColumbaDestroyNotify destroy_notify) COLUMBA_NON_NULL (1);

COLUMBA_AVAILABLE_IN_ALL void*
columba_object_allocate      (size_t size);

#define columba_object_new(T) ((T*) columba_object_allocate (sizeof (T)))

COLUMBA_AVAILABLE_IN_ALL void*
columba_object_ref           (void* obj) COLUMBA_NON_NULL (1);

COLUMBA_AVAILABLE_IN_ALL void*
columba_object_unref         (void* obj) COLUMBA_NON_NULL (1);

COLUMBA_AVAILABLE_IN_ALL bool
columba_object_ref_sink      (void* obj) COLUMBA_NON_NULL (1);

COLUMBA_AVAILABLE_IN_ALL bool
columba_object_is_floating   (void* obj) COLUMBA_NON_NULL (1);

COLUMBA_AVAILABLE_IN_ALL unsigned int
columba_object_get_ref_count (void* obj) COLUMBA_NON_NULL (1);

COLUMBA_END_DECLS

#endif /* !defined(COLUMBA_OBJECT_H) */
