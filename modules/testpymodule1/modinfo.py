#!/usr/bin/env python3

minfo = { "TESTPYMOD1" :
  {
    "name"        : "Some test",
    "type"        : "python_module",
    "version"     : "0.1a",
    "description" : "Some test module",
    "authors"     : ["me", "myself", "I"],
    "refs"        : ["some paper", "some other paper"],
    "options"     : {
                        # Key               Default    Req     Check   Help
                        "double_opt_def":  (   1.0,    False,   None,  "Some help string"),
                        "double_opt":      ( float,    False,   None,  "Some help string"),
                        "double_req":      ( float,    False,   None,  "Some help string")
                    }
  }
}


