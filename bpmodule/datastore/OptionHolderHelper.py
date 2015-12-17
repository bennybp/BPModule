from . import *
from bpmodule.python import PythonType
from bpmodule.exception import GeneralException


makermap = { PythonType.Int         : OptionHolder_int,
             PythonType.Bool        : OptionHolder_bool,
             PythonType.Float       : OptionHolder_float,
             PythonType.String      : OptionHolder_str,
             PythonType.ListInt     : OptionHolder_int,
             PythonType.ListBool    : OptionHolder_bool,
             PythonType.ListFloat   : OptionHolder_float,
             PythonType.ListString  : OptionHolder_str,
           }



def MakeOptionHolder(key, pytype, required, validator, help, default):
    if not pytype in makermap:
        raise GeneralException("Invalid python type for an option",
                               "type", str(pytype),
                               "key", key)

    if default == None:
        return makermap[pytype](key, pytype, required, validator, help)
    else:
        return makermap[pytype](key, pytype, required, validator, help, default)






