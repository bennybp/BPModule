import sys
import os
import importlib

from . import modulemanager, output, exception, datastore, CheckSupermodule



class ModuleAdministrator(modulemanager.ModuleManager):
    def __init__(self):
        super(ModuleAdministrator, self).__init__()

        # TODO - remove me?
        #self.modmap = {}

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

        path = os.path.dirname(m.__file__)

        output.Output("\n")
        output.Output("Loading module %1% v%2%\n", modulename, minfo["version"])

        # Create a c++ moduleinfo
        cppminfo = modulemanager.ModuleInfo()
        cppminfo.name = modulename
        cppminfo.path = path

        if "modpath" in minfo:
            cppminfo.path = os.path.join(cppminfo.path, minfo["modpath"])

        cppminfo.type = minfo["type"]

        cppminfo.version = minfo["version"]
        cppminfo.description = minfo["description"]
        cppminfo.authors = minfo["authors"]
        cppminfo.refs = minfo["refs"]


        # Create the options
        for optkey,opt in minfo["options"].items():
            oph = datastore.MakeOptionHolder(optkey, opt[0], opt[2], opt[3], opt[4], opt[1])
            cppminfo.options.AddOption(oph)
        

        # actually load
        super(ModuleAdministrator, self).LoadModuleFromModuleInfo(cppminfo)

        output.Debug("Imported module name %1% from %2%\n", modulename, supermodule)

        super(ModuleAdministrator, self).AddKey(modulekey, modulename)

        output.Debug("Associated key %1% with module %2%\n", modulekey, supermodule)
        output.Output("\n")

        # TODO - remove me?
        #self.modmap[modulekey] = minfo;



    def SanityCheck(self):
        # May add more here
        super(ModuleAdministrator, self).TestAll() 
