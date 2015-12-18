import sys
import os
import importlib

from . import modulelocator, output, exception, datastore, CheckSupermodule



class ModuleManager(modulelocator.ModuleLocator):
    def __init__(self):
        super(ModuleManager, self).__init__()

        # Main module store and module loaders
        self.cml = modulelocator.CModuleLoader(self)
        self.pml = modulelocator.PyModuleLoader(self) 
        self.modmap = {}

        self.paths = [ ]


    def Finalize(self):

        #################################################
        # WARNING WARNING WARNING
        # Clearing the cache and store MUST be done BEFORE unloading
        # the modules or else deleting elements will cause a segfault.
        # The GenericHolder is a template, so the code for
        # the destructors exists in the modules
        #################################################
        super(ModuleManager, self).ClearCache()
        super(ModuleManager, self).ClearStore()

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

            # check options, etc
            #CheckSupermodule(spath)

        except exception.GeneralException as e:
            raise e

        except Exception as e:
            raise exception.GeneralException("Unable to load supermodule",
                                             "supermodule", supermodule,
                                             "exception", str(e)) from None

        if not name in m.minfo:
            raise exception.GeneralException("Supermodule doesn't have module!",
                                             "supermodule", supermodule,
                                             "name", name)


        minfo = m.minfo[name]

        path = os.path.dirname(m.__file__) + "/"

        output.Output("\n")
        output.Output("Loading module %1% v%2%\n", name, minfo["version"])

        # Create a c++ moduleinfo
        cppminfo = modulelocator.ModuleInfo()
        cppminfo.key = key
        cppminfo.name = name
        cppminfo.path = path
        cppminfo.type = minfo["type"]

        if "soname" in minfo:
            cppminfo.soname = minfo["soname"]

        cppminfo.version = minfo["version"]
        cppminfo.description = minfo["description"]
        cppminfo.authors = minfo["authors"]
        cppminfo.refs = minfo["refs"]


        # Create the options
        for optkey,opt in minfo["options"].items():
            oph = datastore.MakeOptionHolder(optkey, opt[0], opt[2], opt[3], opt[4], opt[1])
            print(oph)
            cppminfo.options.AddOption(oph)
        

        # actually load
        if minfo["type"] == "c_module":
            self.cml.LoadSO(key, cppminfo)
        elif minfo["type"] == "python_module":  # TODO - check for CreateModule
            self.pml.LoadPyModule(key, m.CreateModule, cppminfo)
        output.Debug("Done importing module %1% from %2%\n", key, supermodule)
        output.Output("\n")

        self.modmap[key] = minfo;



    def SanityCheck(self):
        # May add more here
        super(ModuleManager, self).TestAll() 
