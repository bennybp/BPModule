import os
import sys

# Add the pulsar path
thispath = os.path.dirname(os.path.realpath(__file__))
psrpath = os.path.join(os.path.dirname(thispath), "../", "modules")
sys.path.insert(0, psrpath)

from pulsar.system import CreateAtom

nonane = [ CreateAtom([    0.0000,    0.0000,    0.3723 ], 6),
           CreateAtom([    0.0000,    1.2842,   -0.4867 ], 6),
           CreateAtom([    0.0000,   -1.2842,   -0.4867 ], 6),
           CreateAtom([    0.0000,    2.5682,    0.3723 ], 6),
           CreateAtom([    0.0000,   -2.5682,    0.3723 ], 6),
           CreateAtom([    0.0000,    3.8525,   -0.4867 ], 6),
           CreateAtom([    0.0000,   -3.8525,   -0.4867 ], 6),
           CreateAtom([    0.0000,    5.1338,    0.3697 ], 6),
           CreateAtom([    0.0000,   -5.1338,    0.3697 ], 6),
           CreateAtom([    0.8756,    0.0000,    1.0188 ], 6),
           CreateAtom([  -0.8756,    0.0000,    1.0188 ], 1),
           CreateAtom([   0.8756,    1.2842,   -1.1332 ], 1),
           CreateAtom([  -0.8756,    1.2842,   -1.1332 ], 1),
           CreateAtom([  -0.8756,   -1.2842,   -1.1332 ], 1),
           CreateAtom([   0.8756,   -1.2842,   -1.1332 ], 1),
           CreateAtom([   0.8756,    2.5688,    1.0188 ], 1),
           CreateAtom([  -0.8756,    2.5688,    1.0188 ], 1),
           CreateAtom([  -0.8756,   -2.5688,    1.0188 ], 1),
           CreateAtom([   0.8756,   -2.5688,    1.0188 ], 1),
           CreateAtom([  -0.8757,    3.8523,   -1.1332 ], 1),
           CreateAtom([   0.8757,    3.8523,   -1.1332 ], 1),
           CreateAtom([   0.8757,   -3.8523,   -1.1332 ], 1),
           CreateAtom([  -0.8757,   -3.8523,   -1.1332 ], 1),
           CreateAtom([   0.0000,    6.0168,   -0.2627 ], 1),
           CreateAtom([  -0.8795,    5.1714,    1.0056 ], 1),
           CreateAtom([   0.8795,    5.1714,    1.0056 ], 1),
           CreateAtom([   0.0000,   -6.0168,   -0.2627 ], 1),
           CreateAtom([   0.8795,   -5.1714,    1.0056 ], 1),
           CreateAtom([  -0.8795,   -5.1714,    1.0056 ], 1)
          ]


water  = [ 
           CreateAtom([0.0000000000,  0.0000000000, -0.1294768896], 8),
           CreateAtom([0.0000000000, -1.4941867446,  1.0274460989], 1),
           CreateAtom([0.0000000000,  1.4941867446,  1.0274460989], 1)
         ]

water2 = [ 
           CreateAtom([ 0.000000000000,  -0.143225816552,   0.000000000000], 8),
           CreateAtom([ 1.638036840407,   1.136548822547,  -0.000000000000], 1),
           CreateAtom([-1.638036840407,   1.136548822547,  -0.000000000000], 1)
         ]

