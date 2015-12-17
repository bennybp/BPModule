from . import GeneralException as GeneralException_

class GeneralException(Exception):
  def __init__(self, whatstr, *args):
    self.gex = GeneralException_(whatstr)

    self.AppendInfo(*args)


  def AppendInfo(self, *args):
    lst = list(args)
    if len(lst) % 2: # if isn't even
      lst.append("DEVELOPER ERROR. I WAS NOT PASSED AN EVEN NUMBER")
    plist = [ lst[i:i+2] for i in range(0, len(lst), 2) ]

    for i in plist:
        self.gex.AppendInfo(str(i[0]), str(i[1]))


  def __str__(self):
    return self.gex.what()

