#ifndef MODULESTORE_H
#define MODULESTORE_H

#include <memory>
#include <unordered_map>
#include <string>
#include <vector>
#include <atomic>
#include <utility>

#include "BPModule/core/ModuleInfo.h"

namespace bpmodule {


class ModuleBase;
class ModuleStore;

typedef std::function<ModuleBase *(long, ModuleStore *, const OptionMap &)> ModuleGeneratorFunc;


class ModuleStore
{
public:
  size_t Size(void) const;
  void Dump(void) const;

  template<typename T>
  T * GetModule(const std::string & key)
  {
      // \todo change away from at()?
      auto minfo = store_.at(key);

      ModuleBase * mbptr = (minfo.second(curid_, this, minfo.first.options)); // must remember to delete it
      idmap_[curid_] = key;
      curid_++;

      T * dptr = dynamic_cast<T *>(mbptr);
      if(dptr == nullptr)
      {
          delete mbptr;
          throw std::runtime_error("Unable to cast pointer");
      }
      //return std::unique_ptr<T>(dptr);
      return dptr;
  }

  ModuleStore();
  ~ModuleStore();

  ModuleStore & operator=(const ModuleStore & rhs) = delete;
  ModuleStore(const ModuleStore & rhs) = delete;

  bool LoadSO(const std::string & key, const std::string & sopath, ModuleInfo minfo);
  void Lock(void);
  ModuleInfo ModuleInfoFromID(long id) const;
  ModuleInfo ModuleInfoFromKey(const std::string & key) const;

private:
  typedef std::unordered_map<std::string, std::pair<ModuleInfo, ModuleGeneratorFunc>> StoreType;
  typedef std::unordered_map<std::string, void *> HandleMap; 
  typedef std::unordered_map<long, std::string> IDMap; 

  std::atomic<long> curid_;
  StoreType store_;
  HandleMap handles_;
  IDMap idmap_;
  bool locked_;

  void CloseAll(void);
};

} // close namespace bpmodule


#endif
