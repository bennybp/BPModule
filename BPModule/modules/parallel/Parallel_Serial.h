#ifndef PARALLEL_SERIAL_H
#define PARALLEL_SERIAL_H

#include <string>
#include <memory>

#include "BPModule/modules/base/Parallel_Base.h"

namespace bpmodule {

class Parallel_Serial : public Parallel_Base
{
public:
  Parallel_Serial(ModuleStore * mstore,
                  const std::string & filepath)
       : Parallel_Base(mstore, filepath,
                       ModuleType::SERIAL, 
                       "Serial computation module",
                       "Benjamin Pritchard",
                       "0.1a",
                       "No parallelization")
  {
  }

  virtual int Size(void) const { return 1; };
  virtual int Rank(void) const { return 0; };
  virtual int Threads(void) const { return 1; };

  virtual bool Init(void) { return true; }
  virtual void Finalize(void) { }

protected:
  virtual void ParallelFor_(Range range, ParallelFunc pf, int rank)
  {
    for(long i = range.first; i < range.second; i++)
        pf(i, rank, 0);
  }

};

} // close namespace bpmodule

#endif
