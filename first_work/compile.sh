#!/bin/sh
cp yile.h.in yile.h
sed "s@{\$yile_version}@ `date +'%Y-%m-%d %H:%M:%S'`@g" -i yile.h
make clean
case "$1" in
	debug)
		echo " 调试版本"
		sed 's/YILE_SWITCH_DEBUG/YILE_DEBUG/g' -i yile.h	
	;;
	*)
		echo " 正式版本"
		sed 's/YILE_DEBUG_SWITCH/YILE_NO_DEBUG/g' -i yile.h
	;;
esac
case "$2" in
	all)
		echo " 全部编译"
		cd ../yile_net_client/
		./compile.sh
        cd ../fight/
        ./compile.sh $1
		cd ../yile_new/
	;;
esac
./cpy.sh
make && make install && echo "编译yile_new.so成功"
#sleep 1
#rm /data/wwwroot/new_trunk/so/yile_new.so -f 
#cp /usr/local/php/lib/php/extensions/no-debug-non-zts-20090626/yile_new.so /data/wwwroot/new_trunk/so/
#data/app/bin/php-fpm restart
