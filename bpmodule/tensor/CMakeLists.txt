set(BPMODULE_TENSOR_FILES
    InitFinalize.cpp
    Tensor.cpp
    export.cpp
   )
  

ADD_COREMODULE(tensor
           "${BPMODULE_TENSOR_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(tensor)
