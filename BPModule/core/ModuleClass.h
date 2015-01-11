#ifndef MODULECLASS_H
#define MODULECLASS_H

#include <string>

namespace bpmodule {

enum class ModuleClass
{
   TEST,
   PARALLEL,
};


enum class ModuleType
{
   // MODULE_TEST
   TEST,

   // MODULE_PARALLEL
   SERIAL,
   LOCAL,
   DISTRIBUTED
};

const char * MClassToString(ModuleClass mc);
const char * MTypeToString(ModuleType mt);

} // close namespace bpmodule

#endif
