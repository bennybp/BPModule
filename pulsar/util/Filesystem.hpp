/*! \file
 *
 * \brief Handling files and filesystems (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef BPMODULE_GUARD_UTIL__FILESYSTEM_HPP_
#define BPMODULE_GUARD_UTIL__FILESYSTEM_HPP_

#include <utility>
#include <string>


namespace bpmodule {
namespace util {


/*! \brief Adds a trailing slash to a path if it doesn't have one
 */
std::string AddTrailingSlash(const std::string & path);


/*! \brief Removes all trailing slashes from a path if it has them
 */
std::string RemoveTrailingSlash(const std::string & path);


/*! \brief Split the last component off of a path
 *
 * For example, /home/someone/myfile.txt -> (/home/someone, myfile.txt)
 * and /home/someone/somedir -> (/home/someone, somedir)
 *
 * The paths returned won't have trailing slashes
 */
std::pair<std::string, std::string> SplitPath(const std::string & path);


} // close namespace util
} // close namespace bpmodule


#endif
