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
  bool Merge(const std::string & filepath, const StoreType & sp);
  ModuleBaseUPtr Get(const std::string & id);
  void DumpInfo(void) const;

private:
  // The store stores a std::bind version of the generator that automatically binds the filename
  // as the only argument
  typedef std::function<ModuleBase *()> StoredModuleGeneratorFunc;
  typedef std::unordered_map<std::string,  StoredModuleGeneratorFunc> InternalStoreType;

  InternalStoreType store_;
};

} // close namespace bpmodule


#endif
