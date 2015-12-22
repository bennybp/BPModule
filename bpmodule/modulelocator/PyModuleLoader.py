import os
from . import ModuleLocator,ModuleInfo
from bpmodule.exception import GeneralException
from bpmodule import output


class PyModuleLoader:
  def __init__(self, mlt):
    self.mlt = mlt

    # Stores a tupe of (module, creators) for a given key
    # (key = path to module)
    self.loaded = {}

  def LoadPyModule(self, mod, cppminfo):

    mpath = os.path.dirname(os.path.abspath(mod.__file__))

    if mpath in self.loaded:
        creators = self.loaded[mpath][1]
    else:
        if not hasattr(mod, "InsertSupermodule"):
            raise exception.GeneralException("Python supermodule doesn't have a InsertSupermodule function. Skipping",
                                                 "supermodule", supermodule)

        # See if there is an initialization function
        if hasattr(mod, "InitializeSupermodule"):
            output.Debug("Initializing supermodule: %1%\n", mpath)
            mod.InitializeSupermodule()
        else:
            output.Debug("Supermodule %1% doesn't have initialization function. Skipping\n", mpath)

        creators = mod.InsertSupermodule()
        self.loaded[mpath] = (mod, creators)


    if not creators.HasCreator(cppminfo.name):
      raise GeneralException("Error - this module cannot create this key", "name", cppminfo.name)

    self.mlt.InsertModule(creators, cppminfo)



  def CloseAll(self):
    output.Debug("Closing python modules\n")

    # Call all the finalize functions
    for mpath,mod in self.loaded.items():
        output.Debug("Closing %1%\n", mpath)

        if hasattr(mod[0], "FinalizeSupermodule"):
            output.Debug("Finalizing supermodule %1%\n", mpath)
        else:
            output.Debug("Supermodule %1% doesn't have a FinalizeSupermodule function. Skipping\n", mpath)
        
