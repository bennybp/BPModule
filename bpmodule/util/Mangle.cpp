/*! \file
 *
 * \brief Mangling/Demangling helpers (source) 
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#include <cxxabi.h>
#include "bpmodule/util/Mangle.hpp"

namespace bpmodule {
namespace util {


std::string DemangleCpp(const char * typestr)
{
    // taken from https://gcc.gnu.org/onlinedocs/libstdc++/manual/ext_demangling.html

    int status;
    char * realname;

    // by default, return typestr, unless the
    // demangling fails. Should it ever?
    std::string ret(typestr);

    realname = abi::__cxa_demangle(typestr, NULL, NULL, &status);

    if(status == 0)
        ret = std::string(realname);

    free(realname);

    return ret;   
}


} // close namespace util
} // close namespace bpmodule
