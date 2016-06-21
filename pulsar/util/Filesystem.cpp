/*! \file
 *
 * \brief Handling files and filesystems (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include "pulsar/util/Filesystem.hpp"

namespace pulsar{
namespace util {


std::string add_trailing_slash(const std::string & path)
{
    /*! \todo - This is woefully incomplete at the moment. It only
     *          works with linux and doesn't handle some edge cases
     *          (such as escaped slashes)
     */

    if(path == "")
        return "/"; //?

    if(path.back() == '/')
        return path;
    else
        return path + "/";
}

std::string remove_trailing_slash(const std::string & path)
{
    /*! \todo - This is woefully incomplete at the moment. It only
     *          works with linux and doesn't handle some edge cases
     *          (such as escaped slashes)
     */

    std::string tmp(path);
    while(tmp.size() && tmp.back() == '/')
        tmp.pop_back();
    return tmp;
}


std::pair<std::string, std::string> split_path(const std::string & path)
{
    /*! \todo - This is woefully incomplete at the moment. It only
     *          works with linux and doesn't handle some edge cases
     *          (such as escaped slashes)
     */

    // 
    std::string tmp = remove_trailing_slash(path);

    size_t slashpos = tmp.find_last_of("/");

    if(slashpos == tmp.npos)
        return {tmp, ""};
    else
        return {tmp.substr(0, slashpos), path.substr(slashpos+1, tmp.npos)};
    
}


} // close namespace util
} // close namespace pulsar
