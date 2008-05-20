AC_INIT([pspshrink], [1.0.1])
AC_CONFIG_SRCDIR([src])
AM_INIT_AUTOMAKE

AC_LANG_CPLUSPLUS
AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_SYS_LARGEFILE 

AC_HEADER_STDC
AC_CHECK_HEADERS(
    [string zlib.h pthread.h],,
    [AC_MSG_ERROR([Missing headers required to compile pspshrink])]
)

PKG_CHECK_MODULES(	GTKMM, gtkmm-2.4, HAVE_GTKMM=yes, 
					AC_MSG_ERROR([ +Could not find gtkmm-2.4. Please update PKG_CONFIG_PATH to point at location of gtkmm pkgconfig files directory.])
)

PKG_CHECK_MODULES(	GTHREAD, gthread-2.0, HAVE_GTHREAD=yes, 
					AC_MSG_ERROR([ +Could not find gthread-2.0. Please update PKG_CONFIG_PATH to point at location of gthread pkgconfig files directory.])
)

AC_SUBST(GTKMM_CFLAGS)
AC_SUBST(GTKMM_LIBS)
AC_SUBST(GTHREAD_LIBS)

AC_OUTPUT([
	Makefile
])
