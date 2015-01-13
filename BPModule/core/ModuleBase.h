#ifndef MODULEBASE_H
#define MODULEBASE_H

#include <string>
#include <memory>
#include <functional>

#include "BPModule/core/OptionMap.h"

namespace bpmodule {

class ModuleStore;


class ModuleBase
{
public:
  ModuleBase(ModuleStore * mstore, const OptionMap & options);

  virtual ~ModuleBase() { }


  template<typename T>
  const T & GetTrait(const std::string & key) const
  {
    return traits_.Get<T>(key);
  }

  bool HasTrait(const std::string & key) const
  {
    return traits_.Has(key);
  }

  template<typename T>
  const T & GetOption(const std::string & key) const
  {
    return options_.Get<T>(key);
  }

  bool HasOption(const std::string & key) const
  {
    return options_.Has(key);
  }

  template<typename T>
  void SetOption(const std::string & key, const T & value)
  {
    options_.Set(key, value);
  }

protected:
  const ModuleStore & mstore_;

  template<typename T>
  void SetTrait(const std::string & key, const T & value)
  {
    traits_.Set(key, value);
  }

private:
  OptionMap traits_;
  OptionMap options_;
};

typedef std::unique_ptr<ModuleBase> ModuleBaseUPtr;


} // close namespace bpmodule

#endif
