/*! \file
 *
 * \brief Some miscellaneous time functions (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include <ctime>
#include <chrono>

#include "pulsar/util/Time.hpp"


namespace pulsar {

std::string timestamp_string(void)
{
    char buf[128];

    time_t timer;
    struct tm* tmi;

    time(&timer);
    tmi = localtime(&timer);


    strftime(buf, 128, "%c", tmi);
    return std::string(buf);
}

} // close namespace pulsar

