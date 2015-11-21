set(BPMODULE_TENSOR_FILES
    InitFinalize.cpp
    Tensor.cpp
    export.cpp
   )
  
set(BPMODULE_TENSOR_HEADERS
            InitFinalize.hpp
            Matrix.hpp
            Tensor.hpp
   )


ADD_MODULE(tensor
           "${BPMODULE_TENSOR_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(tensor "${BPMODULE_TENSOR_HEADERS}" "")
