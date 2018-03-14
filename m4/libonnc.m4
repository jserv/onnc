dnl
dnl @synopsis CHECK_ONNC
dnl
dnl @summary Setup variables for libonnc.
dnl
dnl   LIBONNC_INCLUDES
dnl   LIBONNC_LDFLAGS
dnl   LIBONNC_LIBS
dnl
dnl * libonnc.m4 depends on AM variables set by pthread_check.m4
dnl

AC_DEFUN([CHECK_ONNC],
[dnl

abs_builddir=$(cd "${builddir}" && pwd)
abs_srcdir=$(cd "${srcdir}" && pwd)

LIBONNC_INCLUDES="-I${abs_srcdir}/include \
 -I${abs_builddir}/include \
 -I${abs_builddir}/lib"
LIBONNC_LDFLAGS="-L${abs_builddir}/lib"
LIBONNC_LIBS="${ONNX_LIBS} -lonnc ${PROTOBUF_LIBS}"

AM_COND_IF([HAVE_PTHREADS],[
  LIBONNC_LIBS="${LIBONNC_LIBS} -lpthread"
])

AC_SUBST(LIBONNC_INCLUDES)
AC_SUBST(LIBONNC_LDFLAGS)
AC_SUBST(LIBONNC_LIBS)

AC_MSG_NOTICE([setup libonnc variables:])
AC_MSG_RESULT([	LIBONNC_INCLUDES=${LIBONNC_INCLUDES}])
AC_MSG_RESULT([	LIBONNC_LDFLAGS=${LIBONNC_LDFLAGS}])
AC_MSG_RESULT([	LIBONNC_LIBS=${LIBONNC_LIBS}])

])
