#!/bin/bash

mpicxx -Wall -pedantic -fPIC -c *.cpp
mpicxx -shared -Wl,-soname,libmpitest.so.1 -o libmpitest.so.1.0  *.o
