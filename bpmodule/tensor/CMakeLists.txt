set(BPMODULE_TENSOR_FILES
    InitFinalize.cpp
    Tensor.cpp
    export.cpp
   )
  

ADD_SUPERMODULE(tensor
           "${BPMODULE_TENSOR_FILES}"
           ""
           "${LIBELEMENTAL_LIBRARIES}"
           "${LIBELEMENTAL_INCLUDE_DIR}")

INSTALL_COREMODULE(tensor)
