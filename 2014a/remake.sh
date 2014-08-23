#!/bin/sh

rm ./compiler.o
cd example
rm *.obj 
rm *.ent 
rm *.ext
cd ../
make
./compiler.o example/sample example/sample2 example/sample3

