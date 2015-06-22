#ifndef MODULESTORE_H
#define MODULESTORE_H

#include <memory>
#include <unordered_map>
#include <string>
#include <atomic>
#include <utility>

#include "BPModule/core/ModuleInfo.h"

namespace bpmodule {


class ModuleBase;
class ModuleStore;

typedef std::function<ModuleBase *(unsigned long, ModuleStore *, const OptionMap &)> ModuleGeneratorFunc;


class ModuleStore
{
public:


  ModuleStore();
  ~ModuleStore();

  ModuleStore & operator=(const ModuleStore & rhs) = delete;
  ModuleStore(const ModuleStore & rhs) = delete;

  bool LoadSO(const std::string & key, const std::string & sopath, ModuleInfo minfo);
  void Lock(void);

  std::vector<std::string> GetKeys(void) const;
  ModuleInfo ModuleInfoFromKey(const std::string & key) const;

  size_t Size(void) const;

  bool Has(const std::string & key) const;

  template<typename T>
  T * GetModule(const std::string & key)
  {
      const StoreEntry & se = GetOrThrow(key);

      ModuleBase * mbptr = se.func(curid_, this, se.mi.options); // must remember to delete it
      curid_++;

      T * dptr = dynamic_cast<T *>(mbptr);
      if(dptr == nullptr)
      {
          delete mbptr;
            throw MapException("ModuleStore", key, typeid(mbptr).name(), typeid(T *).name());
      }
      return dptr;
  }

private:

  struct StoreEntry
  {
    ModuleInfo mi;
    ModuleGeneratorFunc func;
  };

  typedef std::unordered_map<std::string, StoreEntry> StoreMap;
  typedef StoreMap::value_type StoreMapValue;

  typedef std::unordered_map<std::string, void *> HandleMap; 
  typedef HandleMap::value_type HandleMapValue;


  std::atomic<unsigned long> curid_;
  StoreMap store_;
  HandleMap handles_;
  bool locked_;

  const StoreEntry & GetOrThrow(const std::string & key) const;

  void CloseAll(void);
};

} // close namespace bpmodule


#endif
