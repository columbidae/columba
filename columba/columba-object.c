/* columba-object.c -- Reference-counting base object type
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

#include <columba/columba-internal.h>

#include <limits.h>
#include <stdlib.h>
#include <assert.h>

/**
 * @defgroup columba-object Object
 * @brief A reference-counting base object from which other types in Columba are derived
 * @{
 */

/** @cond INTERNAL */

typedef struct ColumbaObject_ {
	atomic_uint ref_count;
	atomic_bool is_floating;
	ColumbaDestroyNotify destroy_notify;
} ColumbaObject;

/* We want to allow extending the base object type in plugins, so the API has to
   be public.  Unfortunately, we can't make the atomic fields public without
   also making at least part of the atomic API public, which I don't want to do
   since they should really just be C11 atomics.

   The best solution I could think of is returning a pointer to the middle of
   the allocated memory which is a little evil... */

static const size_t columba_object_offset =
    ((sizeof (ColumbaObject) / sizeof (void*)) * sizeof (void*)) +
    (((sizeof (ColumbaObject) % sizeof (void*)) == 0) ? 0 : sizeof (void*));

static inline ColumbaObject*
columba_object_from_pointer (void* ptr) {
	return (ColumbaObject*) (((uint8_t*) ptr) - columba_object_offset);
}

static inline void*
columba_object_to_pointer (ColumbaObject* obj) {
	return (void*) (((uint8_t*) obj) + columba_object_offset);
}

/** @endcond */

void*
columba_object_allocate (size_t size) {
	ColumbaObject* obj = (ColumbaObject*) malloc (size + columba_object_offset);

	if (obj == NULL)
		return NULL;

	return columba_object_to_pointer (obj);
}

/**
 * @brief Initialize an object
 *
 * Keep in mind that the @a destroy_notify callback is for deinitialization, but
 * *not* for actually freeing the value.  free() is invoked automatically after
 * @a destroy_notify returns.
 *
 * @param obj The object to initialize
 * @param is_floating Whether the initial reference should be floating
 * @param destroy_notify Function to be invoked with the reference count reaches
 *   0, or @a NULL
 */
void
columba_object_init (void*                obj,
                     bool                 is_floating,
                     ColumbaDestroyNotify destroy_notify) {
	ColumbaObject* o = columba_object_from_pointer (obj);

	o->ref_count = 1;
	o->is_floating = is_floating;
	o->destroy_notify = destroy_notify;
}

/**
 * @brief Add a reference
 *
 * Adds a reference to the specified object.
 *
 * @param obj The object to add a reference to
 * @return The object (@a obj)
 */
void*
columba_object_ref (void* obj) {
	ColumbaObject* o = columba_object_from_pointer (obj);
	bool is_floating = atomic_load_explicit (&(o->is_floating), memory_order_acquire);

	if (COLUMBA_UNLIKELY (o->is_floating))
		if (atomic_compare_exchange_strong (&(o->is_floating), &is_floating, false))
			return obj;

	unsigned int ref_count = atomic_fetch_add_explicit (&(o->ref_count), 1, memory_order_relaxed);
	assert (ref_count < UINT_MAX);

	return obj;
}

/**
 * @brief Remove a reference
 *
 * Remove a reference to the specified object and, if it was the last reference,
 * destroy the object.
 *
 * @param obj The object to remove a reference from
 * @return @a NULL
 */
void*
columba_object_unref (void* obj) {
	ColumbaObject* o = columba_object_from_pointer (obj);
	unsigned int old = atomic_fetch_sub (&(o->ref_count), 1);

	assert (old != 0);

	if (old == 1) {
		if (o->destroy_notify != NULL)
			o->destroy_notify (obj);

		free (o);
	}

	return NULL;
}

/**
 * @brief Sink a floating reference
 *
 * @param obj The object on which to sink the floating reference
 * @retval true A floating reference was sunk
 * @retval false No floating reference to sink
 */
bool
columba_object_ref_sink (void* obj) {
	ColumbaObject* o = columba_object_from_pointer (obj);
	bool is_floating = atomic_load_explicit (&(o->is_floating), memory_order_acquire);

	if (COLUMBA_LIKELY (o->is_floating))
		if (atomic_compare_exchange_strong (&(o->is_floating), &is_floating, false))
			return true;

	return false;
}

/**
 * @brief Check whether an object has a floating reference
 *
 * @param obj The object to check
 * @return Whether or not the object has a floating reference
 */
bool
columba_object_is_floating (void* obj) {
	ColumbaObject* o = columba_object_from_pointer (obj);

	return atomic_load (&(o->is_floating));
}

/**
 * @brief Get the reference count of an object
 *
 * @param obj The object
 * @return The reference count
 */
unsigned int
columba_object_get_ref_count (void* obj) {
	ColumbaObject* o = columba_object_from_pointer (obj);

	return atomic_load (&(o->ref_count));
}

/**
 * @}
 */
