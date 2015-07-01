#!/usr/bin/env python3

minfo = { 
  "TEST_INT" :
  {
    "name"        : "Test int",
    "type"        : "python_module",
    "version"     : "0.1a",
    "description" : "Tests option parsing of integers",
    "authors"     : [],
    "refs"        : [],
    "options"     : {
                        # Key            Default     Req     Check  Help
                        "int_opt_def":  (     1,    False,   None,  "Some help string"),
                        "int_opt":      (   int,    False,   None,  "Some help string"),
                        "int_req":      (   int,    True,    None,  "Some help string"),
                    }
  },

  "TEST_FLOAT" :
  {
    "name"        : "Test float",
    "type"        : "python_module",
    "version"     : "0.1a",
    "description" : "Tests option parsing of floats/doubles",
    "authors"     : [],
    "refs"        : [],
    "options"     : {
                        "float_opt_def":  (   1.0,    False,   None,  "Some help string"),
                        "float_opt":      ( float,    False,   None,  "Some help string"),
                        "float_req":      ( float,    True,    None,  "Some help string"),
                    }
  },

  "TEST_BOOL" :
  {
    "name"        : "Test int",
    "type"        : "python_module",
    "version"     : "0.1a",
    "description" : "Tests option parsing of bools",
    "authors"     : [],
    "refs"        : [],
    "options"     : {
                        "bool_opt_def":  (   True,    False,   None,  "Some help string"),
                        "bool_opt":      (   bool,    False,   None,  "Some help string"),
                        "bool_req":      (   bool,    True,    None,  "Some help string"),
                    }
  },

  "TEST_STR" :
  {
    "name"        : "Test int",
    "type"        : "python_module",
    "version"     : "0.1a",
    "description" : "Tests option parsing of strings",
    "authors"     : [],
    "refs"        : [],
    "options"     : {
                        "str_opt_def":  (  "Hi",    False,   None,  "Some help string"),
                        "str_opt":      (   str,    False,   None,  "Some help string"),
                        "str_req":      (   str,    True,    None,  "Some help string"),
                    }
  },

  "TEST_VEC_INT" :
  {
    "name"        : "Test int",
    "type"        : "python_module",
    "version"     : "0.1a",
    "description" : "Tests option parsing of vector of integers",
    "authors"     : [],
    "refs"        : [],
    "options"     : {
                        "intvec_opt_def":  ( [ 1, 2, 3 ],   False, None, "Some help string"),
                        "intvec_req":      ( [int],         True,  None, "Some help string"),
                        "intvec_opt":      ( [int],         False, None, "Some help string"),
                    }
  },

  "TEST_VEC_FLOAT" :
  {
    "name"        : "Test int",
    "type"        : "python_module",
    "version"     : "0.1a",
    "description" : "Tests option parsing of vector of floats/doubles",
    "authors"     : [],
    "refs"        : [],
    "options"     : {
                        "floatvec_opt_def":  ( [ 1.0, 2.0, 3.0 ],  False, None, "Some help string"),
                        "floatvec_req":      ( [float],            True,  None, "Some help string"),
                        "floatvec_opt":      ( [float],            False, None, "Some help string"),
                    }
  },

  "TEST_VEC_BOOL" :
  {
    "name"        : "Test int",
    "type"        : "python_module",
    "version"     : "0.1a",
    "description" : "Tests option parsing of vector of bools",
    "authors"     : [],
    "refs"        : [],
    "options"     : {
                        "boolvec_opt_def":  ( [ True, False, True ],  False, None, "Some help string"),
                        "boolvec_req":      ( [bool],                 True,  None, "Some help string"),
                        "boolvec_opt":      ( [bool],                 False, None, "Some help string"),
                    }
  },

  "TEST_VEC_STR" :
  {
    "name"        : "Test int",
    "type"        : "python_module",
    "version"     : "0.1a",
    "description" : "Tests option parsing of vector of strings",
    "authors"     : [],
    "refs"        : [],
    "options"     : {
                        "strvec_opt_def":  ( [ "A", "Test", "List" ],  False, None, "Some help string"),
                        "strvec_req":      ( [str],                     True, None, "Some help string"),
                        "strvec_opt":      ( [str],                    False, None, "Some help string"),
                    }
  }


}
