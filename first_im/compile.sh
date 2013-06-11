#!/bin/sh -e
make clean
rm proto_bin* -f
rm task_server.* -f
rm encode_server.h -f
rm decode_server.h -f
rm proto_size.h -f
cp /data/wwwroot/tool/build/proto_bin* .
cp /data/wwwroot/tool/build/task_server.* .
cp /data/wwwroot/tool/build/encode_server.h .
cp /data/wwwroot/tool/build/decode_server.h .
cp /data/wwwroot/tool/build/proto_size.h .
make || echo ' 编译出错'
