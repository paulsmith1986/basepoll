#!/bin/sh
rm proto_so* -f
rm encode_client.h -f
rm decode_client.h -f
rm proto_size.h -f
if [ $# != 1 ]; then
    base_dir="/data/wwwroot/tool/build/"
else
    base_dir="$1"
fi
cp "$base_dir"encode_client.h .
cp "$base_dir"decode_client.h .
cp "$base_dir"proto_so* .
cp "$base_dir"proto_size.h .
#data/app/bin/php-fpm restart
