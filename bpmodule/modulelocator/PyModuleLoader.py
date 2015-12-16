from . import ModuleLocator,ModuleInfo
from bpmodule.exception import GeneralException

# TODO - replace generalexception 

class PyModuleLoader:
  def __init__(self, mlt):
    self.mlt = mlt

    # TODO - store those already loaded?
  
  def LoadPyModule(self, key, func, minfo):
    creators = func()

    if not creators.HasCreator(minfo["name"]):
      raise GeneralException("Error - this module cannot create this key", "name", minfo["name"])

    pyminfo = ModuleInfo(minfo)
    self.mlt.InsertModule(key, creators, pyminfo)
