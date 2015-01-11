#!/usr/bin/env python3

import sys
from ctypes import *


testlib = CDLL("./libmpitest.so.1.0")
testlib.PrintInfo()

argc = c_int(len(sys.argv))

argv = (c_char_p * (len(sys.argv) + 1))()
idx = 0
for arg in sys.argv:
    argv[idx] = bytes(str(arg), 'ascii')
    idx += 1
argv[ len(sys.argv) ] = None

testlib.Init(argc, argv)
testlib.PrintMPIInfo()
testlib.Finalize()

