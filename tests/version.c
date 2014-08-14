#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <columba/columba-internal.h>

#include <stdlib.h>

static void
test_version (void** state COLUMBA_UNUSED) {
	unsigned int version = columba_version ();

	assert_true (version == COLUMBA_VERSION_CURRENT);
	assert_true (COLUMBA_VERSION_EXTRACT_MAJOR (version) == COLUMBA_VERSION_MAJOR);
	assert_true (COLUMBA_VERSION_EXTRACT_MINOR (version) == COLUMBA_VERSION_MINOR);
	assert_true (COLUMBA_VERSION_EXTRACT_MICRO (version) == COLUMBA_VERSION_MICRO);
}

int
main (void) {
	const UnitTest tests[] = {
		unit_test (test_version),
	};

	return run_tests (tests);
}
