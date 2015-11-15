minfo = { "testtensor" :
  {
    "type"        : "c_module",
    "soname"      : "testtensor.so",
    "version"     : "0.1a",
    "description" : "Some test module",
    "authors"     : ["me", "myself", "I"],
    "refs"        : ["some paper", "some other paper"],
    "options"     : {
                        # Key               Default    Req     Check   Help
                        "double_opt_def":  ( "float",   6.02e23,  False,   None,  "Some help string"),
                    }
  }
}


