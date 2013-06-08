PHP_ARG_ENABLE(first_work, whether to enable first support,
[ --enable-first   Enable first support])
if test "$PHP_FIRST_WORK" = "yes"; then
  AC_DEFINE(HAVE_HELLO, 1, [Whether you have first work])
  first_sources="first.c first_work.c"
  PHP_NEW_EXTENSION(first_work, $first_sources, $ext_shared)
fi
