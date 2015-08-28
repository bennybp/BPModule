##################################################
# Actual library
##################################################

set(BPMODULE_PYTHON_HELPER_FILES
   )


ADD_CORELIB(bpmodule_python_helper "${BPMODULE_PYTHON_FILES}"
                                   ""
                                   "${Boost_INCLUDE_DIRS};${PYTHON_INCLUDE_DIRS}")

