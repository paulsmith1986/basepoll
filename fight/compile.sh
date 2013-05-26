#!/bin/sh -e
cp first_fight.h.in first_fight.h
sed "s@{\$firs_version}@ `date +'%Y-%m-%d %H:%M:%S'`@g" -i first_fight.h
make clean
case "$1" in
	debug)
		echo " 调试版本"
		sed 's/FIRST_SWITCH_DEBUG/FIRST_DEBUG/g' -i first_fight.h	
	;;
	*)
		echo " 正式版本"
		sed 's/FIRST_SWITCH_DEBUG/FIRST_NO_DEBUG/g' -i first_fight.h
	;;
esac


make clean
make libfirstfight.a
make libfirstfightformula.a
make fight
bak_dir="first_fight"

if [ -d "$bak_dir" ]
then
rm "$bak_dir" -rf;
fi
mkdir "$bak_dir"
mkdir "$bak_dir"/include
mkdir "$bak_dir"/lib
cp *.h "$bak_dir"/include/
cp libfirstfight.a libfirstfightformula.a "$bak_dir"/lib/
