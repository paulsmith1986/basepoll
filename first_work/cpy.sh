#!/bin/sh
rm formula.list* -f
rm proto_so* -f
rm proto_c* -f
rm game_formula.c -f
rm encode_client.h -f
rm decode_client.h -f
rm proto_size.h -f
if [ $# != 1 ]; then
    base_dir="/data/wwwroot/new_code/so/im/"
else
    base_dir="$1"
fi
cp "$base_dir"formula.list* .
cp "$base_dir"game_formula.c .
cp "$base_dir"proto_so* .
cp "$base_dir"proto_c* .
cp "$base_dir"encode_client.h .
cp "$base_dir"decode_client.h .
cp "$base_dir"proto_size.h .
#data/app/bin/php-fpm restart
