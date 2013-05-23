#!/bin/sh -e
make clean
make

bak_dir="protocol"
if [ -d "$bak_dir" ]
then
rm "$bak_dir" -rf;
fi
mkdir "$bak_dir"
mkdir "$bak_dir"/include
mkdir "$bak_dir"/lib
cp first_protocol.h "$bak_dir"/include/
cp *.a "$bak_dir/lib/"



make clean
make -f Makefile_gcc

bak_dir="protocol_gcc"
if [ -d "$bak_dir" ]
then
rm "$bak_dir" -rf;
fi
mkdir "$bak_dir"
mkdir "$bak_dir"/include
mkdir "$bak_dir"/lib
cp first_protocol.h "$bak_dir"/include/
cp *.a "$bak_dir/lib/"