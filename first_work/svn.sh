#!/bin/sh -e
cp /data/app/php/lib/php/extensions/no-debug-non-zts-20090626/yile_new.so /data/wwwroot/yile_so/
cd /data/wwwroot/yile_so
svn up
svn ci -m "new yile_new.so"
