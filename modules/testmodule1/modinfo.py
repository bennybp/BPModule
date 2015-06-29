#!/usr/bin/env python3

minfo = { "TESTMOD1" :
  {
    "name"        : "Some test",
    "type"        : "c_module",
    "soname"      : "testmodule1.so",
    "version"     : "0.1a",
    "description" : "Some test module",
    "authors"     : ["me", "myself", "I"],
    "refs"        : ["some paper", "some other paper"],
    "options"     : {
                        # Key               Default    Req     Check   Help
                        "int_opt_def":  (     1,    False,   None,  "Some help string"),
                        "int_opt":      (   int,    False,   None,  "Some help string"),
                        "int_req":      (   int,    True,    None,  "Some help string"),

                        "double_opt_def":  (   1.0,    False,   None,  "Some help string"),
                        "double_opt":      ( float,    False,   None,  "Some help string"),
                        "double_req":      ( float,    True,    None,  "Some help string"),

                        "str_opt_def":  (  "Hi",    False,   None,  "Some help string"),
                        "str_opt":      (   str,    False,   None,  "Some help string"),
                        "str_req":      (   str,    True,    None,  "Some help string"),

                        "bool_opt_def":  (   True,    False,   None,  "Some help string"),
                        "bool_opt":      (   bool,    False,   None,  "Some help string"),
                        "bool_req":      (   bool,    True,    None,  "Some help string"),


                        "intvec_opt_def":     ( [ 1, 2, 3 ],                    False, None, "Some help string"),
                        "doublevec_opt_def":  ( [ 1.0, 2.0, 3.0 ],              False, None, "Some help string"),
                        "strvec_opt_def":     ( [ "Hi", "There", "Handsome" ],  False, None, "Some help string"),
                        "boolvec_opt_def":    ( [ True, False, True ],          False, None, "Some help string"),

                        "intvec_req":     ( [int],    True, None, "Some help string"),
                        "doublevec_req":  ( [float],  True, None, "Some help string"),
                        "strvec_req":     ( [str],    True, None, "Some help string"),
                        "boolvec_req":    ( [bool],   True, None, "Some help string"),

                        "intvec_opt":     ( [int],    False, None, "Some help string"),
                        "doublevec_opt":  ( [float],  False, None, "Some help string"),
                        "strvec_opt":     ( [str],    False, None, "Some help string"),
                        "boolvec_opt":    ( [bool],   False, None, "Some help string"),
                    }
  }
}


