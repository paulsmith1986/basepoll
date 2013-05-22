#!/bin/sh -e
make clean
cp first_poll.h.in first_poll.h
sed "s@{\$first_poll_version}@ `date +'%Y-%m-%d %H:%M:%S'`@g" -i first_poll.h
case "$1" in
	debug)
		echo " 调试版本"
		sed 's/FIRST_NET_DEBUG_SWITCH/FIRST_NET_DEBUG/g' -i first_poll.h	
	;;
	*)
		echo " 正式版本"
		sed 's/FIRST_NET_DEBUG_SWITCH/FIRST_NET_NO_DEBUG/g' -i first_poll.h
	;;
esac
make || echo ' 编译出错'