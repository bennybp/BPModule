#!/usr/bin/env python3

import sys
import bp_common



if len(sys.argv) != 2:
   print("Usage: generate_factorial.py outbase")
   quit(1)


outbase = sys.argv[1]


def PrintArr(src, lst, name, t, fmt):
  src.fh.write("extern {} {}_lut_[{}];\n".format(t, name, len(lst)))
  src.fh.write("static const int {}_LUT_MAX_ = {};      // Maximum index of {}\n".format(name.upper(), len(lst)-1, name))
  src.fh.write("\n")

  nameline = "{} {}_lut_[{}] = {{".format(t, name, len(lst))
  src.f.write(nameline + "\n")
  for v in lst:
    src.f.write(" "*(len(nameline)+1) + fmt.format(v) + ",\n")
  src.f.write(" "*(len(nameline)-1) + "}}; // close {}\n".format(name))




######################################################
# Calculate as arbitrary length integers, then convert
fac = [1, 1, 2]
dfac = [1, 1, 1, 2, 3]

# Normal factorial
# Go until we overflow double precision
try:
    fac_val = 6
    fac_val2 = 6.0
    while True:
        fac.append(fac_val)
        fac_val = fac[-1] * (len(fac)) 
        fac_val2 = float(fac_val) # may raise exception
except OverflowError:
    pass



# Double factorial
# Shifted by one since (-1)!! = 1
try:
    fac_val = 8
    fac_val2 = 8.0
    while True:
        dfac.append(fac_val)
        fac_val = dfac[-2] * (len(dfac)-1)
        fac_val2 = float(fac_val) # may raise exception
except OverflowError:
    pass


# (2n-1)!! with n >= 0
# start at n=0 which is (-1)!! and stride over every other value
# Note that (-1)!! is stored as dfac[0]
dfac_2nm1 = [ v for i,v in enumerate(dfac) if (i%2) == 0 ]




#######################################################
# create the various types
#######################################################
# factorial: 64-bit unsigned integer
fac_int = [ v for v in fac if v <= 2**64 ]

# factorial: single precision
fac_float = [ float(v) for v in fac if v <= float(3.40282e+38) ]

# factorial: double precision (all values)
fac_double = [ float(v) for v in fac ]

# double factorial: 64-bit unsigned integer
dfac_int = [ v for v in dfac if v <= 2**64 ]

# double factorial: single precision
dfac_float = [ float(v) for v in dfac if v <= float(3.40282e+38) ]

# double factorial: double precision (all values)
dfac_double = [ float(v) for v in dfac ]

# (2n-1)!!: 64-bit unsigned integer
dfac_2nm1_int = [ v for v in dfac_2nm1 if v <= 2**64 ] 

# double factorial: single precision
dfac_2nm1_float = [ float(v) for v in dfac_2nm1 if v <= float(3.40282e+38) ]

# double factorial: double precision (all values)
dfac_2nm1_double = [ float(v) for v in dfac_2nm1 ]






# Create cpp and hpp files
cppfile = "test.cpp"
hppfile = "test.hpp"


with bp_common.HeaderSourceFiles(outbase, "LUT for factorial and related functions", 
                                 ["math", "lut"],
                                 hppincludes = ["<cstdint>"],
                                 cppincludes = ["<cstdint>"]) as src:

    ########################################################
    # Create LUT
    ########################################################
    ####################################################
    # Factorial
    ####################################################
    src.f.write("// Factorial: 64-bit unsigned integer\n")
    src.f.write("// fac_int_lut_[i] = i!\n")
    PrintArr(src, fac_int, "fac_int", "uint64_t", "{}")
    src.f.write("\n\n\n")

    src.f.write("// Factorial: single-precision floating point\n")
    src.f.write("// fac_float_lut_[i] = i!\n")
    PrintArr(src, fac_float, "fac_float", "float", "{:.10e}f")
    src.f.write("\n\n\n")

    src.f.write("// Factorial: double-precision floating point\n")
    src.f.write("// fac_double_lut_[i] = i!\n")
    PrintArr(src, fac_double, "fac_double", "double", "{:.18e}")
    src.f.write("\n\n\n")

    ####################################################
    # Double factorial
    ####################################################
    src.f.write("// Double Factorial: 64-bit unsigned integer\n")
    src.f.write("// dfac_int_lut_[i] = (i-1)!!\n")
    PrintArr(src, dfac_int, "dfac_int", "uint64_t", "{}")
    src.f.write("\n\n\n")

    src.f.write("// Double Factorial: single-precision floating point\n")
    src.f.write("// dfac_float_lut_[i] = (i-1)!!\n")
    PrintArr(src, dfac_float, "dfac_float", "float", "{:.10e}f")
    src.f.write("\n\n\n")

    src.f.write("// Double Factorial: double-precision floating point\n")
    src.f.write("// dfac_double_lut_[i] = (i-1)!!\n")
    PrintArr(src, dfac_double, "dfac_double", "double", "{:.18e}")
    src.f.write("\n\n\n")

    ####################################################
    # (2n-1)!!
    ####################################################
    src.f.write("// (2n-1)!!: 64-bit unsigned integer\n")
    src.f.write("// dfac_2nm1_int_lut_[i] = (2i-1)!!\n")
    PrintArr(src, dfac_2nm1_int, "dfac_2nm1_int", "uint64_t", "{}")
    src.f.write("\n\n\n")

    src.f.write("// (2n-1)!!: single-precision floating point\n")
    src.f.write("// dfac_2nm1_float_lut_[i] = (2i-1)!!\n")
    PrintArr(src, dfac_2nm1_float, "dfac_2nm1_float", "float", "{:.10e}f")
    src.f.write("\n\n\n")

    src.f.write("// (2n-1)!!: double-precision floating point\n")
    src.f.write("// dfac_2nm1_float_lut_[i] = (2i-1)!!\n")
    PrintArr(src, dfac_2nm1_double, "dfac_2nm1_double", "double", "{:.18e}")
    src.f.write("\n\n\n")

