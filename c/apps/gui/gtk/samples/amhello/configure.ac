# Process this file with autoconf to produce a configure script.

dnl **************************
dnl Define version information
dnl **************************
m4_define([amh_version_major], [0])
m4_define([amh_version_minor], [1])
m4_define([amh_version_micro], [0])
m4_define([amh_version], [amh_version_major.amh_version_minor.amh_version_micro])

dnl *************************
dnl Define bug-report address
dnl *************************
m4_define([amh_bug_report_url],
          [https://github.com/tfarina/isengard/issues])

dnl *******************
dnl Initialize autoconf
dnl *******************
AC_INIT([amhello], [amh_version], [amh_bug_report_url])
AC_PREREQ([2.69])

dnl *******************
dnl Initialize automake
dnl *******************
AM_INIT_AUTOMAKE([1.8 foreign])

dnl ************************
dnl Check for basic programs
dnl ************************
AC_PROG_CC

dnl ***************************
dnl Check for required programs
dnl ***************************
AC_PATH_PROG(GLIB_COMPILE_RESOURCES, glib-compile-resources)

PKG_PROG_PKG_CONFIG([0.29.1])

dnl *************************
dnl Minimum required versions
dnl *************************
glib_min_version=2.19.7
gtk_min_version=2.16.1

dnl ***************************
dnl Check for required packages
dnl ***************************
PKG_CHECK_MODULES([GLIB], [glib-2.0 >= glib_min_version])
PKG_CHECK_MODULES([GTK], [gtk+-2.0 >= gtk_min_version])

AC_CONFIG_FILES([
Makefile
src/Makefile
])

AC_OUTPUT
