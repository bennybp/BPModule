import os
import sys

# Add the pulsar path
thispath = os.path.dirname(os.path.realpath(__file__))
psrpath = os.path.join(os.path.dirname(thispath), "../", "modules")
sys.path.insert(0, psrpath)

from pulsar.datastore import OptionType

# map to optionstypes,
opttypemap = { "int"   : OptionType.Int,
               "float" : OptionType.Float,
               "bool"  : OptionType.Bool,
               "str"   : OptionType.String,
               "setint"   : OptionType.SetInt,
               "setfloat" : OptionType.SetFloat,
               "setbool"  : OptionType.SetBool,
               "setstr"   : OptionType.SetString,
               "listint"   : OptionType.ListInt,
               "listfloat" : OptionType.ListFloat,
               "listbool"  : OptionType.ListBool,
               "liststr"   : OptionType.ListString,
               "dictintint"   : OptionType.DictIntInt,
               "dictintfloat" : OptionType.DictIntFloat,
               "dictintbool"  : OptionType.DictIntBool,
               "dictintstr"   : OptionType.DictIntString,
               "dictstrint"   : OptionType.DictStringInt,
               "dictstrfloat" : OptionType.DictStringFloat,
               "dictstrbool"  : OptionType.DictStringBool,
               "dictstrstr"   : OptionType.DictStringString
            }


# valid data for some types
validvals = [ ("int" ,   5),
              ("float" , 5.0),
              ("bool" , True),
              ("str", "Hello"),
              ("listint" , [-1, 0, 1]),
              ("listfloat" , [-1.0, 0.0, 1.0]),
              ("listbool" , [False, False, True]),
              ("liststr" , ["My", "Test", "Strings"]),
              ("setint" , { -1, 0, 1, 10 }),
              ("setfloat" , { -1.1, 0.1, 1.1, 10.1 }),
              ("setbool" , { True, False }),
              ("setstr" , { "My", "Set", "of", "Strings" }),
              ("dictintint", { 1 : 10, -500: 123 }),
              ("dictintfloat", { 1 : 10.2, -500 : 123.123 }),
              ("dictintbool", { 1 : True, -500 : False, 123: True }),
              ("dictintstr", { 1 : "1", -500 : "-500", 123: "123" }),
              ("dictstrint", { "k1" : 10, "-500": 123 }),
              ("dictstrfloat", { "k1" : 10.2, "-500" : -123.123 }),
              ("dictstrbool", { "k1" : True, "-500" : False, "123": True }),
              ("dictstrstr", { "k1" : "1", "-500" : "-500", "123": "123" })
            ]


# empty lists, sets, dicts
emptyvals = [ ("listint" , []),
              ("listfloat" , []),
              ("listbool" , []),
              ("liststr" , []),
              ("setint" , set()),
              ("setfloat" , set()),
              ("setbool" , set()),
              ("setstr" , set()),
              ("dictintint", dict()),
              ("dictintfloat", dict()),
              ("dictintbool", dict()),
              ("dictintstr", dict()),
              ("dictstrint", dict()),
              ("dictstrfloat", dict()),
              ("dictstrbool", dict()),
              ("dictstrstr", dict())
            ]


allvals = validvals + emptyvals
