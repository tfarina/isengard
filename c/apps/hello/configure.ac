# Process this file with autoconf to produce a configure script.

dnl **************************
dnl Define version information
dnl **************************
m4_define([major_version], [0])
m4_define([minor_version], [1])
m4_define([version], [major_version.minor_version])

dnl *************************
dnl Define bug-report address
dnl *************************
m4_define([bug_report_url],
          [https://github.com/tfarina/isengard/issues])

dnl *******************
dnl Initialize autoconf
dnl *******************
AC_INIT([hello], [version], [bug_report_url])
AC_PREREQ([2.69])
AC_CONFIG_HEADERS([config.h])

dnl *******************
dnl Initialize automake
dnl *******************
AM_INIT_AUTOMAKE([1.8 foreign])

dnl ************************
dnl Check for basic programs
dnl ************************
AC_PROG_CC

AC_CONFIG_FILES([
Makefile
src/Makefile
])

AC_OUTPUT
