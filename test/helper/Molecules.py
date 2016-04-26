import os
import sys

# Add the pulsar path
thispath = os.path.dirname(os.path.realpath(__file__))
psrpath = os.path.join(os.path.dirname(thispath), "../", "modules")
sys.path.insert(0, psrpath)


import pulsar as psr

#RMR-No guarantees I named these correctly, in the immortal words of MHG 
#(modified appropriately) : Pulsar "doesn't care what you call the molecule"

#Helium atom Kh
He = psr.system.MakeSystem("""
0 1
He 0.0 0.0 0.0
""")

#Acetylene with D_inf_h symmetry
C2H2 = psr.system.MakeSystem("""
     0 1
     C 0 0  0.65
     C 0 0 -0.65
     H 0 0  1.75
     H 0 0 -1.75
     """)
     
#Chlro-acetylene with C_inf_h symmetry
C2HCl = psr.system.MakeSystem("""
    0 1
    H  0 0 -1.0
    C  0 0  0.0
    C  0 0  1.1
    Cl 0 0  2.6
    """)
#Boric acid with C3H symmetry
BoricAcid = psr.system.MakeSystem("""
B	0.0000	0.0000	0.0000
O	0.0000	1.3651	0.0000
O	-1.1822	-0.6826	0.0000
O	1.1822	-0.6826	0.0000
H	-0.9229	1.6969	0.0000
H	-1.0081	-1.6478	0.0000
H	1.9310	-0.0492	0.0000
""")


#Sulfur 8 in D4d symmetry
S8 = psr.system.MakeSystem("""
S 0.0238 2.3163 -0.5082
S -1.6211 1.6547 0.5080
S 1.6548 1.6209 0.5085
S -2.3163 0.0237 -0.5082
S 2.3163 -0.0238 -0.5084
S -1.6546 -1.6208 0.5086
S 1.6212 -1.6547 0.5080
S -0.0241 -2.3162 -0.5083
""")    
    
#Corannulene C_5v symmetry
Corannulene = psr.system.MakeSystem("""
C        0.70622800     0.97211978     0.61694803
C       -0.70622800     0.97211978     0.61694803
C       -1.14280400    -0.37137722     0.61681203
C        0.00000000    -1.20165922     0.61659503
C        1.14280400    -0.37137722     0.61681203
C        1.45779000     2.00650178     0.09413403
C       -1.45779000     2.00650178     0.09413403
C       -2.35873800    -0.76639722     0.09397203
C        0.00000000    -2.48004022     0.09366903
C        2.35873800    -0.76639722     0.09397203
C        0.69261800     3.17923978    -0.25321497
C       -0.69261800     3.17923978    -0.25321497
C       -2.80958100     1.64119778    -0.25292797
C       -3.23765700     0.32373778    -0.25303797
C       -2.42918200    -2.16498922    -0.25302597
C       -1.30841500    -2.97916822    -0.25327697
C        1.30841500    -2.97916822    -0.25327697
C        2.42918200    -2.16498922    -0.25302597
C        3.23765700     0.32373778    -0.25303797
C        2.80958100     1.64119778    -0.25292797
H        1.20851300     4.06642078    -0.61418797
H       -1.20851300     4.06642078    -0.61418797
H       -3.49401500     2.40602178    -0.61367197
H       -4.24094400     0.10729578    -0.61373997
H       -3.36816400    -2.57958822    -0.61350597
H       -1.41248600    -4.00024222    -0.61397997
H        1.41248600    -4.00024222    -0.61397997
H        3.36816400    -2.57958822    -0.61350597
H        4.24094400     0.10729578    -0.61373997
H        3.49401500     2.40602178    -0.61367197
""")

#BromoChloromethane C_s symmetry
CH2BrCl = psr.system.MakeSystem("""
    Cl        0.00000        0.00000        0.00000
    C        1.00000        0.00000        0.00000
    Br        1.25882        0.96593        0.00000
    H        1.25882       -0.48296        0.83652
    H        1.25882       -0.48296       -0.83652
    """)

#Hydrogen peroxide dimer in C_i symmetry
PeroxideDimer = psr.system.MakeSystem("""
    0 1
    H   0.9911262285  -1.7979226333   0.1465182515
    O   2.7691093095  -1.3485218649  -0.0071557684
    O   2.5178030311   1.3808374923  -0.1154058014
    H   3.2883200453   1.8308595095   1.4757706825
    H  -3.2883200453  -1.8308595095  -1.4757706825
    O  -2.5178030311  -1.3808374923   0.1154058014
    O  -2.7691093095   1.3485218649   0.0071557684
    H  -0.9911262285   1.7979226333  -0.1465182515
    """)

#Hydrogen peroxide C_2 symmetry
Peroxide = psr.system.MakeSystem("""
    H  -3.2883200453  -1.8308595095  -1.4757706825
    O  -2.5178030311  -1.3808374923   0.1154058014
    O  -2.7691093095   1.3485218649   0.0071557684
    H  -0.9911262285   1.7979226333  -0.1465182515
    """)
    
#Trihydroxylnitrogen C_3 symmetry
NO3H3 = psr.system.MakeSystem("""
    N        1.00000        0.00000        0.00000
    O        1.26047        1.47721        0.00000
    O        1.26047       -0.73861        1.27930
    O        1.26047       -0.73861       -1.27930
    H        1.58640        1.76704        0.89987
    H        1.58640       -1.66283        1.08036
    H        1.58640       -0.10421       -1.98024
    """)

#C_2v water
Water = psr.system.MakeSystem("""
      H        0.00000        0.00000        0.00000
      O        1.00000        0.00000        0.00000
      H        1.33381        0.94264        0.00000
      """)
     
     
#C_3v ammonia
Ammonia = psr.system.MakeSystem("""
        N        1.00000        0.00000        0.00000
        H        1.40149        0.86099        0.00000
        H        1.40149       -0.43050        0.74564
        H        1.40149       -0.43050       -0.74564
        """)
        
#Brominepentaflourine C_4v
BrF5=psr.system.MakeSystem("""
        F        0.00000        0.00000        0.00000
        Br        1.70000        0.00000        0.00000
        F        1.70000        1.70000        0.00000
        F        1.70000        0.00000       -1.70000
        F        1.70000        0.00000        1.70000
        F        1.70000       -1.70000        0.00000
        """)

#Diazene in C_2h symmetry
Diazene=psr.system.MakeSystem("""
    N        0.00000        0.00000        0.00000
    N        1.40000        0.00000        0.00000
    H       -0.76604        0.64279        0.00000
    H        2.16604       -0.64279        0.00000
    """)

# 1,3,5,7-etrafluoracyclooctatetrane in S_4 symmetry
C8F4H4 = psr.system.MakeSystem("""
    C       -1.618188     -0.437140     -0.409373
    C       -1.394411      0.896360     -0.429596
    C       -0.896360     -1.394411      0.429596
    C       -0.437140      1.618188      0.409373
    C        0.437140     -1.618188      0.409373
    C        0.896360      1.394411      0.429596
    C        1.394411     -0.896360     -0.429596
    C        1.618188      0.437140     -0.409373
    F        2.147277     -1.690111     -1.235043
    F        1.690111      2.147277      1.235043
    F       -2.147277      1.690111     -1.235043
    F       -1.690111     -2.147277      1.235043
    H        0.878010     -2.418132      1.029595
    H       -2.418132     -0.878010     -1.029595
    H       -0.878010      2.418132      1.029595
    H        2.418132      0.878010     -1.029595
    """)
    
#Staggered ethane in D_3d symmetry
EthaneStag = psr.system.MakeSystem("""
   C -0.7560    0.0000    0.0000 
   C  0.7560    0.0000    0.0000 
   H -1.1404    0.6586    0.7845 
   H -1.1404    0.3501   -0.9626 
   H -1.1405   -1.0087    0.1781 
   H  1.1404   -0.3501    0.9626 
   H  1.1405    1.0087   -0.1781 
   H  1.1404   -0.6586   -0.7845
   """)

#Eclipsed ethane in D_3h symmetry
EthaneEclip = psr.system.MakeSystem("""
  H        0.00000        0.00000        0.00000
  C        1.00000        0.00000        0.00000
  C        1.51303        1.40954        0.00000
  H        0.74699        2.05233        0.00000
  H        2.07152        1.57024        0.81380
  H        2.07152        1.57024       -0.81380
  H        1.32453       -0.48209       -0.81380
  H        1.32453       -0.48209        0.81380
  """)

#PtCl4 in D_4h
PtCl4 = psr.system.MakeSystem("""
Pt 0.0 0.0 0.0
Cl 0.0 0.0 2.3
Cl 0.0 0.0 -2.3
Cl 0.0 2.3 0.0
cl 0.0 -2.3 0.0
""")

    
#Cyclopentadiene anion in D_5h
C5H5Anion = psr.system.MakeSystem("""
  C        1.00000        1.19300        0.00000
  C        1.00000        0.36866        1.13461
  C        1.00000       -0.96516        0.70123
  C        1.00000       -0.96516       -0.70123
  C        1.00000        0.36866       -1.13461
  H        1.00000        2.27300       -0.00000
  H        1.00000        0.70240        2.16175
  H        1.00000       -1.83890        1.33604
  H        1.00000       -1.83890       -1.33604
  H        1.00000        0.70240       -2.16175
"""
)
    
    
#Allene in D_2d symmetry
Allene = psr.system.MakeSystem("""
    H -2.0  0.0  1.0
    H -2.0  0.0 -1.0
    C -1.5  0.0  0.0
    C  0.0  0.0  0.0
    C  1.5  0.0  0.0
    H  2.0  1.0  0.0
    H  2.0 -1.0  0.0
    """)

#Napthalene D_2h symmetry
Napthalene = psr.system.MakeSystem("""
    C  0.0000    0.7076    0.0000 
    C  0.0000   -0.7076    0.0000 
    C  1.2250    1.3944    0.0000 
    C  1.2250   -1.3944    0.0000 
    C -1.2250    1.3943    0.0000
    C -1.2250   -1.3943    0.0000 
    C  2.4327    0.6959    0.0000 
    C  2.4327   -0.6958    0.0000 
    C -2.4327    0.6958    0.0000 
    C -2.4327   -0.6958    0.0000 
    H  1.2489    2.4821    0.0000 
    H  1.2489   -2.4822    0.0000 
    H -1.2490    2.4821    0.0000 
    H -1.2489   -2.4822    0.0000
    H  3.3732    1.2391    0.0000
    H  3.3733   -1.2390    0.0000 
    H -3.3732    1.2390    0.0000 
    H -3.3733   -1.2390    0.0000 
    """)

#Twistane D_2
Twistane=psr.system.MakeSystem("""
C -0.62693 1.12897 0.473219
C -1.57383 0.000 0.000
C -0.626924 -1.12898 -0.473218
C 0.62693 -1.12897 0.473219
C 1.57383 0.0000 0.0000
C 0.626924 1.12898 -0.473218
H -2.22992 0.316878 -0.819538
H -2.22992 -0.316889 0.819535
H -1.12028 2.10795 0.450953
H -1.12027 -2.10796 -0.450951
H 1.12028 -2.10795 0.450953
H 2.22992 -0.316878 -0.819538
H 2.22992 0.316889 0.819535
H 1.12027 2.10796 -0.450951
C 0.208906 0.750972 -1.91221
C -0.208906 -0.750972 -1.91221
H 1.04301 0.921837 -2.60376
H -0.617155 1.38713 -2.25376
H 0.617155 -1.38713 -2.25376
H -1.04301 -0.921837 -2.60376
C 0.208911 -0.750971 1.91221
C -0.208911 0.750971 1.91221
H 1.04301 -0.921831 2.60376
H -0.617146 -1.38713 2.25376
H 0.617146 1.38713 2.25376
H -1.04301 0.921831 2.60376
""")

#Dinitrogen-tetraoxide D_2h
N2O4=psr.system.MakeSystem("""
N 0.7005 0.0000 0.0015
O 1.3095 -1.0548 0.0007
O 1.3095 1.0548 -0.0018
N -0.7005 -0.0000 0.0003
O -1.3095 -1.0548 -0.0015
O -1.3095 1.0548 0.001
""")

#Benzene D_6h
Benzene=psr.system.MakeSystem("""
C -1.2131 -0.6884 0.0000
C -1.2028 0.7064 0.0001
C -0.0103 -1.3948 0.0000
C 0.0104 1.3948 -0.0001
C 1.2028 -0.7063 0.0000
C 1.2131 0.6884 0.0000
H -2.1577 -1.2244 0.0000
H -2.1393 1.2564 0.0001
H -0.0184 -2.4809 -0.0001
H 0.0184 2.4808 0.0000
H 2.1394 -1.2563 0.0001
H 2.1577 1.2245 0.0000
""")

#Ferrocene Staggard D5d
FerroceneStag=psr.system.MakeSystem("""
Fe	0.0000	0.0000	0.0000
C	0.0000	1.1988	1.8359
C	-1.1402	0.3705	1.8359
C	-0.7047	-0.9699	1.8359
C	0.7047	-0.9699	1.8359
C	1.1402	0.3705	1.8359
C	0.0000	-1.1988	-1.8359
C	-1.1402	-0.3705	-1.8359
C	-0.7047	0.9699	-1.8359
C	0.7047	0.9699	-1.8359
C	1.1402	-0.3705	-1.8359
H	0.0000	2.2707	1.8288
H	-2.1595	0.7017	1.8288
H	-1.3347	-1.8370	1.8288
H	1.3347	-1.8370	1.8288
H	2.1595	0.7017	1.8288
H	0.0000	-2.2707	-1.8288
H	-2.1595	-0.7017	-1.8288
H	-1.3347	1.8370	-1.8288
H	1.3347	1.8370	-1.8288
H	2.1595	-0.7017	-1.8288
""")
