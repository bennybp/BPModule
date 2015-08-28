##################################################
# Actual library
##################################################

set(PYTHON_FILES
   )


ADD_CORELIB(bpmodule_python_helper "${mYTHON_FILES}"
                                   ""
                                   "${Boost_INCLUDE_DIRS};${PYTHON_INCLUDE_DIRS}")

