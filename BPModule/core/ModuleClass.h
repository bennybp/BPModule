#ifndef MODULECLASS_H
#define MODULECLASS_H

#include <string>

namespace bpmodule {

enum class ModuleClass
{
   MCLASS_TEST,
   MCLASS_1E_INTEGRAL
};


enum class ModuleType
{
   // MODULE_TEST
   MTYPE_TEST,

   // MODULE_1E_INTEGRAL
   MTYPE_STD
};

const char * MClassToString(ModuleClass mc);
const char * MTypeToString(ModuleType mt);

} // close namespace bpmodule

#endif
