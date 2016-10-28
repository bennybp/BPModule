import sys
import os
import importlib

from .pulsar_core import ModuleManager

# TODO - document me
class ModuleAdministrator(ModuleManager):
    """The Python interface to the ModuleManager.

    Attributes:
        paths: A list of places to look for modules
    """

    def __init__(self):
        """Initializes an empty ModuleAdministrator."""
        super(ModuleAdministrator, self).__init__()
        self.paths = [ ]


    def __enter__(self):
        return self


    def __exit__(self, exc_type, exc_value, traceback):
        pass
        

    def add_path(self, path):
        """Adds path to the list of paths to search"""
        self.paths.append(path)


    def load_module(self, supermodule, modulename, modulekey):
        output.print_global_output("Importing {} module from supermodule {} under key {}\n".format(modulename, supermodule, modulekey))

        if supermodule == "" or supermodule == None:
            raise exception.GeneralException("Empty supermodule given to LoadModule") 
        if modulename == "" or modulename == None:
            raise exception.GeneralException("Empty modulename given to LoadModule") 
        if modulekey == "" or modulekey == None:
            raise exception.GeneralException("Empty modulekey given to LoadModule") 

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

        output.print_global_output("\n")
        output.print_global_output("Loading module {} v{}\n".format(modulename, minfo["version"]))

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

        # Options inheritence
        if "base" in minfo:
            b = minfo["base"]
            cppminfo.base = b

            if b in modulebase.base_options:
                dtmp = modulebase.base_options[b].copy()

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

        output.print_global_debug("Imported module name {} from {} and associated key {}\n".format(modulename, supermodule, modulekey))
        output.print_global_output("\n")


    def sanity_check(self):
        # TODO May add more here
        super(ModuleAdministrator, self).test_all() 
