#ifndef MODULESTORE_H
#define MODULESTORE_H

#include <memory>
#include <unordered_map>
#include <string>
#include <vector>

#include "BPModule/core/OptionMap.h"

namespace bpmodule {


class ModuleBase;
class ModuleStore;
enum class ModuleClass;
enum class ModuleType;

typedef std::function<ModuleBase *(ModuleStore *, const OptionMap &)> ModuleGeneratorFunc;

struct ModuleInfo
{
  std::string name;
  std::string soname;
  std::string version;
  std::vector<std::string> authors;
  std::string description;
  std::vector<std::string> refs;
  OptionMap options;

  void * handle;
  ModuleGeneratorFunc createfunc;
};

typedef std::unique_ptr<ModuleBase> ModuleBaseUPtr;


class ModuleStore
{
public:
  size_t Size(void) const;
  void Dump(void) const;

  template<typename T>
  std::unique_ptr<T> GetModule(const std::string & id)
  {
      // \todo change away from at()?
      ModuleInfo minfo = store_.at(id);

      ModuleBase * mbptr = (minfo.createfunc(this, minfo.options)); // must remember to delete it

      T * dptr = dynamic_cast<T *>(mbptr);
      if(dptr == nullptr)
      {
          delete mbptr;
          throw std::runtime_error("Unable to cast pointer");
      }
      return std::unique_ptr<T>(dptr);
  }

  ModuleStore();
  ~ModuleStore();

  ModuleStore & operator=(const ModuleStore & rhs) = delete;
  ModuleStore(const ModuleStore & rhs) = delete;

  bool LoadSO(const std::string & key, const std::string & sopath, ModuleInfo minfo);
  void Lock(void);

private:
  typedef std::unordered_map<std::string, ModuleInfo> StoreType;
  typedef std::unordered_map<std::string, void *> HandleMap; 

  StoreType store_;
  HandleMap handles_;
  bool locked_;

  void CloseAll(void);
};

} // close namespace bpmodule


#endif
