#!/bin/sh
rm -v osc/* 
cp ../*.cpp osc/
cp ../*.h osc/
cp ../README osc/
cp ../keywords.txt osc/

rm arduino-osc-$1.zip
zip arduino-osc-$1.zip osc 