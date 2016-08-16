# SO file
from ..pulsar_core.datastore import *

# Other files
from .OptionValidators import *

def set_options(mm,daopts):
   """
   A convience function for setting a lot of options at once
   
      Most modules have lots of options.  To clean up the input files somewhat
      I have created this function which can be used like:
      
      psr.datastore.set_options(mm,{"Module1's Key":{"Option 1 for mod 1":value,
                                                    "Option 2 for mod 1":value},
                                    "Module2's Key":{Option 1 for mod 1":value,
                                                    "Option 2 for mod 2":value}
      })
      
      Obviously you can specify more options and more modules if you like.
   
      mm (psr.modulemanager.ModuleManager) : The module manager
      daopts (dict of dict) : A nested dictionary mapping module keys to option
                              key to option value
   """
   for mod in daopts:
      for opt in daopts[mod]:
         mm.change_option(mod,opt,daopts[mod][opt])
