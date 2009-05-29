#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

#AC_PREREQ(2.61)
AC_PREREQ(2.50)
AC_INIT(codequery, 1.0, gcross at phys.washington.edu)
AM_INIT_AUTOMAKE
AC_CONFIG_HEADER([config.h])
AC_CONFIG_SRCDIR(src)
AC_CONFIG_FILES([Makefile src/Makefile])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Check for namespaces
AC_CXX_NAMESPACES

# Checks for STL string streams.
AC_CXX_HAVE_SSTREAM

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_C_RESTRICT
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE

AC_OUTPUT
