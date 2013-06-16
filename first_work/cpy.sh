#!/bin/sh
rm proto_so* -f
rm proto_c* -f
rm so_decode.h -f
rm so_encode.h -f
rm so_send.h -f
rm proto_size* -f
if [ $# != 1 ]; then
    base_dir="/data/wwwroot/tool/build/so/"
else
    base_dir="$1"
fi
cp "$base_dir"so_decode.h .
cp "$base_dir"so_encode.h .
cp "$base_dir"so_send.h .
cp "$base_dir"proto_so* .
cp "$base_dir"proto_c* .
cp "$base_dir"proto_size* .
#data/app/bin/php-fpm restart
