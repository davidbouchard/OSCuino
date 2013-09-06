#!/bin/sh
mkdir osc
rm -v osc/* 
cp ../*.cpp osc/
cp ../*.h osc/
cp ../README osc/
cp ../keywords.txt osc/

rm arduino-osc-$1.zip
zip -r arduino-osc-$1.zip osc 