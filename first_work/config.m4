PHP_ARG_ENABLE(yile_new, whether to enable ch.yile.com support,
[ --enable-yile   Enable ch.yile.com support])
PHP_ARG_WITH(yilenet, for yilenet support,[ --with-yilenet Include yilenet support]) 
PHP_ARG_WITH(yileutil, for yileutil support,[ --with-yileutil Include yileutil support]) 
PHP_ARG_WITH(yile_net_lib, for yile_net_lib support,[ --with-yile_net_lib Include yile_net_lib support]) 
PHP_ARG_WITH(yilefight, for yilefight support,[ --with-yilefight Include yilefight support]) 
PHP_ARG_WITH(yileprotocol, for yileprotocol support,[ --with-yileprotocol Include yileprotocol support]) 
PHP_ARG_WITH(yilefightformula, for yilefightformula support,[ --with-yilefightformula Include yilefightformula support]) 

if test "$PHP_YILENET" != "no"; then
  YILENET_DIR=$PHP_YILENET
  PHP_ADD_INCLUDE($YILENET_DIR/include)
  LIBNAME=yilenet # you may want to change this
  LIBSYMBOL=adler32 # you most likely want to change this
 

  PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $YILENET_DIR/lib, YILE_NEW_SHARED_LIBADD)
  #PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  #[
  #PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $YILENET_DIR/lib, YILE_NEW_SHARED_LIBADD)
  #AC_DEFINE(HAVE_YILENETLIB,1,[ ])],
  #[AC_MSG_ERROR([$YILENET_DIR/lib wrong yilenet lib version or lib not found])],[ -L$YILENET_DIR/lib -lyilenet])

  #if test "$PHP_YILEFIGHT" != "no"; then
  #    YILEFIGHT_DIR=$PHP_YILEFIGHT
  #    PHP_ADD_INCLUDE($YILEFIGHT_DIR/include)
  #    LIBNAME=yilefight
  #    PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $YILEFIGHT_DIR/lib, YILE_NEW_SHARED_LIBADD)
  #fi

  if test "$PHP_YILEFIGHT" != "no"; then
      YILEFIGHT_DIR=$PHP_YILEFIGHT
      PHP_ADD_INCLUDE($YILEFIGHT_DIR/include)
      LIBNAME=yilefight # you may want to change this
      LIBSYMBOL=create_ext_buff # you most likely want to change this
      PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
      [
      PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $YILEFIGHT_DIR/lib, YILE_NEW_SHARED_LIBADD)
      AC_DEFINE(HAVE_YILEFIGHTLIB,1,[ ])],
      [AC_MSG_ERROR([$YILEFIGHT_DIR/lib wrong $LIBNAME lib version or lib not found])],[ -L$YILEFIGHT_DIR/lib -lyilefight])
  fi

  if test "$PHP_YILEFIGHTFORMULA" != "no"; then
      YILEFIGHTFORMULA_DIR=$PHP_YILEFIGHTFORMULA
      PHP_ADD_INCLUDE($YILEFIGHTFORMULA_DIR/include)
      LIBNAME=yilefightformula # you may want to change this
      LIBSYMBOL=init_skill_formula # you most likely want to change this
      PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
      [
      PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $YILEFIGHTFORMULA_DIR/lib, YILE_NEW_SHARED_LIBADD)
      AC_DEFINE(HAVE_YILEFIGHTFORMULALIB,1,[ ])],
      [AC_MSG_ERROR([$YILEFIGHTFORMULA_DIR/lib wrong $LIBNAME lib version or lib not found])],[ -L$YILEFIGHTFORMULA_DIR/lib -lyilefightformula])
  fi
  
  if test "$PHP_YILEPROTOCOL" != "no"; then
      YILEPROTOCOL_DIR=$PHP_YILEPROTOCOL
      PHP_ADD_INCLUDE($YILEPROTOCOL_DIR/include)
      LIBNAME=protocol # you may want to change this
      LIBSYMBOL=yile_result_push_data # you most likely want to change this
      PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
      [
      PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $YILEPROTOCOL_DIR/lib, YILE_NEW_SHARED_LIBADD)
      AC_DEFINE(HAVE_YILEPROTOCOLLIB,1,[ ])],
      [AC_MSG_ERROR([$YILEPROTOCOL_DIR wrong $LIBNAME lib version or lib not found])],[ -L$YILEPROTOCOL_DIR/lib -lprotocol])
  fi

  if test "$PHP_YILEUTIL" != "no"; then
      YILEUTILS_DIR=$PHP_YILEUTIL
      PHP_ADD_INCLUDE($YILEUTILS_DIR)
      UTIL_INCLUDE=-I$YILEUTILS_DIR
      PHP_SUBST_OLD(UTIL_INCLUDE)
  fi

  if test "$PHP_YILE_NET_LIB" != "no"; then
      YILE_NET_LIB_DIR=$PHP_YILE_NET_LIB
      PHP_ADD_INCLUDE($YILE_NET_LIB_DIR)
      NET_INCLUDE=-I$YILE_NET_LIB_DIR
      PHP_SUBST_OLD(NET_INCLUDE)
  fi


  AC_DEFINE(HAVE_YILE_NEW, 1, [Whether you have ch.Yile.com])
  yile_sources="yile.c \
                yile_new.c \
		ch_json.c \
               yile_im.c \
               new_fight.c \
               game_formula.c \
               yile_proto.c \
               proto_so.c \
               proto_c.c \
               yile_log.c"
  PHP_SUBST(YILE_NEW_SHARED_LIBADD)
  PHP_NEW_EXTENSION(yile_new, $yile_sources, $ext_shared)
fi