class RangeCheck:
    def __init__(self, min, max, includemin = True, includemax = True):
        self.min = min
        self.max = max
        self.includemin = includemin
        self.includemax = includemax

        self.errstr = [ "Value must be in the range {}{},{}{}".format("[" if self.includemin else "(",
                                                                      self.min, self.max,
                                                                      "]" if self.includemax else ")")
                      ]

    def Validate(self, value):
        if (value > self.min) and (value < self.max):
            return True
        elif (self.includemin == True) and (value == self.min):
            return True
        elif (self.includemax == True) and (value == self.max):
            return True
        else:
            err = [ "Value \"{}\" is not valid".format(value) ]
            err.extend(self.errstr)
            return err







class InList:
    def __init__(self, lst):
        self.lst = []
        for l in lst:
            if type(l) == str:
                self.lst.append(l.lower())
            else:
                self.lst.append(l)


        self.errstr = [ "Value of must be one of the following {} values".format(len(self.lst)) ]
        for v in self.lst:
            self.errstr.append("    {}".format(v))

        

    def Validate(self, value):
        v = value
        if type(value) == str:
            v = v.lower()

        if not v in self.lst:
            err = [ "Value \"{}\" is not valid".format(v) ]
            err.extend(self.errstr)
            return err

