#ifndef OUTPUT_H
#define OUTPUT_H

#include <iostream>

#include <boost/format.hpp>

namespace bpmodule {

void SetOut_Stdout(void);
void SetOut_Stderr(void);
void SetOut_File(const std::string & filepath);
std::ostream & GetOut(void);


// in cpp file
void Output_(std::ostream & out, boost::format & fmt);


template<typename T, typename... Targs>
void Output_(std::ostream & out, boost::format & fmt, T targ, Targs... Fargs)
{
    fmt % targ;
    Output_(out, fmt, Fargs...);
}

template<typename... Targs>
void Output_(std::ostream & out, const char * fmt, Targs... Fargs)
{
    boost::format bfmt(fmt);
    Output_(out, bfmt, Fargs...);
}


template<typename... Targs>
void Stdout(const char * fmt, Targs... Fargs)
{
    Output_(std::cout, fmt, Fargs...);
}

template<typename... Targs>
void Stderr(const char * fmt, Targs... Fargs)
{
    Output_(std::cerr, fmt, Fargs...);
}


template<typename... Targs>
void Output(const char * fmt, Targs... Fargs)
{
    Output_(GetOut(), fmt, Fargs...);
}



} // close namespace bpmodule


#endif
