#include <iostream>


#include "BPModule/core/ModuleStore.h"


extern "C" {

bool LoadSO(const char * modulepath, const char * components);
void CloseAll(void);
int Count(void);
void DumpInfo(void);
void RunTest(const char * modname);
void InitParallel(void);
void FinalizeParallel(void);

} // close extern C
