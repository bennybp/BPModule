#!/usr/bin/env python3

import sys
import mpmath as mp
import bp_common

mp.dps=250
mp.mp.dps = 250

if len(sys.argv) != 2:
   print("Usage: generate_constants.py outbase")
   quit(1)

outbase = sys.argv[1]



constants = {}


# All constants to generate
#       variable base                      value                             description                            reference
constants["PI"]                       =  (mp.pi,                            "The famous pi",                         "mmm pie"                       )
constants["TWO_PI"]                   =  (mp.mpf('2.0')*mp.pi,              "2*pi",                                  None                            )
constants["PI_SQUARED"]               =  (mp.pi*mp.pi,                      "pi^2",                                  None                            )
constants["SQRT_PI"]                  =  (mp.sqrt(mp.pi),                   "sqrt(pi)",                              None                            )
constants["ONE_OVER_PI"]              =  (mp.mpf('1.0')/mp.pi,              "1/pi",                                  None                            )
constants["ONE_OVER_SQRT_PI"]         =  (mp.mpf('1.0')/mp.sqrt(mp.pi),     "1/sqrt(pi)",                            None                            )
constants["SPEED_OF_LIGHT_SI"]        =  (mp.mpf('299792458'),              "Speed of light in m/s",                 "NIST CODATA 2014"              )
constants["BOHR_RADIUS_SI"]           =  (mp.mpf('0.52917721067e-10'),      "Bohr radius (AU of length) in m",       "NIST CODATA 2014"              )
constants["BOHR_RADIUS_ANGSTROMS"]    =  (mp.mpf('0.52917721067'),         "Bohr radius (AU of length) in A",       "NIST CODATA 2014"              )
constants["ELECTRON_MASS_SI"]         =  (mp.mpf('9.10938356e-31'),         "Mass of electron in kg",                "NIST CODATA 2014"              )
constants["AVOGADROS_CONSTANT"]       =  (mp.mpf('6.022140857e23'),         "Avogadro's constant (mol^-1)",          "NIST CODATA 2014"              )
constants["BOLTZMANN_CONSTANT_SI"]    =  (mp.mpf('1.38064852e-23'),         "Boltzmann's constant in J/K",           "NIST CODATA 2014"              )
constants["BOLTZMANN_CONSTANT_EV_K"]  =  (mp.mpf('8.6173303e-5'),           "Boltzmann's constant in eV/K",          "NIST CODATA 2014"              )
constants["JOULE_TO_EV"]              =  (mp.mpf('6.241509126e18'),         "Joule -> eV relationship (eV)",         "NIST CODATA 2014"              )
constants["JOULE_TO_HARTREE"]         =  (mp.mpf('2.293712317e17'),         "Joule -> Hartree relationship (E_H)",   "NIST CODATA 2014"              )
constants["HARTREE_TO_JOULE"]         =  (mp.mpf('4.359744650e-18'),        "Hartree -> Joule relationship (J)",     "NIST CODATA 2014"              )
constants["ANGSTROM_SI"]              =  (mp.mpf('1e-10'),                  "Angstrom (in m)",                       None                            )
constants["PLANCK_H_SI"]              =  (mp.mpf('6.626070040e-34'),        "Planck's constant in J*s",              "NIST CODATA 2014"              )
constants["PLANCK_HBAR_SI"]           =  (mp.mpf('1.054571800e-34'),        "h/(2pi)",                               "NIST CODATA 2014"              )
constants["AU_TIME_SI"]               =  (mp.mpf('2.418884326509e-17'),     "Atomic unit of time (in s)",            "NIST CODATA_2014"              )
constants["AU_VELOCITY_SI"]           =  (mp.mpf('2.18769126277e6'),        "Atomic unit of velocity (in m/s)",      "NIST CODATA_2014"              )

constants["SPEED_OF_LIGHT_AU"]        =  (mp.fdiv(constants['SPEED_OF_LIGHT_SI'][0], constants['AU_VELOCITY_SI'][0]), "Speed of light in atomic units",    "Derived from NIST CODATA 2014"   )


# Some aliases
constants["ATOMIC_UNIT_MASS"]         =  constants['ELECTRON_MASS_SI']
constants["ATOMIC_UNIT_LENGTH"]       =  constants['BOHR_RADIUS_SI']
constants["ATOMIC_UNIT_ENERGY"]       =  constants['JOULE_TO_HARTREE']
constants["ATOMIC_UNIT_TIME"]         =  constants['AU_TIME_SI']
constants["ATOMIC_UNIT_VELOCITY"]     =  constants['AU_VELOCITY_SI']




# keys in alphabetical order
keys = sorted(constants.keys())


with bp_common.HeaderSourceFiles(outbase, "Various constants and conversion factors", 
                                 [], # no namespaces
                                 createsource = False,
                                 plainc = True) as src:
    for c in keys:
        v = constants[c]

        src.fh.write("/*! \\brief {}\n".format(v[1]))
        src.fh.write(" *\n")
        if v[2]:
            src.fh.write(" * {}\n".format(v[2]))
        src.fh.write(" */\n")
        # setting min_fixed > max_fixed results in always using floating-point format
        #src.fh.write("#define {}_F   {}f\n".format(c, mp.nstr(v[0], 10, strip_zeros=True, min_fixed=1, max_fixed=0)))   # float (single precision)
        src.fh.write("#define {}     {}\n".format(c, mp.nstr(v[0], 18, strip_zeros=True, min_fixed=1, max_fixed=0)))   # double precision
        src.fh.write("\n\n")

