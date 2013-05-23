#!/bin/sh -e
make clean
rm proto_c* -f
rm task_client.* -f
rm encode_client.h -f
rm decode_client.h -f
rm proto_size.h -f
cp /data/wwwroot/tool/build/proto_c* .
cp /data/wwwroot/tool/build/task_client.* .
cp /data/wwwroot/tool/build/encode_client.h .
cp /data/wwwroot/tool/build/decode_client.h .
cp /data/wwwroot/tool/build/proto_size.h .
make || echo ' 编译出错'
