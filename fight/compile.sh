#!/bin/sh -e
cp yile_fight.h.in yile_fight.h
sed "s@{\$yile_version}@ `date +'%Y-%m-%d %H:%M:%S'`@g" -i yile_fight.h
make clean
case "$1" in
	debug)
		echo " 调试版本"
		sed 's/YILE_SWITCH_DEBUG/YILE_DEBUG/g' -i yile_fight.h	
	;;
	*)
		echo " 正式版本"
		sed 's/YILE_SWITCH_DEBUG/YILE_NO_DEBUG/g' -i yile_fight.h
	;;
esac


make clean
make libyilefight.a
make libyilefightformula.a
make fight
bak_dir="yile_fight"

if [ -d "$bak_dir" ]
then
rm "$bak_dir" -rf;
fi
mkdir "$bak_dir"
mkdir "$bak_dir"/include
mkdir "$bak_dir"/lib
cp *.h "$bak_dir"/include/
cp libyilefight.a libyilefightformula.a "$bak_dir"/lib/
