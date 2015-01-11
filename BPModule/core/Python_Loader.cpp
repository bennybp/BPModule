#include <iostream>
#include <dlfcn.h>

extern "C" {

bool PythonLoader(const char * coreso)
{
    //! \todo Wrap handle obeject in a smart-ptr type thing
    //        Also store that in the handle map
    char * error;

    // open the module
    std::cout << "Looking to open core so file: " << coreso << "\n";
    void * handle = dlopen(coreso, RTLD_NOW | RTLD_GLOBAL);
    if(!handle)
    {
        error = dlerror();
        std::cout << "Error - unable to open core SO file: " << coreso << "\n";
        std::cout << error << "\n";
        return false;
    }
    else
        std::cout << "Successfully opened " << coreso << "\n";
    return true;
}


} // close extern C
