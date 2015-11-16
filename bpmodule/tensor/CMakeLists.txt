set(BPMODULE_TENSOR_FILES
    Tensor.cpp
    export.cpp
   )
  
set(BPMODULE_TENSOR_HEADERS
            Tensor.hpp
   )


ADD_MODULE(tensor
           "${BPMODULE_TENSOR_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(tensor "${BPMODULE_TENSOR_HEADERS}" "")
