PHP_ARG_ENABLE(first_work, whether to enable first_work support,
Make sure that the comment is aligned:
[  --enable-first_work           Enable first_work support])

PHP_ARG_WITH(firstprotocol, for firstprotocol support,[ --with-firstprotocol Include firstprotocol support]) 

if test "$PHP_FIRST_WORK" != "no"; then
  if test "$PHP_FIRSTPROTOCOL" != "no"; then
      FIRSTPROTOCOL_DIR=$PHP_FIRSTPROTOCOL
      PHP_ADD_INCLUDE($FIRSTPROTOCOL_DIR/include)
      LIBNAME=firstprotocol # you may want to change this
      LIBSYMBOL=first_result_push_data # you most likely want to change this
      PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
      [
      PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $FIRSTPROTOCOL_DIR/lib, FIRST_NEW_SHARED_LIBADD)
      AC_DEFINE(HAVE_FIRSTPROTOCOLLIB,1,[ ])],
      [AC_MSG_ERROR([$FIRSTPROTOCOL_DIR wrong $LIBNAME lib version or lib not found])],[ -L$FIRSTPROTOCOL_DIR/lib -l$LIBNAME])
  fi
  first_sources="first_work.c		\
		first_proto.c		\
		proto_so.c		\
		first_poll.c"
  PHP_NEW_EXTENSION(first_work, $first_sources, $ext_shared)
fi
