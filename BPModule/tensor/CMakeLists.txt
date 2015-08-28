##################################################
# Actual library
##################################################

set(TENSOR_FILES
   )
  


ADD_CORELIB(bpmodule_tensor "${TENSOR_FILES}"
                            ""
                            "${Boost_INCLUDE_DIRS};${PYTHON_INCLUDE_DIRS}")

