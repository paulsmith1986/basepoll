#!/bin/sh -e
rm proto_bin* -f
rm decode_server.h -f
rm proto_size.h -f
rm init_define.h -f
rm skill_formula.* -f
if [ $# != 1 ]; then
    base_dir="/data/wwwroot/new_code/so/fight/"
else
    base_dir="$1"
fi
cp "$base_dir"proto_bin* .
cp "$base_dir"decode_server.h .
cp "$base_dir"proto_size.h .
cp "$base_dir"skill.dat .
cp "$base_dir"skill_formula.* .
cp "$base_dir"init_define.h .
