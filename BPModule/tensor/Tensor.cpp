#include "BPModule/tensor/Tensor.hpp"

#include <memory>

namespace {
  std::unique_ptr<CTF::World> world_;

}



namespace bpmodule {
namespace tensor {


void InitTensor(int argc, char ** argv)
{
    world_ = std::unique_ptr<CTF::World>(new CTF::World(argc, argv));
}

void FinalizeTensor(void)
{
   world_.reset(); 
}

CTF::World & GetWorld(void)
{
    return *world_;
}


} // close namespace tensor
} // close namespace bpmodule
