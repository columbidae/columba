NULL =

EXTRA_DIST =

lib_LTLIBRARIES = \
	libcolumba-@COLUMBA_API_VERSION@.la \
	$(NULL)

columbaincludedir = $(includedir)/columba/@COLUMBA_API_VERSION@/columba
columbainclude_HEADERS = \
	columba.h \
	columba-object.h \
	columba-version.h \
	columba-compiler.h \
	$(NULL)

COLUMBA_INTERNAL_HDRS = \
	columba-internal.h \
	columba-atomic-internal.h \
	$(NULL)

EXTRA_DIST += $(COLUMBA_INTERNAL_HDRS)

libcolumba_@COLUMBA_API_VERSION@_la_SOURCES = \
	columba-object.c \
	columba-version.c \
	$(NULL)

# If we are building the unit tests then we have to expose all the internal
# stuff so we can get better coverage.
if ENABLE_TESTS
COLUMBA_INTERNAL_CFLAGS=-DCOLUMBA_INTERNAL=COLUMBA_PUBLIC
else
COLUMBA_INTERNAL_CFLAGS=-DCOLUMBA_INTERNAL=COLUMBA_HIDDEN
endif

libcolumba_@COLUMBA_API_VERSION@_la_CFLAGS = \
	$(C99_FLAG) \
	$(FATAL_WARNING_CFLAGS) \
	-DCOLUMBA_COMPILATION \
	-I$(top_srcdir) \
	$(COVERAGE_CFLAGS) \
	$(COLUMBA_WARNING_CFLAGS) \
	$(COLUMBA_INTERNAL_CFLAGS) \
	$(NULL)

libcolumba_@COLUMBA_API_VERSION@_la_LDFLAGS = \
	$(COVERAGE_LDFLAGS) \
	$(NULL)

GITIGNOREFILES =

MAINTAINERCLEANFILES = \
	config.h \
	stamp-h1 \
	$(NULL)

if ENABLE_API_DOCS
api-reference.stamp: Doxyfile $(libcolumba_@COLUMBA_API_VERSION@_la_SOURCES) $(columba_HEADERS) $(COLUMBA_INTERNAL_HDRS)
	$(AM_V_GEN) $(DOXYGEN) $< && touch $@

html-local: api-reference.stamp

MAINTAINERCLEANFILES += \
	api-reference.stamp \
	$(NULL)

GITIGNOREFILES += \
	api-reference \
	$(NULL)

maintainer-clean-local:
	-rm -rf api-reference
endif

-include $(top_srcdir)/git.mk
