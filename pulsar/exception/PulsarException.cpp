/*! \file
 * \brief Exceptions thrown by Pulsar (source)
 */

#include "pulsar/exception/PulsarException.hpp"

#include <iomanip>

namespace pulsar {

void PulsarException::build_str_(std::string & outstr,
                                 const std::string & key,
                                 const std::string & value)
{
    // The value string that is passed in may contain multiple
    // lines (via inclusion of newlines). So we go over all the
    // lines and indent them so that they look nice.

    // NOTE: The formatting here is done manually so that
    //       it is completely independent of the rest of
    //       Pulsar

    // The full, formatted output
    std::stringstream ss;

    // Store the original width of the stringstream
    auto oldw = ss.width();

    // The value parameter as a string
    std::string str;

    // Convert the value to a stringstream
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
    std::string escaped("\n"); // start with a newline

    for(const auto & it : ss.str())
    {
        if(it == '%')
            escaped.append("%%");
        else
            escaped.push_back(it);
    }

    // now add the information to my final string
    // std::string.append() has strong exception safety
    outstr.append(escaped);
}

} // close namespace pulsar
