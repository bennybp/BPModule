#ifndef MODULEBASE_H
#define MODULEBASE_H

#include <string>
#include <functional>

#include "BPModule/core/ModuleInfo.h" // includes optionmap

namespace bpmodule {

class ModuleStore;


class ModuleBase
{
public:
  ModuleBase(unsigned long id, ModuleStore * mstore, const OptionMap & options);

  virtual ~ModuleBase() { }

  ModuleBase(const ModuleBase & rhs) = delete;
  ModuleBase(ModuleBase && rhs) = delete;
  ModuleBase & operator= (const ModuleBase & rhs) = delete;
  ModuleBase & operator= (ModuleBase && rhs) = delete;

  OptionMap Traits(void) const;
  OptionMap Options(void) const;

  std::string Key(void) const;
  ModuleInfo Info(void) const;

protected:
  ModuleStore & mstore_;

  template<typename T>
  void SetTrait(const std::string & key, const T & value)
  {
    traits_.Set(key, value);
  }

  template<typename T>
  void SetOption(const std::string & key, const T & value)
  {
    options_.Set(key, value);
  }

private:
  unsigned long id_;
  OptionMap traits_;
  OptionMap options_;
};


} // close namespace bpmodule

#endif
