# Process this file with autoconf to produce a configure script.

dnl **************************
dnl Define version information
dnl **************************
m4_define([he_version_major], [0])
m4_define([he_version_minor], [1])
m4_define([he_version_micro], [0])
m4_define([he_version], [he_version_major.he_version_minor.he_version_micro])

dnl *************************
dnl Define bug-report address
dnl *************************
m4_define([he_bug_report_url],
          [https://github.com/tfarina/isengard/issues])

dnl *******************
dnl Initialize autoconf
dnl *******************
AC_INIT([hello], [he_version], [he_bug_report_url])
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

dnl *******************
dnl Enable i18n support
dnl *******************
AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION([0.19.7])

GETTEXT_PACKAGE=hello
AC_SUBST([GETTEXT_PACKAGE])
AC_DEFINE_UNQUOTED([GETTEXT_PACKAGE], ["$GETTEXT_PACKAGE"],
                   [Name of default gettext domain])

AC_CONFIG_FILES([
Makefile
src/Makefile
])

AC_OUTPUT
