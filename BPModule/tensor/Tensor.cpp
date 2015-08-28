#include "BPModule/tensor/Tensor.hpp"

#include <memory>

namespace {
  std::unique_ptr<CTF::World> world_;

}


void InitTensor(int argc, char ** argv)
{
    world_ = std::unique_ptr<CTF::World>(new CTF::World(argc, argv));
}

void FinalizeTensor(void)
{
    
}

CTF::World & GetWorld(void)
{
    return *world_;
}

