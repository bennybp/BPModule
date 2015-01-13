#!/usr/bin/env python3

compinfo = { "libtestmodule1.so" :
              {
                "name"        : "TESTMOD1",
                "version"     : "0.1a",
                "description" : "Some test module",
                "authors"     : ["me", "myself", "I"],
                "refs"        : ["some paper", "some other paper"],
                "class"       : "TEST",
                "type"        : "TEST",
              }
            }


options = [ 
            ("adouble", "double", "0.0"),
            ("aint", "int", "1")
          ]
