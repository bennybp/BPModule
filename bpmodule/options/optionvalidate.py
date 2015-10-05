from bpmodule import output

class RangeCheck:
    def __init__(self, min, max, includemin = True, includemax = True):
        self.min = min
        self.max = max
        self.includemin = includemin
        self.includemax = includemax

        # Read in from the C++ code as the description of this validator
        self.descstr = output.FormatStr("Value must be in the range %1%%2%,%3%%4%",
                                        "[" if self.includemin else "(",
                                        self.min, self.max,
                                        "]" if self.includemax else ")")

    def Validate(self, value):
        if (value > self.min) and (value < self.max):
            return True
        elif (self.includemin == True) and (value == self.min):
            return True
        elif (self.includemax == True) and (value == self.max):
            return True
        else:
            return False


class InList:
    def __init__(self, lst):
        self.lst = []
        for l in lst:
            if type(l) == str:
                self.lst.append(l.lower())
            else:
                self.lst.append(l)


        # Read in from the C++ code as the description of this validator
        self.descstr = output.FormatStr("Value of must be one of the following %1% values\n", len(self.lst)) 
        for v in self.lst:
            self.descstr += output.FormatStr("%1%\n", v)

        

    def Validate(self, value):
        if type(value) == str:
            return (value.lower() in self.lst)
        else:
            return (value in self.lst)

