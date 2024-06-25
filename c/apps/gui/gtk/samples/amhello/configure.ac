# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])

AC_INIT([amhello], [0.1], [bug-report@address])
AM_INIT_AUTOMAKE

dnl Checks for programs.
AC_PROG_CC

PKG_PROG_PKG_CONFIG([0.29.1])

PKG_CHECK_MODULES([GTK], [gtk+-2.0 >= 2.10.0])

AC_CONFIG_FILES([
Makefile
src/Makefile
])

AC_OUTPUT
