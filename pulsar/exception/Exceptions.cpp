/*! \file
 * \brief Exceptions thrown by Pulsar (source)
 */

#include "pulsar/exception/Exceptions.hpp"

#include <iomanip>

namespace pulsar {
namespace exception {


void GeneralException::append_info_(const std::string & key,
                                    const std::string & value)
{
    // NOTE: The formatting here is done manually so that
    //       it is completely independent of the rest of
    //       Pulsar

    // The full, formatted output
    std::stringstream ss;

    // Store the original width of the stringstream
    auto oldw = ss.width();

    // The value parameter as a string
    std::string str;

    // Convert the value to a string
    std::stringstream sstr(value);

    // Get the first line of the formatted output and
    // output it to the stream
    std::getline(sstr, str);
    ss << std::setw(24) << key << std::setw(oldw) << " : " << str;

    // For the rest of the lines, output the values aligned
    // with the first
    while(std::getline(sstr, str).good())
        ss << "\n" << std::setw(24) << " " << oldw << "     " << str;

    // The resulting string may be output via printf() or the
    // pulsar output functions which use percent signs as escape
    // characters. So we must escape the percent signs here
    std::string escaped;

    for(const auto & it : ss.str())
    {
        if(it == '%')
            escaped.append("%%");
        else
            escaped.push_back(it);
    }

    // now add the information to my final string
    whatstr_.append("\n");
    whatstr_.append(escaped);
}


const char * GeneralException::what(void) const noexcept
{
    return whatstr_.c_str();
}


} // close namespace exception
} // close namespace pulsar

