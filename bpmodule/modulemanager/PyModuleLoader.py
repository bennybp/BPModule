import os
import sys
import importlib
from bpmodule.exception import GeneralException
from bpmodule import output


class PyModuleLoader:
  def __init__(self, mlt):
    self.mlt = mlt

    # Stores a tupe of (module, creators) for a given key
    # (key = path to module)
    self.loaded = {}

  def LoadPyModule(self, minfo):

    if not minfo.path in self.loaded:
        # split the path - we need to add the directory above
        # to the import path
        mpath,mimportname = os.path.split(os.path.abspath(minfo.path))

        # update the paths
        oldpath = sys.path
        sys.path = mpath

        # Don't use RTLD_GLOBAL
        olddl = sys.getdlopenflags()
        sys.setdlopenflags(os.RTLD_NOW)
        m = importlib.import_module(mimportname)
        sys.setdlopenflags(olddl)

        # reset the paths
        sys.path = oldpath

        if not hasattr(m, "InsertSupermodule"):
            raise exception.GeneralException("Python supermodule doesn't have a InsertSupermodule function. Skipping",
                                             "supermodule", minfo.path)

        # See if there is an initialization function
        if hasattr(m, "InitializeSupermodule"):
            output.Debug("Initializing supermodule: %1%\n", minfo.path)
            mod.InitializeSupermodule()
        else:
            output.Debug("Supermodule %1% doesn't have initialization function. Skipping\n", minfo.path)


        creators = m.InsertSupermodule()

        self.loaded[mpath] = (m, creators)

        if not creators.HasCreator(minfo.name):
          raise GeneralException("Error - this module cannot create this key", "name", minfo.name)

        self.mlt.InsertModule(creators, minfo)

  def CloseAll(self):
    output.Debug("Closing python modules\n")

    # Call all the finalize functions
    for mpath,mod in self.loaded.items():
        output.Debug("Closing %1%\n", mpath)

        if hasattr(mod[0], "FinalizeSupermodule"):
            output.Debug("Finalizing supermodule %1%\n", mpath)
        else:
            output.Debug("Supermodule %1% doesn't have a FinalizeSupermodule function. Skipping\n", mpath)
        
