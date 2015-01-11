#ifndef MODULEBASE_H
#define MODULEBASE_H

#include <string>
#include <memory>
#include <functional>

#include "BPModule/core/ModuleClass.h"

namespace bpmodule {

class ModuleStore;

class ModuleBase
{
public:
  ModuleBase(ModuleStore * mstore,
             const std::string & filepath,
             ModuleClass mclass, ModuleType mtype,
             const std::string & name, const std::string & authors, 
             const std::string & version, const std::string & description);


  ModuleClass MClass(void) const { return mclass_; }
  ModuleType MType(void) const { return mtype_; }

  std::string MClassStr(void) const { return MClassToString(mclass_); }
  std::string MTypeStr(void) const { return MTypeToString(mtype_); }

  const std::string & Name(void) const { return name_; }
  const std::string & Authors(void) const { return authors_; }
  const std::string & Version(void) const { return version_; }
  const std::string & Description(void) const { return description_; }
  const std::string & Filepath(void) const { return filepath_; }

  virtual ~ModuleBase() { }

protected:
  ModuleStore & mstore_;

private:
  std::string filepath_;

  ModuleClass mclass_;
  ModuleType mtype_;

  std::string name_;
  std::string authors_;
  std::string version_;
  std::string description_;
};

typedef std::unique_ptr<ModuleBase> ModuleBaseUPtr;


} // close namespace bpmodule

#endif
