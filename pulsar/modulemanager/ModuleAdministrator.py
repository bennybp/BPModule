import sys
import os
import importlib

from pulsar import *

# TODO - document me
class ModuleAdministrator(ModuleManager):
    """The Python interface to the ModuleManager.

    Attributes:
        paths: A list of places to look for modules
        supermodules: A dictionary of supermodules that have been loaded
    """

    def __init__(self):
        """Initializes an empty ModuleAdministrator."""
        super(ModuleAdministrator, self).__init__()
        self.paths = [ ]
        self.supermodules = { }


    def __enter__(self):
        return self


    def __exit__(self, exc_type, exc_value, traceback):
        pass


    def add_path(self, path):
        """Adds path to the list of paths to search"""
        self.paths.append(path)

    def load_supermodule(self,supermodule):
        if not supermodule :
            raise PulsarException("Provied an empty supermodule name\n")
        if supermodule in self.supermodules:
            raise PulsarException("Supermodule is already loaded")

        print_global_output("Importing supermodule {} \n".format(supermodule))
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
            check_supermodule(spath)

        except PulsarException as e:
            raise e

        except Exception as e:
            raise PulsarException("Unable to load supermodule",
                                             "supermodule", supermodule,
                                             "exception", str(e)) from None

        #Must be before next call to avoid infinite recursion
        self.supermodules[supermodule]=m

        #See if supermodule has an initialize function and if it does call it
        if hasattr(m,"initialize") and callable(getattr( m, "initialize")):
            m.initialize(self)



    def load_module(self, supermodule, modulename, modulekey):
        print_global_output("Importing {} module from supermodule {} under key {}\n".format(modulename, supermodule, modulekey))

        if not supermodule:
            raise PulsarException("Empty supermodule given to LoadModule")
        if not modulename:
            raise PulsarException("Empty modulename given to LoadModule")
        if not modulekey:
            raise PulsarException("Empty modulekey given to LoadModule")

        if supermodule not in self.supermodules:
            self.load_supermodule(supermodule)

        m = self.supermodules[supermodule]

        # Check to see if this supermodule actually contains
        # the desired module
        if not modulename in m.minfo:
            raise PulsarException("Supermodule doesn't have module!",
                                             "supermodule", supermodule,
                                             "modulename", modulename)



        # Extract the module info for the desired module
        minfo = m.minfo[modulename]

        print_global_output("\n")
        print_global_output("Loading module {} v{}\n".format(modulename, minfo["version"]))

        # Create a c++ moduleinfo structure
        # And fill it
        cppminfo = ModuleInfo()
        cppminfo.name = modulename
        cppminfo.path = os.path.dirname(m.__file__)

        # TODO: This handles the deprecated 'type' key. Remove in the future
        if "type" in minfo:
            cppminfo.language = minfo["type"]
            print_global_warning("Module {} from {} uses \"type\" rather than "
                                 "\"language\" in its module info. This "
                                 "is deprecated\n".format(modulename, supermodule, modulekey))
            
        else:
            cppminfo.language = minfo["language"]

        cppminfo.version = minfo["version"]
        cppminfo.description = minfo["description"]
        cppminfo.authors = minfo["authors"]
        cppminfo.refs = minfo["refs"]

        # Options inheritence
        if "base" in minfo:
            b = minfo["base"]
            cppminfo.base = b

            if b in base_options:
                dtmp = base_options[b].copy()

                # Update with the options in modinfo, which
                # will overwrite the defaults if necessary
                dtmp.update(minfo["options"])

                minfo["options"] = dtmp



        # Modpath is used for C++ SO modules
        # if it exists, append it
        if "modpath" in minfo:
            cppminfo.path = os.path.join(cppminfo.path, minfo["modpath"])


        # Create the options object
        for optkey,opt in minfo["options"].items():
            cppminfo.options.add_option(optkey, opt[0], opt[2], opt[3], opt[4], opt[1])


        # Now we can actually load it
        super(ModuleAdministrator, self).load_module_from_minfo(cppminfo, modulekey)

        print_global_debug("Imported module name {} from {} and associated key {}\n".format(modulename, supermodule, modulekey))
        print_global_output("\n")


    def sanity_check(self):
        # TODO May add more here
        super(ModuleAdministrator, self).test_all()
