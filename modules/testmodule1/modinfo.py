minfo = { "testmodule1" :
  {
    "type"        : "c_module",
    "soname"      : "testmodule1.so",
    "version"     : "0.1a",
    "description" : "Some test module",
    "authors"     : ["me", "myself", "I"],
    "refs"        : ["some paper", "some other paper"],
    "options"     : {
                        # Key                 Type    Default     Req     Check   Help
                        "bool_opt_def":    (  "bool",  False,     False,   None,  "Some help string"),
                        "bool_opt":        (  "bool",  None,      False,   None,  "Some help string"),

                        "int_opt_def":     (  "int",    1,        False,   None,  "Some help string"),
                        "int_opt":         (  "int",    None,     False,   None,  "Some help string"),

                        "double_opt_def":  ( "float",   6.02e23,  False,   None,  "Some help string"),
                        "double_opt":      ( "float",   None,     False,   None,  "Some help string"),

                        "str_opt_def":     (  "str",    "Hi",     False,   None,  "Some help string"),
                        "str_opt":         (  "str",    None,     False,   None,  "Some help string"),
                    }
  }
}


