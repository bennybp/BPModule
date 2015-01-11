#ifndef MODULESTORE_H
#define MODULESTORE_H

#include <memory>
#include <unordered_map>
#include <string>
#include <vector>

namespace bpmodule {


class ModuleBase;
typedef std::function<ModuleBase *(const std::string &)> ModuleGeneratorFunc;
typedef std::unordered_map<std::string,  ModuleGeneratorFunc> StoreType;
typedef std::unique_ptr<ModuleBase> ModuleBaseUPtr;


class ModuleStore
{
public:
  size_t Count(void) const;
  ModuleBaseUPtr Get(const std::string & id);
  void DumpInfo(void) const;

  void CloseAll(void);
  bool LoadModule(const char * modulepath, const char * components);

  ~ModuleStore();

private:
  // The store stores a std::bind version of the generator that automatically binds the filename
  // as the only argument
  typedef std::function<ModuleBase *()> StoredModuleGeneratorFunc;
  typedef std::unordered_map<std::string, StoredModuleGeneratorFunc> InternalStoreType;

  typedef std::unordered_map<std::string, void *> HandleMap; 
  typedef HandleMap::value_type HandleMapPair;

  InternalStoreType store_;
  HandleMap handles_;

  bool Merge(const std::string & filepath, void * handle, const StoreType & sp);
};

} // close namespace bpmodule


#endif
