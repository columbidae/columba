if ENABLE_TESTS

NULL =

if ENABLE_VALGRIND
LOG_COMPILER = $(LIBTOOL) --mode=execute $(VALGRIND) -q --error-exitcode=1 --leak-check=full

TESTS_ENVIRONMENT = VALGRIND='$(LOG_COMPILER)'
endif

AM_CFLAGS = \
	$(C99_FLAG) \
	$(FATAL_WARNING_CFLAGS) \
	-DCOLUMBA_COMPILATION \
	-DCOLUMBA_INTERNAL=COLUMBA_PUBLIC \
	-I$(top_srcdir) \
	$(COLUMBA_WARNING_CFLAGS) \
	$(CMOCKA_CFLAGS) \
	$(PTHREAD_CFLAGS) \
	$(NULL)

LDADD = \
	$(top_builddir)/columba/libcolumba-@COLUMBA_API_VERSION@.la \
	$(CMOCKA_LIBS) \
	$(PTHREAD_LIBS) \
	$(NULL)

TESTS = \
	reference-counting \
	version \
	$(NULL)

noinst_PROGRAMS = $(TESTS)

endif

-include $(top_srcdir)/git.mk
