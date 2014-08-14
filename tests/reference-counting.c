#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <columba/columba-internal.h>

#include <stdlib.h>

typedef struct ColumbaSimpleTestObject_ {
	bool* freed;
} ColumbaSimpleTestObject;

static void
columba_simple_test_object_destroy (ColumbaSimpleTestObject* obj);

static void
columba_simple_test_object_init (ColumbaSimpleTestObject* obj,
                                 bool                     is_floating,
                                 bool*                    freed);

static ColumbaSimpleTestObject*
columba_simple_test_object_new (bool  floater,
                                bool* freed);


static void
columba_simple_test_object_destroy (ColumbaSimpleTestObject* obj) {
	assert_false (*(obj->freed));
	*(obj->freed) = true;
}

static void
columba_simple_test_object_init (ColumbaSimpleTestObject* obj,
                                 bool                     is_floating,
                                 bool*                    freed) {
	columba_object_init (obj, is_floating, (ColumbaDestroyNotify) columba_simple_test_object_destroy);

	obj->freed = freed;
	*freed = false;
}

static ColumbaSimpleTestObject*
columba_simple_test_object_new (bool  floater,
                                bool* freed) {
	ColumbaSimpleTestObject* obj = columba_object_new (ColumbaSimpleTestObject);

	columba_simple_test_object_init (obj, floater, freed);

	return obj;
}

static void
simple_refcounting (void** state COLUMBA_UNUSED) {
	bool freed = false;
	ColumbaSimpleTestObject* obj = columba_simple_test_object_new (false, &freed);

	assert_non_null (obj);
	assert_false (columba_object_is_floating (obj));
	assert_int_equal (columba_object_get_ref_count (obj), 1);
	assert_false (columba_object_ref_sink (obj));
	assert_int_equal (columba_object_get_ref_count (obj), 1);
	assert_true (columba_object_ref (obj) == obj);
	assert_int_equal (columba_object_get_ref_count (obj), 2);
	assert_null (columba_object_unref (obj));
	assert_int_equal (columba_object_get_ref_count (obj), 1);
	assert_false (freed);
	assert_null (columba_object_unref (obj));
	assert_true (freed);
}

static void
simple_floating (void** state COLUMBA_UNUSED) {
	ColumbaSimpleTestObject* obj;
	bool freed;

	/* Create an object, sink the floating reference, unref */
	obj = columba_simple_test_object_new (true, &freed);
	assert_non_null (obj);
	assert_true (columba_object_is_floating (obj));
	assert_int_equal (columba_object_get_ref_count (obj), 1);
	assert_true (columba_object_ref_sink (obj));
	assert_int_equal (columba_object_get_ref_count (obj), 1);
	assert_false (freed);
	assert_null (columba_object_unref (obj));
	assert_true (freed);

	/* Make sure ref() sinks the floating reference instead of incrementing the
	   reference count. */
	obj = columba_simple_test_object_new (true, &freed);
	assert_true (columba_object_is_floating (obj));
	assert_int_equal (columba_object_get_ref_count (obj), 1);
	assert_true (columba_object_ref (obj) == obj);
	assert_int_equal (columba_object_get_ref_count (obj), 1);
	assert_false (freed);
	assert_null (columba_object_unref (obj));
	assert_true (freed);
}

int
main (void) {
	const UnitTest tests[] = {
		unit_test (simple_refcounting),
		unit_test (simple_floating),
	};

	return run_tests (tests);
}
