import os
import sys

# Add the bpmodule path
thispath = os.path.dirname(os.path.realpath(__file__))
bppath = os.path.join(os.path.dirname(thispath), "../", "modules")
sys.path.insert(0, bppath)

from bpmodule.system import CreateAtom

nonane = [ CreateAtom(0, [    0.0000,    0.0000,    0.3723 ], 6),
           CreateAtom(1, [    0.0000,    1.2842,   -0.4867 ], 6),
           CreateAtom(2, [    0.0000,   -1.2842,   -0.4867 ], 6),
           CreateAtom(3, [    0.0000,    2.5682,    0.3723 ], 6),
           CreateAtom(4, [    0.0000,   -2.5682,    0.3723 ], 6),
           CreateAtom(5, [    0.0000,    3.8525,   -0.4867 ], 6),
           CreateAtom(6, [    0.0000,   -3.8525,   -0.4867 ], 6),
           CreateAtom(7, [    0.0000,    5.1338,    0.3697 ], 6),
           CreateAtom(8, [    0.0000,   -5.1338,    0.3697 ], 6),
           CreateAtom(9, [    0.8756,    0.0000,    1.0188 ], 6),
           CreateAtom(10, [  -0.8756,    0.0000,    1.0188 ], 1),
           CreateAtom(11, [   0.8756,    1.2842,   -1.1332 ], 1),
           CreateAtom(12, [  -0.8756,    1.2842,   -1.1332 ], 1),
           CreateAtom(13, [  -0.8756,   -1.2842,   -1.1332 ], 1),
           CreateAtom(14, [   0.8756,   -1.2842,   -1.1332 ], 1),
           CreateAtom(15, [   0.8756,    2.5688,    1.0188 ], 1),
           CreateAtom(16, [  -0.8756,    2.5688,    1.0188 ], 1),
           CreateAtom(17, [  -0.8756,   -2.5688,    1.0188 ], 1),
           CreateAtom(18, [   0.8756,   -2.5688,    1.0188 ], 1),
           CreateAtom(19, [  -0.8757,    3.8523,   -1.1332 ], 1),
           CreateAtom(20, [   0.8757,    3.8523,   -1.1332 ], 1),
           CreateAtom(21, [   0.8757,   -3.8523,   -1.1332 ], 1),
           CreateAtom(22, [  -0.8757,   -3.8523,   -1.1332 ], 1),
           CreateAtom(23, [   0.0000,    6.0168,   -0.2627 ], 1),
           CreateAtom(24, [  -0.8795,    5.1714,    1.0056 ], 1),
           CreateAtom(25, [   0.8795,    5.1714,    1.0056 ], 1),
           CreateAtom(26, [   0.0000,   -6.0168,   -0.2627 ], 1),
           CreateAtom(27, [   0.8795,   -5.1714,    1.0056 ], 1),
           CreateAtom(28, [  -0.8795,   -5.1714,    1.0056 ], 1)
          ]
