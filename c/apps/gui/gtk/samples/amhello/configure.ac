# Process this file with autoconf to produce a configure script.

dnl *******************
dnl Initialize autoconf
dnl *******************
AC_INIT([amhello], [0.1], [bug-report@address])
AC_PREREQ([2.69])

dnl *******************
dnl Initialize automake
dnl *******************
AM_INIT_AUTOMAKE([1.8 foreign])

dnl ************************
dnl Check for basic programs
dnl ************************
AC_PROG_CC

PKG_PROG_PKG_CONFIG([0.29.1])

dnl Dependencies

gtk_min_version=2.10.0

dnl Check for required packages
PKG_CHECK_MODULES([GTK], [gtk+-2.0 >= gtk_min_version])

AC_PATH_PROG(GLIB_COMPILE_RESOURCES, glib-compile-resources)

AC_CONFIG_FILES([
Makefile
src/Makefile
])

AC_OUTPUT
