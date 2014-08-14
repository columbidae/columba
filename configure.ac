#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

# Making releases:
#   COLUMBA_VERSION_MICRO += 1;
#   COLUMBA_INTERFACE_AGE += 1;
# if any functions have been added, set COLUMBA_INTERFACE_AGE to 0.
m4_define([columba_version_major], [0])
m4_define([columba_version_minor], [0])
m4_define([columba_version_micro], [1])
m4_define([columba_version],       [columba_version_major.columba_version_minor.columba_version_micro])
m4_define([columba_interface_age], [0])
m4_define([columba_api_version],   [0.1])
m4_define([columba_binary_age],    [m4_eval(100 * columba_minor_version + columba_micro_version)])

AC_PREREQ([2.68])
AC_INIT([Columba], [columba_version],
	[https://github.com/columbidae/columba/issues],
	[columba],
	[https://columbidae.github.io/columba])

AC_CONFIG_SRCDIR([columba/columba-object.c])
AC_CONFIG_HEADERS([columba/config.h])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([foreign dist-xz subdir-objects parallel-tests])
AM_SILENT_RULES([yes])

COLUMBA_VERSION_MAJOR=columba_version_major
COLUMBA_VERSION_MINOR=columba_version_minor
COLUMBA_VERSION_MICRO=columba_version_micro
AC_SUBST(COLUMBA_VERSION_MAJOR)
AC_SUBST(COLUMBA_VERSION_MINOR)
AC_SUBST(COLUMBA_VERSION_MICRO)
COLUMBA_API_VERSION=columba_api_version
AC_SUBST(COLUMBA_API_VERSION)

LT_INIT([disable-static])

AC_CANONICAL_HOST

# Checks for programs.
AC_PROG_CC

C99_FLAG=
AX_CHECK_COMPILE_FLAG([-std=c99], [
    C99_FLAG=-std=c99
  ], [
    AC_PROG_CC_C99
  ])
AC_SUBST([C99_FLAG])

AC_CHECK_PROGS([DOXYGEN], [doxygen])
AC_ARG_ENABLE([api-docs], AS_HELP_STRING([--disable-api-docs], [Build API documentation (doxygen)]),, [enable_api_docs=auto])
AS_CASE([$enable_api_docs],
  [auto], [AS_IF([test "x$DOXYGEN" != "x"], [enable_api_docs=yes], [enable_api_docs=no])],
  [yes], [AS_IF([test "x$DOXYGEN" = "x"], [AC_MSG_ERROR([doxygen is required to build API documentation])])])
AM_CONDITIONAL([ENABLE_API_DOCS], test "x$enable_api_docs" = "xyes")

AC_CHECK_PROGS([VALGRIND], [valgrind])
AC_ARG_ENABLE([valgrind], AS_HELP_STRING([--enable-valgrind], [Run unit tests in valgrind]), [
  AS_IF([test "x$enable_valgrind" = "xyes"], [
    AS_IF([test "x$VALGRIND" = "x"], [
      AC_MSG_ERROR([Unable to find valgrind])
    ])
  ])
], [enable_valgrind=no])
AM_CONDITIONAL([ENABLE_VALGRIND], test "x$enable_valgrind" = "xyes")

AC_TDD_GCOV
AC_SUBST(COVERAGE_CFLAGS)
AC_SUBST(COVERAGE_CXXFLAGS)
AC_SUBST(COVERAGE_LDFLAGS)

# Checks for libraries.
AX_PTHREAD

PKG_CHECK_MODULES([LIBEV], [libev],, [
    ORIGINAL_LIBS="$LIBS"
    AC_SEARCH_LIBS([ev_default_loop], [ev], [
        LIBEV_CFLAGS=
	LIBEV_LIBS=-lev
      ], [
        AC_MSG_ERROR([Unable to find libev])
      ])
    LIBS="$ORIGINAL_LIBS"
  ])
AC_SUBST([LIBEV_CFLAGS])
AC_SUBST([LIBEV_LIBS])

AC_ARG_ENABLE([tests], AS_HELP_STRING([--enable-tests], [Enable unit tests]),, [enable_tests=no])
PKG_CHECK_MODULES([CMOCKA], [cmocka],, [
    AS_CASE([$enable_tests], [yes], [AC_MSG_ERROR([cmocka required for unit tests.])])
    enable_tests=no
  ])
AM_CONDITIONAL([ENABLE_TESTS], test "x$enable_tests" = "xyes")
AC_SUBST([CMOCKA_CFLAGS])
AC_SUBST([CMOCKA_LIBS])

PKG_CHECK_MODULES([LIBSODIUM], [libsodium])
AC_SUBST([LIBSODIUM_CFLAGS])
AC_SUBST([LIBSODIUM_LIBS])

PKG_CHECK_MODULES([LEVELDB], [leveldb],, [
    ORIGINAL_LIBS="$LIBS"
    AC_SEARCH_LIBS([leveldb_open], [leveldb], [
        LEVELDB_CFLAGS=
	LEVELDB_LIBS=-lleveldb
      ], [
        AC_SEARCH_LIBS([leveldb_close], [leveldb], [
            LEVELDB_CFLAGS=
            LEVELDB_LIBS="-lleveldb -lstdc++ -lsnappy -lpthread"
          ], [
            AC_MSG_ERROR([Unable to find LevelDB])
          ], [-lleveldb -lstdc++ -lsnappy -lpthread])
      ])
    LIBS="$ORIGINAL_LIBS"
  ])
AC_SUBST([LEVELDB_CFLAGS])
AC_SUBST([LEVELDB_LIBS])

PKG_CHECK_MODULES([PROTOBUF_C], [libprotobuf-c],, [
    ORIGINAL_LIBS="$LIBS"
    AC_SEARCH_LIBS([protobuf_c_message_pack], [protobuf-c], [
        PROTOBUF_C_CFLAGS=
	PROTOBUF_C_LIBS=-lprotobuf-c
      ], [
        AC_MSG_ERROR([Unable to find protobuf-c])
      ])
    LIBS="$ORIGINAL_LIBS"
  ])
AC_SUBST([PROTOBUF_C_CFLAGS])
AC_SUBST([PROTOBUF_C_LIBS])

PKG_CHECK_MODULES([LIBCURVECPR], [libcurvecpr])
AC_SUBST([LIBCURVECPR_CFLAGS])
AC_SUBST([LIBCURVECPR_LIBS])

# Checks for header files.
AC_CHECK_HEADERS([stdatomic.h])

# Checks for typedefs, structures, and compiler characteristics.
COLUMBA_CHECK_WARNING_FLAGS

FATAL_WARNING_CFLAGS=
AC_ARG_ENABLE([fatal-warnings], AS_HELP_STRING([--enable-fatal-warnings], [Make compiler warnings fatal]))
AS_IF([test "x$enable_fatal_warnings" = "xyes"], [
  AX_CHECK_COMPILE_FLAG([-Werror], [
      FATAL_WARNING_CFLAGS=-Werror
    ], [
      AC_MSG_ERROR([Unable to determine flag(s) to make warnings fatal])
    ])
  ], [enable_fatal_warnings=no])
AC_SUBST([FATAL_WARNING_CFLAGS])

# Checks for library functions.

AC_CONFIG_FILES([
  Makefile
  .dir-locals.el
  columba/Makefile
  columba/Doxyfile
  columba/columba-version.h
  tests/Makefile])

AC_OUTPUT

echo "
  Configuration summary:

    Build API documentation:         $enable_api_docs
    Build unit tests:                $enable_tests
    Run tests in valgrind:           $enable_valgrind
    Generate coverage information:   $use_gcov
    Compiler warnings are fatal:     $enable_fatal_warnings
"
