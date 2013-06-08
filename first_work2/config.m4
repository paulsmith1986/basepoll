PHP_ARG_ENABLE(first_work, whether to enable first_work support,
Make sure that the comment is aligned:
[  --enable-first_work           Enable first_work support])

if test "$PHP_FIRST_WORK" != "no"; then
  PHP_NEW_EXTENSION(first_work, first_work.c, $ext_shared)
fi
