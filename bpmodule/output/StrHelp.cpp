/*! \file
 *
 * \brief Various string helper functions (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include "bpmodule/output/FormatStr.hpp"


/*! \brief Joins each element of a vector of strings
 *
 * \param [in] vec A vector of strings to join
 * \param [in] j   What to put in between each string
 * \return A string with each element of \p vec joined with \p j in between
 */
std::string Join(const std::vector<std::string> & vec, const std::string & j)
{
    if(vec.size() == 0)
        return std::string();

    std::string str = vec[0];

    for(size_t i = 1; i < vec.size(); ++i)
    {
        str.append(j);
        str.append(vec[i]);
    }

    return str;

}

