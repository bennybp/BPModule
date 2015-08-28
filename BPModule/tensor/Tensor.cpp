#include "BPModule/tensor/Tensor.hpp"

namespace {
  CTF::World world_;

}


void InitTensor(int argc, char ** argv)
{
    world_ = CTF::World(argc, argv);
}

void FinalizeTensor(void)
{
    
}

CTF::World & GetWorld(void)
{
    return world_;
}

