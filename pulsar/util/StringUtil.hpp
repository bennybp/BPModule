/*! \file
 *
 * \brief Various string helper functions (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef BPMODULE_GUARD_UTIL__STRINGUTIL_HPP_
#define BPMODULE_GUARD_UTIL__STRINGUTIL_HPP_

#include <string>


namespace bpmodule {
namespace util {

/*! \brief Joins each element of a container of strings
 *
 * \tparam A container type (vector, set) of strings
 * \param [in] container A container of strings to join
 * \param [in] j         What to put in between each string
 * \return A string with each element of \p container joined with \p j in between
 */
template<typename T>
std::string Join(const T & container, const std::string & j)
{
    if(container.size() == 0)
        return std::string();


    auto it = container.begin();
    std::string str = *it;
    std::advance(it, 1);

    for(; it != container.end(); ++it)
    {
        str.append(j);
        str.append(*it);
    }

    return str;
}


/*! \brief Transform a string to lower case
 *
 * \param [inout] str String to convert to lowercase
 */
void ToLower(std::string & str);


/*! \brief Transform a string to lower case, copying to a new string
 *
 * \param [in] str String to convert to lowercase
 * \return The string converted to lowercase
 */
std::string ToLower_Copy(std::string str);


/*! \brief Trim a string (beginning)
 * 
 * \param [in] s String to trim
 */
void LeftTrim(std::string & s);


/*! \brief Trim a string (ending)
 * 
 * \param [in] s String to trim
 */
void RightTrim(std::string & s);


/*! \brief Trim a string (beginning and ending)
 * 
 * \param [in] s String to trim
 */
void Trim(std::string & s);


/*! \brief Trim a string (beginning)
 * 
 * \param [in] s String to trim
 * \return Copy of s with leading whitespace removed
 */
std::string LeftTrim_Copy(std::string s);


/*! \brief Trim a string _Copy(ending)
 * 
 * \param [in] s String to trim
 * \return Copy of s with trailing whitespace removed
 */
std::string RightTrim_Copy(std::string s);


/*! \brief Trim a string _Copy(beginning and ending)
 * 
 * \param [in] s String to trim
 * \return Copy of s with leading and trailing whitespace removed
 */
std::string Trim_Copy(std::string s);



/*! \brief Less-than comparison of a case-insensitive string
 *
 * Useful for containers (maps) where the key is
 * case insensitive
 */
struct CaseInsensitiveLess
{
    bool operator()(std::string lhs, std::string rhs) const;
}; 


/*! \brief Less-than comparison of a case-insensitive string, trimming leading and trailing whitespace
 *
 * Useful for containers (maps) where the key is
 * case insensitive
 */
struct CaseInsensitiveTrimLess
{
    bool operator()(std::string lhs, std::string rhs) const;
}; 


/*! \brief Equality comparison of a case-insensitive string
 *
 * Useful for containers (maps) where the key is
 * case insensitive
 */
struct CaseInsensitiveEquality
{
    bool operator()(std::string lhs, std::string rhs) const;
}; 


/*! \brief Equality comparison of a case-insensitive string, trimming leading and trailing whitespace
 */
struct CaseInsensitiveTrimEquality
{
    bool operator()(std::string lhs, std::string rhs) const;
}; 


/*! \brief Create a line of characters
 *
 * Repeats a character a number of times. A newline is included.
 *
 * \param [in] c The character to use
 * \param [in] n The number of times to repeat the character
 */
std::string Line(char c, int n = 80);




} // close namespace util
} // close namespace bpmodule

#endif

