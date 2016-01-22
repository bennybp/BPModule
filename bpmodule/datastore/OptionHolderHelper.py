from . import *
from bpmodule.exception import GeneralException


makermap = { OptionType.Int         : OptionHolder_Int,
             OptionType.Bool        : OptionHolder_Bool,
             OptionType.Float       : OptionHolder_Float,
             OptionType.String      : OptionHolder_String,
             OptionType.ListInt     : OptionHolder_ListInt,
             OptionType.ListBool    : OptionHolder_ListBool,
             OptionType.ListFloat   : OptionHolder_ListFloat,
             OptionType.ListString  : OptionHolder_ListString,
             OptionType.SetInt      : OptionHolder_SetInt,
             OptionType.SetBool     : OptionHolder_SetBool,
             OptionType.SetFloat    : OptionHolder_SetFloat,
             OptionType.SetString   : OptionHolder_SetString,
           }



def MakeOptionHolder(key, opttype, required, validator, help, default):
    if not opttype in makermap:
        raise GeneralException("Invalid python type for an option",
                               "type", str(opttype),
                               "key", key)

    if default == None:
        return makermap[opttype](key, required, validator, help)
    else:
        return makermap[opttype](key, required, validator, help, default)






