#!/bin/sh
cp first.h.in first.h
sed "s@{\$first_version}@ `date +'%Y-%m-%d %H:%M:%S'`@g" -i first.h
make clean
case "$1" in
	debug)
		echo " 调试版本"
		sed 's/YILE_SWITCH_DEBUG/YILE_DEBUG/g' -i first.h	
	;;
	*)
		echo " 正式版本"
		sed 's/YILE_DEBUG_SWITCH/YILE_NO_DEBUG/g' -i first.h
	;;
esac
./cpy.sh
make && make install && echo "编译first_new.so成功"

