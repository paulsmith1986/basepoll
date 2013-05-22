#!/bin/sh -e
make clean
make

bak_dir="firstclient"
if [ -d "$bak_dir" ]
then
rm "$bak_dir" -rf;
fi
mkdir "$bak_dir"
mkdir "$bak_dir"/include
mkdir "$bak_dir"/lib
cp first_client.h "$bak_dir"/include/
cp *.a "$bak_dir/lib/"



make clean
make -f Makefile_gcc

bak_dir="firstclient_gcc"
if [ -d "$bak_dir" ]
then
rm "$bak_dir" -rf;
fi
mkdir "$bak_dir"
mkdir "$bak_dir"/include
mkdir "$bak_dir"/lib
cp first_client.h "$bak_dir"/include/
cp *.a "$bak_dir/lib/"