#include "bpmodule/tensor/Tensor.hpp"
#include "bpmodule/tensor/Matrix.hpp"
#include "bpmodule/util/Cmdline.hpp"


namespace bpmodule {
namespace tensor {


void Init(void)
{
    ///////////////////////////
    // Initialize matrix
    ///////////////////////////
    El::Initialize(*(util::GetArgc()), *(util::GetArgv())); 
}

void Finalize(void)
{
    ///////////////////////////
    // Finalize matrix
    ///////////////////////////
    El::Finalize();
}


} // close namespace tensor
} // close namespace bpmodule
