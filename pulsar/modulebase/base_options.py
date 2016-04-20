from bpmodule.datastore import OptionType
from bpmodule.datastore import RangeCheck, GreaterThan, LessThan

base_options = {

  "Test_Base" :
  {
  },

  "SystemFragmenter" :
  {
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
    "E_TOLERANCE"     :  (OptionType.Float, 1.0e-8, False, GreaterThan(0),
                          'Tolerance for energy convergence'),
  }
}

