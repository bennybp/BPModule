import sys
import os
import importlib

from . import modulelocator, output, exception, datastore, CheckSupermodule



class ModuleManager(modulelocator.ModuleLocator):
    def __init__(self):
        super(ModuleManager, self).__init__()

        # Main module store and module loaders
        self.cml = modulelocator.CppModuleLoader(self)
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


    def LoadModule(self, supermodule, modulename, modulekey):
        output.Output("Importing %1% module from supermodule %2% under key %3%\n", modulename, supermodule, modulekey)

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

        if not modulename in m.minfo:
            raise exception.GeneralException("Supermodule doesn't have module!",
                                             "supermodule", supermodule,
                                             "modulename", modulename)


        minfo = m.minfo[modulename]

        path = os.path.dirname(m.__file__) + "/"

        output.Output("\n")
        output.Output("Loading module %1% v%2%\n", modulename, minfo["version"])

        # Create a c++ moduleinfo
        cppminfo = modulelocator.ModuleInfo()
        cppminfo.key = modulekey
        cppminfo.name = modulename
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
            cppminfo.options.AddOption(oph)
        

        # actually load
        if minfo["type"] == "c_module":
            self.cml.LoadSO(cppminfo)
        elif minfo["type"] == "python_module":
            if not hasattr(m, "CreateModule"):
                raise exception.GeneralException("Python supermodule doesn't have a CreateModule function",
                                                 "supermodule", supermodule)

            self.pml.LoadPyModule(m.CreateModule, cppminfo)
        output.Debug("Done importing module %1% from %2%\n", modulekey, supermodule)
        output.Output("\n")

        self.modmap[modulekey] = minfo;



    def SanityCheck(self):
        # May add more here
        super(ModuleManager, self).TestAll() 
