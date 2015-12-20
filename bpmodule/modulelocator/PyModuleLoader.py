from . import ModuleLocator,ModuleInfo
from bpmodule.exception import GeneralException

# TODO - replace generalexception 

class PyModuleLoader:
  def __init__(self, mlt):
    self.mlt = mlt
    self.loaded = {}

  def LoadPyModule(self, func, cppminfo):
    creators = func()

    if not creators.HasCreator(cppminfo.name):
      raise GeneralException("Error - this module cannot create this key", "name", cppminfo.name)

    self.mlt.InsertModule(creators, cppminfo)
