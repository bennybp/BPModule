#ifndef CMODULELOADER_H
#define CMODULELOADER_H

#include <memory>
#include <unordered_map>
#include <string>

namespace bpmodule {


class ModuleBase;
class ModuleStore;
class OptionMap;
class ModuleInfo;

typedef ModuleBase *(*CreateFunc)(const std::string &, unsigned long, ModuleStore *, const OptionMap &);
typedef std::function<ModuleBase *(const std::string &, unsigned long, const OptionMap &)> ModuleGeneratorFunc;
typedef std::function<void(unsigned long)> ModuleDeleterFunc;


class CModuleLoader
{
public:

  CModuleLoader(ModuleStore * mst);
  ~CModuleLoader();

  CModuleLoader & operator=(const CModuleLoader & rhs) = delete;
  CModuleLoader(const CModuleLoader & rhs) = delete;

  bool LoadSO(const std::string & sopath, const std::string & key, const ModuleInfo & minfo);

private:
  ModuleStore * mst_;

  typedef std::unordered_map<std::string, void *> HandleMap; 
  typedef std::unordered_map<unsigned long, std::unique_ptr<ModuleBase>> ObjectMap;

  HandleMap handles_;
  ObjectMap objects_;

  void CloseAll_(void);

  ModuleBase * FuncWrapper_(CreateFunc fn, const std::string & key,
                                           unsigned long id,
                                           const OptionMap & options);
  void DeleteWrapper_(unsigned long id);
  void DeleteObject_(unsigned long id);
};

} // close namespace bpmodule


#endif
