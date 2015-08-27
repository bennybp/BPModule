##################################################
# Actual library
##################################################

set(BPMODULE_PYTHON_FILES
            Core_python.cpp
            Output_python.cpp
            Exception_python.cpp
   )


ADD_CORELIB(bpmodule_python "${BPMODULE_PYTHON_FILES}"
                            ""
                            "${Boost_INCLUDE_DIRS};${PYTHON_INCLUDE_DIRS}")

