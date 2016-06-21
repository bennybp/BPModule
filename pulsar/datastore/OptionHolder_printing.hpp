/*! \file
 *
 * \brief helpers for printing options
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#include <string>
#include <vector>
#include <map>
#include <set>
#include "pulsar/util/Format.hpp"


namespace pulsar{
namespace datastore {
namespace detail {


//////////////////
// printing helpers
//////////////////
/*! \brief Converts an option value to a string
 */
template<typename T>
std::vector<std::string> OptToString(const T & opt)
{
    if(std::is_integral<T>::value)
        return {util::format_string("%-12?", opt)};
    else
        return {util::format_string("%-12.8?", opt)};
}

/*! \brief Converts an option value to a string
 */
inline std::vector<std::string> OptToString(const std::string & opt)
{
    return {opt};
}


/*! \brief Converts an option value to a string
 *
 * Overload for a bool type
 */
inline std::vector<std::string> OptToString(const bool & opt)
{
    return {(opt ? "True" : "False")};
}



template<typename T>
std::vector<std::string> OptToString(const std::vector<T> & opt)
{
    if(opt.size() == 0)
        return {"(empty)"};

    std::vector<std::string> r;
    for(const auto & it : opt)
    {
        std::vector<std::string> tmp = OptToString(it);
        r.insert(r.end(), tmp.begin(), tmp.end());
    }
    return r;
}


template<typename T>
std::vector<std::string> OptToString(const std::set<T> & opt)
{
    if(opt.size() == 0)
        return {"(empty)"};

    std::vector<std::string> r;
    for(const auto & it : opt)
    {
        std::vector<std::string> tmp = OptToString(it);
        r.insert(r.end(), tmp.begin(), tmp.end());
    }
    return r;
}


template<typename T, typename U>
std::vector<std::string> OptToString(const std::map<T, U> & opt)
{
    //! \todo write this (and probably clean up the rest)
    if(opt.size() == 0)
        return {"(empty)"};
    else
        return {"(map)"};
}


} // close namespace detail
} // close namespace datastore
} // close namespace pulsar

