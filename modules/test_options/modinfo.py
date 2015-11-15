minfo = { 
  "test_int" :
  {
    "type"        : "python_module",
    "version"     : "0.1a",
    "description" : "Tests option parsing of integers",
    "authors"     : [],
    "refs"        : [],
    "options"     : {
                        # Key                 Type    Default     Req     Check   Help
                        "int_opt_def":     (  "int",   5,         False,   None,  "Some help string"),
                        "int_req":         (  "int",   None,      True,    None,  "Some help string"),
                        "int_opt":         (  "int",   None,      False,   None,  "Some help string"),
                    }
  },

  "test_float" :
  {
    "type"        : "python_module",
    "version"     : "0.1a",
    "description" : "Tests option parsing of floats/doubles",
    "authors"     : [],
    "refs"        : [],
    "options"     : {
                        # Key                   Type    Default     Req     Check   Help
                        "float_opt_def":    (  "float",  5.0,       False,   None,  "Some help string"),
                        "float_req":        (  "float",  None,      True,    None,  "Some help string"),
                        "float_opt":        (  "float",  None,      False,   None,  "Some help string"),
                    }
  },

  "test_bool" :
  {
    "type"        : "python_module",
    "version"     : "0.1a",
    "description" : "Tests option parsing of bools",
    "authors"     : [],
    "refs"        : [],
    "options"     : {
                        # Key                 Type    Default     Req     Check   Help
                        "bool_opt_def":    (  "bool",  False,     False,   None,  "Some help string"),
                        "bool_req":        (  "bool",  None,      True,    None,  "Some help string"),
                        "bool_opt":        (  "bool",  None,      False,   None,  "Some help string"),
                    }
  },

  "test_str" :
  {
    "type"        : "python_module",
    "version"     : "0.1a",
    "description" : "Tests option parsing of strings",
    "authors"     : [],
    "refs"        : [],
    "options"     : {
                        # Key                 Type    Default     Req     Check   Help
                        "str_opt_def":     (  "str",   "Hello",   False,   None,  "Some help string"),
                        "str_req":         (  "str",   None,      True,    None,  "Some help string"),
                        "str_opt":         (  "str",   None,      False,   None,  "Some help string"),
                    }
  },

  "test_listint" :
  {
    "type"        : "python_module",
    "version"     : "0.1a",
    "description" : "Tests option parsing of list of integers",
    "authors"     : [],
    "refs"        : [],
    "options"     : {
                        # Key                 Type         Default         Req     Check   Help
                        "listint_opt_def":  (  "listint",   [ 1, 2, 3 ],   False,   None,  "Some help string"),
                        "listint_req":      (  "listint",   None,          True,    None,  "Some help string"),
                        "listint_opt":      (  "listint",   None,          False,   None,  "Some help string"),
                    }
  },

  "test_listfloat" :
  {
    "type"        : "python_module",
    "version"     : "0.1a",
    "description" : "Tests option parsing of list of floats/doubles",
    "authors"     : [],
    "refs"        : [],
    "options"     : {
                        # Key                   Type         Default                Req      Check   Help
                        "listfloat_opt_def":  (  "listfloat",   [ 1.0, 2.0, 3.0 ],   False,   None,  "Some help string"),
                        "listfloat_req":      (  "listfloat",   None,                True,    None,  "Some help string"),
                        "listfloat_opt":      (  "listfloat",   None,                False,   None,  "Some help string"),
                    }
  },

  "test_listbool" :
  {
    "type"        : "python_module",
    "version"     : "0.1a",
    "description" : "Tests option parsing of list of bools",
    "authors"     : [],
    "refs"        : [],
    "options"     : {
                        # Key                 Type         Default                Req    Check   Help
                        "listbool_opt_def":  ( "listbool", [ True, False, True ],  False, None, "Some help string"),
                        "listbool_req":      ( "listbool",   None,                 True,  None, "Some help string"),
                        "listbool_opt":      ( "listbool",   None,                 False, None, "Some help string"),
                    }
  },

  "test_liststr" :
  {
    "type"        : "python_module",
    "version"     : "0.1a",
    "description" : "Tests option parsing of list of strings",
    "authors"     : [],
    "refs"        : [],
    "options"     : {
                        # Key                 Type         Default                   Req     Check   Help
                        "liststr_opt_def":  ( "liststr",    [ "A", "Test", "List" ],  False, None, "Some help string"),
                        "liststr_req":      ( "liststr",    None,                     True,  None, "Some help string"),
                        "liststr_opt":      ( "liststr",    None,                     False, None, "Some help string"),
                    }
  }


}
