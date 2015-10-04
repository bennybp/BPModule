from bpmodule import output

class RangeCheck:
    def __init__(self, min, max, includemin = True, includemax = True):
        self.min = min
        self.max = max
        self.includemin = includemin
        self.includemax = includemax

    def Validate(self, value):
        if (value > self.min) and (value < self.max):
            return True
        elif (self.includemin == True) and (value == self.min):
            return True
        elif (self.includemax == True) and (value == self.max):
            return True
        else:
            return False

    def Desc(self):
        return output.FormatStr("Value must be in the range %1%%2%,%3%%4%",
                                "[" if self.includemin else "(",
                                self.min, self.max,
                                "]" if self.includemax else ")")



class InList:
    def __init__(self, lst):
        self.lst = lst

    def Validate(self, value):
        return (value in self.lst)


    def Desc(self):
        s = output.FormatStr("Value of must be one of the following %1% values\n", len(self.lst))
        for v in self.lst:
            s += output.FormatStr("%1%\n", v)

        return s
