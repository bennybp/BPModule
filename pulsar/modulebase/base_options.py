from pulsar import OptionType
from pulsar import RangeCheck, GreaterThan, LessThan

base_options = {

  "Test_Base" :
  {
  },

  "SystemFragmenter" :
  {
    "TRUNCATION_ORDER" : (OptionType.Int, 1, False, GreaterThan(0),
                          'Unions of up to how many monomers should be taken'),
    "DISTANCE_THRESHOLDS" : (OptionType.DictIntFloat,{},False,
                    None,'A int->float dictionary where the int, call it n, is'\
                    ' the n-mer and the float is the maximum distance '\
                    'that the n monomers can be apart, e.g. {2:3.14} means '\
                    'that dimers whose centers of mass are more than 3.14 '\
                    'Angstroms apart are excluded.')
  },

  "EnergyMethod" :
  {
    "MAX_DERIV"       :  (OptionType.Int, 255, False, RangeCheck(0, 255),
                          'What order analytic derivatives are available (actually '\
                          'have arbitrary order available)'),

    "BASIS_SET"       :  (OptionType.String, "Primary", False, None,
                          'Tag representing the basis set in the system'),
    "FDIFF_DISPLACEMENT" :(OptionType.Float,0.005,False,None,
                          'The displacement (a.u.) for finite difference computations'),
    "FDIFF_STENCIL_SIZE" : (OptionType.Int,3,False,None,
                          'The number of stencil points (points at which the function '\
                          'will be sampled) for the finite difference computation.'),
    #RMR This only makes sense for iterative methods and a priori it is not clear
    #to me that most methods are iterative
    "EGY_TOLERANCE"     :  (OptionType.Float, 1.0e-8, False, GreaterThan(0),
                          'Tolerance for energy convergence'),
  }
}

