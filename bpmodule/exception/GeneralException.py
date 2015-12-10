from . import GeneralException as GeneralException_

class GeneralException(Exception):
  def __init__(self, whatstr, exinfo = None):
    self.gex = GeneralException_(whatstr)

    if exinfo:
        for i in exinfo:
          self.gex.AppendInfo(str(i[0]), str(i[1]))

  def Append(self, lst):
    for i in lst:
        self.gex.AppendInfo(str(i[0]), str(i[1]))

  def __str__(self):
    return self.gex.ExceptionString()

