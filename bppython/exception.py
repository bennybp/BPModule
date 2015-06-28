import sys
import os

from bppython import *


class BPModuleException(Exception):
  def __init__(self, whatstr, info = None):
    self.s = "what() = " + whatstr + "\n"

    if info:
        for i in info:
          self.s += "{:>24} : {:<}\n".format(i[0], i[1])


  def __str__(self):
    return self.s

