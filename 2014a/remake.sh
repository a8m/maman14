#!/bin/sh

rm ./compiler.o
cd example
rm sa*.obj 
rm sa*.ent 
rm sa*.ext
cd ../
make
./compiler.o example/sample example/sample2 example/sample3

