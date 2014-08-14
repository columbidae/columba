
AC_DEFUN([COLUMBA_CHECK_WARNING_FLAGS], [
COLUMBA_WARNING_CFLAGS=
AX_CHECK_COMPILE_FLAG([-Werror-unknown-warning-option], [EXTRA_CHECK_FLAGS=-Werror-unknown-warning-option], [EXTRA_CHECK_FLAGS=])
AC_ARG_ENABLE([extra-warnings], AS_HELP_STRING([--disable-extra-warnings], [Disable extra warnings]))
AS_IF([test "x$extra_warnings" != "xno"], [
for flag in \
    -pedantic \
    -Wall \
    -Wcast-qual \
    -Wclobbered \
    -Wconversion \
    -Wdate-time \
    -Wdouble-promotion \
    -Wempty-body \
    -Werror=format=2 \
    -Werror=format-security \
    -Werror=implicit-function-declaration \
    -Werror=init-self \
    -Werror=missing-include-dirs \
    -Werror=missing-prototypes \
    -Werror=pointer-arith \
    -Wextra \
    -Wextra-tokens \
    -Wfloat-equal \
    -Wformat-nonliteral \
    -Wformat-security \
    -Wignored-qualifiers \
    -Winit-self \
    -Winline \
    -Winvalid-pch \
    -Wlogical-op \
    -Wmaybe-uninitialized \
    -Wmissing-declarations \
    -Wmissing-format-attribute \
    -Wmissing-include-dirs \
    -Wmissing-noreturn \
    -Wmissing-parameter-type \
    -Wmissing-prototypes \
    -Wnested-externs \
    -Wno-missing-field-initializers \
    -Wold-style-definition \
    -Woverride-init \
    -Wpacked \
    -Wpedantic \
    -Wpointer-arith \
    -Wredundant-decls \
    -Wreturn-type \
    -Wshadow \
    -Wsign-compare \
    -Wstrict-prototypes \
    -Wsuggest-attribute=const \
    -Wsuggest-attribute=format \
    -Wsuggest-attribute=noreturn \
    -Wswitch-enum \
    -Wsync-nand \
    -Wtrampolines \
    -Wtype-limits \
    -Wundef \
    -Wuninitialized \
    -Wunsafe-loop-optimizations \
    -WUnsafe-loop-optimizations \
    -Wwrite-strings; do
  AX_CHECK_COMPILE_FLAG([${flag}], [COLUMBA_WARNING_CFLAGS="${COLUMBA_WARNING_CFLAGS} ${flag}"],, [$EXTRA_CHECK_FLAGS])
done
])
  AC_SUBST(COLUMBA_WARNING_CFLAGS)
])

