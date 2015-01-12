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
  ModuleClass mclass;
  ModuleType mtype;
  std::string authors;
  std::string version;
  std::string description;
  std::string refs;
  ModuleGeneratorFunc genfunc;
  OptionMap options;
};

typedef std::unordered_map<std::string,  ModuleInfo> StoreType;
typedef std::unique_ptr<ModuleBase> ModuleBaseUPtr;


class ModuleStore
{
public:
  size_t Count(void) const;
  void DumpInfo(void) const;

  template<typename T>
  std::unique_ptr<T> GetModule(const std::string & id)
  {
      // \todo change away from at()?
      ModuleInfo minfo = store_.at(id);

      ModuleBase * mbptr = (minfo.genfunc(this, minfo.options)); // must remember to delete it

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

  bool LoadSO(const char * modulepath, const char * components);
  void Lock(void);

private:
  typedef std::unordered_map<std::string, void *> HandleMap; 
  typedef std::unordered_map<std::string, std::string> SOFileMap; 

  StoreType store_;
  HandleMap handles_;
  SOFileMap sofiles_;
  bool locked_;

  bool SelectComponents(const std::string & components, StoreType & st);
  bool Merge(const std::string & filepath, void * handle, const StoreType & sp);
  void CloseAll(void);
};

} // close namespace bpmodule


#endif
