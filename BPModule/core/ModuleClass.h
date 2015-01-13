#ifndef MODULECLASS_H
#define MODULECLASS_H

namespace bpmodule {

enum class ModuleClass
{
   TEST,
   MATH
};


enum class ModuleType
{
   // MODULE_TEST
   TEST,

   //Misc
   GENERAL
};

const char * MClassToString(ModuleClass mc);
const char * MTypeToString(ModuleType mt);
ModuleClass StringToMClass(const char * str);
ModuleType StringToMType(const char * str);

} // close namespace bpmodule

#endif
