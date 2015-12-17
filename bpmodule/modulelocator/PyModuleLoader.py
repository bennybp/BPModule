from . import ModuleLocator,ModuleInfo
from bpmodule.exception import GeneralException

# TODO - replace generalexception 

class PyModuleLoader:
  def __init__(self, mlt):
    self.mlt = mlt

    # TODO - store those already loaded?
  
  def LoadPyModule(self, key, func, cppminfo):
    creators = func()

    if not creators.HasCreator(cppminfo.name):
      raise GeneralException("Error - this module cannot create this key", "name", cppminfo.name)

    self.mlt.InsertModule(key, creators, cppminfo)
