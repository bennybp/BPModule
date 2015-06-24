#ifndef PYMODULELOADER_H
#define PYMODULELOADER_H

#include <memory>
#include <unordered_map>
#include <string>

#include <boost/python.hpp>

namespace bpmodule {


class ModuleBase;
class ModuleStore;
class OptionMap;
class ModuleInfo;

typedef std::function<ModuleBase *(const std::string &, unsigned long, const OptionMap &)> ModuleGeneratorFunc;
typedef std::function<void(unsigned long)> ModuleDeleterFunc;


class PyModuleLoader
{
public:

  PyModuleLoader(ModuleStore * mst);
  ~PyModuleLoader();

  PyModuleLoader & operator=(const PyModuleLoader & rhs) = delete;
  PyModuleLoader(const PyModuleLoader & rhs) = delete;

  bool AddPyModule(const std::string & key,
                   boost::python::object func, const ModuleInfo & minfo);

  void DeleteAll(void);

private:
  ModuleStore * mst_;

  typedef std::unordered_map<unsigned long, boost::python::object> ObjectMap;

  ObjectMap objects_;


  ModuleBase * FuncWrapper_(boost::python::object fn, const std::string & key,
                            unsigned long id,
                            const OptionMap & options);

  void DeleteWrapper_(unsigned long id);
  void DeleteObject_(unsigned long id);
};

} // close namespace bpmodule


#endif
