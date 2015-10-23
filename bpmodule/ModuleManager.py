import sys
import os
import importlib

from . import modulelocator, output, CheckSupermodule



class ModuleManager(modulelocator.ModuleLocator):
    def __init__(self):
        super(ModuleManager, self).__init__()

        # Main module store and module loaders
        self.cml = modulelocator.CModuleLoader(self)
        self.pml = modulelocator.PyModuleLoader(self) 
        self.modmap = {}

        self.paths = [ ]


    def Finalize(self):
        output.Output("Deleting python modules\n")
        del self.pml

        output.Output("Deleting C modules\n")
        output.Output("Closing C handles\n")
        del self.cml


    def __enter__(self):
        return self


    def __exit__(self, exc_type, exc_value, traceback):
        self.Finalize()
        

    def AddPath(self, path):
        self.paths.append(path)


    def LoadModule(self, supermodule, name, key):
        output.Output("Importing %1% module from supermodule %2% for key %3%\n", name, supermodule, key)

        try:
            # update the paths
            oldpath = sys.path
            sys.path = self.paths
            sys.path.extend(oldpath)

            # Don't use RTLD_GLOBAL from now on
            olddl = sys.getdlopenflags()
            sys.setdlopenflags(os.RTLD_NOW)
            m = importlib.import_module(supermodule)
            sys.setdlopenflags(olddl)

            # reset the paths
            sys.path = oldpath

            # get full path of loaded supermodule
            spath = os.path.dirname(m.__file__)
            output.Warning("HERE: {}".format(spath))

            # check options, etc
            CheckSupermodule(spath)

        except Exception as e:
            raise exception.PyGeneralException(
                                    "Unable to load supermodule {}".format(supermodule),
                                    [
                                      ("Exception", str(e))
                                    ]
                                   )

        if not name in m.minfo:
            raise exception.PyGeneralException(
                                     "Supermodule doesn't have module!",
                                    [
                                      ("Supermodule", supermodule),
                                      ("Name", name)
                                    ]
                                   )


        minfo = m.minfo[name]

        path = os.path.dirname(m.__file__) + "/"

        output.Output("\n")
        output.Output("Loading module %1% v%2%\n", name, minfo["version"])

        # Copy the key and name to the dict
        minfo["key"] = key
        minfo["name"] = name

        # set the path
        minfo["path"] = path


        if minfo["type"] == "c_module":
            self.cml.LoadSO(key, minfo)
        elif minfo["type"] == "python_module":
            self.pml.LoadPyModule(key, m.CreateModule, minfo)
        output.Debug("Done importing module %1% from %2%\n", key, supermodule)
        output.Output("\n")

        self.modmap[key] = minfo;



    # Set the options for a module under a given key
    def SetOptions(self, key, useropt):
        if not key in self.modmap:
            raise exception.PyGeneralException(
                                     "Key not loaded, so I can't change the options!",
                                    [
                                      ("Key", key)
                                    ]
                                   )

        minfo = self.modmap[key]
        defopt = minfo["options"]


    #    if not CheckOptions(useropt, defopt):
    #      raise exception.PyGeneralException(
    #                 "Problem parsing options",
    #              [
    #                 ("ModuleName", minfo["name"]),
    #                 ("ModuleKey", minfo["key"]),
    #                 ("ModulePath", minfo["path"])
    #              ])

        super(ModuleManager, self).SetOptions(key, useropt);


    def SanityCheck(self):
        # May add more here
        super(ModuleManager, self).TestAll() 
