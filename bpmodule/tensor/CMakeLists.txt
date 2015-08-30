set(BPMODULE_TENSOR_FILES
    Tensor.cpp
    export.cpp
    ${BPMODULE_CYCLOPS_PATH}/lib/libctf.a
   )
  


ADD_MODULE(tensor
           "${BPMODULE_TENSOR_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(tensor "")
