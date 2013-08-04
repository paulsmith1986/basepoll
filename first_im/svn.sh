#!/bin/sh -e
cp im /data/wwwroot/bin_svn/
cd /data/wwwroot/bin_svn/
svn up
svn ci -m "new im"