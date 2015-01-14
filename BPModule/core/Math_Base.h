#ifndef MATH_BASE_H
#define MATH_BASE_H

#include "BPModule/core/ModuleBase.h"

using namespace bpmodule;

namespace bpmodule {

class Math_Base : public ModuleBase
{
public:
  Math_Base(ModuleStore * mstore,
            const std::string & filepath,
            ModuleType mtype,
            const std::string & name, const std::string & authors, 
            const std::string & version, const std::string & description);

  virtual double pow(double, double) = 0;
  virtual double pow(long, long) = 0;
};

}

#endif
