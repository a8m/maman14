#!/bin/sh

rm ./compiler.o
make
./compiler.o assembly/exmp1.as

