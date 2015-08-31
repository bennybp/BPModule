set(BPMODULE_TENSOR_FILES
    Tensor.cpp
    export.cpp
   )
  


ADD_MODULE(tensor
           "${BPMODULE_TENSOR_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(tensor "")
