#!/bin/sh -e
make clean
cp im_common.h.in im_common.h
sed "s@{\$im_version}@ `date +'%Y-%m-%d %H:%M:%S'`@g" -i im_common.h
case "$1" in
	debug)
		echo " 调试版本"
		sed 's/YILE_DEBUG_SWITCH/YILE_DEBUG/g' -i im_common.h	
	;;
	*)
		echo " 正式版本"
		sed 's/YILE_DEBUG_SWITCH/YILE_NO_DEBUG/g' -i im_common.h
	;;
esac

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
