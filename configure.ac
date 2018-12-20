#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([mace], [v0.9], [dev@mattleach.net])
AC_CONFIG_SRCDIR([src/board.h])
AC_CONFIG_HEADERS([config.h])

LT_INIT
AM_INIT_AUTOMAKE([foreign])
AM_SILENT_RULES([yes])


# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MKDIR_P

# Checks for typedefs, structures, and compiler characteristics.o
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT8_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T
AC_CHECK_TYPES([ptrdiff_t])
AX_CXX_COMPILE_STDCXX_17(, [mandatory])

# Checks for library functions.
AC_CONFIG_FILES([Makefile
                 src/Makefile
                 testsuite/Makefile])

AC_OUTPUT
