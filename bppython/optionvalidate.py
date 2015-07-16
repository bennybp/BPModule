from bppython import Error, BPModuleException

class RangeCheck:
  def __init__(self, min, max, includemin = True, includemax = True):
    self.min = min
    self.max = max

  def Check(key, value):
    if (value > self.min) and (value < self.max):
      return True
    elif (includemin == True) and (value == self.min):
      return True
    elif (includemax == True) and (value == self.max):
      return True
    else:
      Error("Error validating key \"%1%\"\n", key)
      Error("Value {} not in the range %1%%2%,%3%%4%\n",
                    value,
                    "[" if includemin else "(",
                    self.min, self.max,
                    "]" if includemax else ")")
      return False



class InList:
  def __init__(self, lst):
    self.lst = lst

  def Check(key, value):
    if value in lst:
        return True
    else: 
      Error("Error validating key \"%1%\"\n", key)
      Error("Value %1% not a valid option. Valid options\n", value)
      for v in self.lst:
        Error("    %1%", v)
      return False

