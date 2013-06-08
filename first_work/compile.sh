#!/bin/sh
cp first.h.in first.h
sed "s@{\$first_version}@ `date +'%Y-%m-%d %H:%M:%S'`@g" -i first.h
make clean
case "$1" in
	debug)
		echo " 调试版本"
		sed 's/FIRST_SWITCH_DEBUG/FIRST_DEBUG/g' -i first.h	
	;;
	*)
		echo " 正式版本"
		sed 's/FIRST_DEBUG_SWITCH/FIRST_NO_DEBUG/g' -i first.h
	;;
esac
case "$2" in
	all)
		echo " 全部编译"
		cd ../first_net_client/
		./compile.sh
        cd ../fight/
        ./compile.sh $1
		cd ../first_work/
	;;
esac
#./cpy.sh
make && make install && echo "编译first_work.so成功"
#sleep 1
#rm /data/wwwroot/new_trunk/so/first_work.so -f 
#cp /usr/local/php/lib/php/extensions/no-debug-non-zts-20090626/first_work.so /data/wwwroot/new_trunk/so/
#data/app/bin/php-fpm restart
