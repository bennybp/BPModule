#ifndef PARALLEL_OPENMP_H
#define PARALLEL_OPENMP_H

#include <string>
#include <memory>

#include "BPModule/modules/base/Parallel_Base.h"

#include <omp.h>

namespace bpmodule {

class Parallel_OpenMP : public Parallel_Base
{
public:
  Parallel_OpenMP(ModuleStore * mstore,
                  const std::string & filepath)
       : Parallel_Base(mstore, filepath,
                       ModuleType::LOCAL, 
                       "OpenMP computation module",
                       "Benjamin Pritchard",
                       "0.1a",
                       "Serialization via OpenMP parallel for loops")
  {
      nthreads_local = nthreads_;
  }

  virtual int Size(void) const { return 1; };
  virtual int Rank(void) const { return 0; };

  virtual bool Init(void)
  {
     nthreads_ = omp_get_max_threads();
     return true;
  }

  virtual void Finalize(void) { }

protected:
  virtual void ParallelFor_(Range range, ParallelFunc pf)
  {
    #pragma omp parallel for num_threads(nthreads_local)
    for(long i = range.first; i < range.second; i++)
      pf(i);
  }

private:
  static int nthreads_;
  int nthreads_local;
};

} // close namespace bpmodule

#endif
