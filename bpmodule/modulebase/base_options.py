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

    "E_TOLERANCE"     :  (OptionType.Float, 1.0e-8, False, GreaterThan(0),
                          'Tolerance for energy convergence'),
  }
}

