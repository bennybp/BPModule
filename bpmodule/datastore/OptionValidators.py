class RangeCheck:
    def __init__(self, min, max, includemin = True, includemax = True):
        self.min = min
        self.max = max
        self.includemin = includemin
        self.includemax = includemax

        self.errstr = "Value must be in the range {}{},{}{}".format("[" if self.includemin else "(",
                                                                    self.min, self.max,
                                                                    "]" if self.includemax else ")")

    def Validate(self, value):
        if (value > self.min) and (value < self.max):
            return []
        elif (self.includemin == True) and (value == self.min):
            return []
        elif (self.includemax == True) and (value == self.max):
            return []
        else:
            err = [ "Value \"{}\" is not valid.".format(value) + self.errstr ]
            err.append(self.errstr)
            return err


class GreaterThan:
    def __init__(self, min, includemin = True):
        self.min = min
        self.includemin = includemin

        eqstr = " or equal to" if self.includemin else ""
        self.errstr = "Value must be greater than{} {}".format(eqstr, self.min)

    def Validate(self, value):
        if (value > self.min):
            return []
        elif (self.includemin == True) and (value == self.min):
            return []
        else:
            err = [ "Value \"{}\" is not valid.".format(value) + self.errstr ]
            err.append(self.errstr)
            return err


class LessThan:
    def __init__(self, max, includemax = True):
        self.max = max
        self.includemax = includemax

        eqstr = " or equal to" if self.includemax else ""
        self.errstr = "Value must be less than{} {}".format(eqstr, self.max)

    def Validate(self, value):
        if (value < self.max):
            return []
        elif (self.includemax == True) and (value == self.max):
            return []
        else:
            err = [ "Value \"{}\" is not valid.".format(value) + self.errstr ]
            err.append(self.errstr)
            return err






class InList:
    def __init__(self, lst):
        self.lst = []
        for l in lst:
            if type(l) == str:
                self.lst.append(l.lower())
            else:
                self.lst.append(l)


        self.errstr = [ ]
        for v in self.lst:
            self.errstr.append("    {}".format(v))



    def Validate(self, value):
        v = value
        if type(value) == str:
            v = v.lower()

        if v in self.lst:
            return []
        else:
            err = [ "Value \"{}\" is not valid. Value must be one of the following {} values".format(v, len(self.lst)) ]
            err.extend(self.errstr)
            return err

