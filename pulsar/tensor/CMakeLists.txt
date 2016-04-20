set(PULSAR_TENSOR_FILES
    InitFinalize.cpp
    Tensor.cpp
    export.cpp
   )
  

ADD_COREMODULE(tensor
           "${PULSAR_TENSOR_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(tensor)
