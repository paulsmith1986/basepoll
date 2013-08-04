#!/bin/sh -e
#rm /data/wwwroot/bin_svn/so/first_work.so
cp /data/app/php/lib/php/extensions/no-debug-non-zts-20090626/first_work.so /data/wwwroot/bin_svn/so/
cd /data/wwwroot/bin_svn/
svn up
svn ci -m "new im"