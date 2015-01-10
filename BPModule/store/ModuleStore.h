#ifndef MODULESTORE_H
#define MODULESTORE_H

#include <memory>
#include <unordered_map>
#include <string>
#include <vector>

namespace bpmodule {


class ModuleBase;
typedef std::function<ModuleBase *(void)> ModuleGeneratorFunc;
typedef std::unique_ptr<ModuleBase> ModuleBaseUPtr;

typedef std::unordered_map<std::string,  ModuleGeneratorFunc> StoreType;
typedef StoreType::value_type StorePair;
typedef std::vector<StorePair> StorePairVec;

class ModuleStore
{
public:
  size_t Count(void) const;
  void Add(StorePair sp);
  void Add(std::string id, ModuleGeneratorFunc mgen);
  void Delete(const std::string & id);
  ModuleBaseUPtr Get(const std::string & id);
  void DumpInfo(void) const;

private:
  StoreType store_;
};

} // close namespace bpmodule


#endif
