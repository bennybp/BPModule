set(BPMODULE_TENSOR_FILES
    InitFinalize.cpp
    Tensor.cpp
    export.cpp
   )
  

ADD_COREMODULE(tensor
           "${BPMODULE_TENSOR_FILES}"
           ""
           "${LIBELEMENTAL_LIBRARIES}"
           "")

INSTALL_COREMODULE(tensor)
