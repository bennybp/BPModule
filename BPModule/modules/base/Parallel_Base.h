#ifndef PARALLEL_BASE_H
#define PARALLEL_BASE_H

#include <functional>

#include "BPModule/core/ModuleBase.h"

namespace bpmodule {

typedef std::function<void(long, int, int)> ParallelFunc;

class Parallel_Base : public ModuleBase
{
public:
  Parallel_Base(ModuleStore * mstore,
                const std::string & filepath,
                ModuleType mtype,
                const std::string & name, const std::string & authors, 
                const std::string & version, const std::string & description);

  virtual int Size(void) const = 0;
  virtual int Rank(void) const = 0;
  virtual int Threads(void) const = 0;
  virtual bool Init(void) = 0;
  virtual void Finalize(void) = 0;

  void ParallelFor(long totalsize, ParallelFunc pf);

protected:
  typedef std::pair<long, long> Range;

  Range GetRange_(long totalsize) const;

  virtual void ParallelFor_(Range range, ParallelFunc pf, int rank) = 0;
};

} // close namespace bpmodule

#endif
