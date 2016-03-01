import sys
import os
import importlib

from . import modulemanager, output, exception, datastore, CheckSupermodule


# TODO - document me
class ModuleAdministrator(modulemanager.ModuleManager):
    def __init__(self):
        super(ModuleAdministrator, self).__init__()

        # Module search paths
        self.paths = [ ]


    def __enter__(self):
        return self


    def __exit__(self, exc_type, exc_value, traceback):
        pass
        

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

            # Actually importing here
            m = importlib.import_module(supermodule)

            # reset the flags and paths
            sys.setdlopenflags(olddl)
            sys.path = oldpath

            # get full path of loaded supermodule
            spath = os.path.dirname(m.__file__)

            # check options, etc
            CheckSupermodule(spath)

        except exception.GeneralException as e:
            raise e

        except Exception as e:
            raise exception.GeneralException("Unable to load supermodule",
                                             "supermodule", supermodule,
                                             "exception", str(e)) from None

        # Check to see if this supermodule actually contains
        # the desired module
        if not modulename in m.minfo:
            raise exception.GeneralException("Supermodule doesn't have module!",
                                             "supermodule", supermodule,
                                             "modulename", modulename)


        # Extract the module info for the desired module
        minfo = m.minfo[modulename]

        output.Output("\n")
        output.Output("Loading module %1% v%2%\n", modulename, minfo["version"])

        # Create a c++ moduleinfo structure
        # And fill it
        cppminfo = modulemanager.ModuleInfo()
        cppminfo.name = modulename
        cppminfo.path = spath
        cppminfo.type = minfo["type"]
        cppminfo.version = minfo["version"]
        cppminfo.description = minfo["description"]
        cppminfo.authors = minfo["authors"]
        cppminfo.refs = minfo["refs"]

        # Modpath is used for C++ SO modules
        # if it exists, append it
        if "modpath" in minfo:
            cppminfo.path = os.path.join(cppminfo.path, minfo["modpath"])


        # Create the options object
        for optkey,opt in minfo["options"].items():
            oph = datastore.MakeOptionHolder(optkey, opt[0], opt[2], opt[3], opt[4], opt[1])
            cppminfo.options.AddOption(oph)
        

        # Now we can actually load it
        super(ModuleAdministrator, self).LoadModuleFromModuleInfo(cppminfo)

        output.Debug("Imported module name %1% from %2%\n", modulename, supermodule)

        super(ModuleAdministrator, self).AddKey(modulekey, modulename)

        output.Debug("Associated key %1% with module %2%\n", modulekey, supermodule)
        output.Output("\n")


    def SanityCheck(self):
        # TODO May add more here
        super(ModuleAdministrator, self).TestAll() 
