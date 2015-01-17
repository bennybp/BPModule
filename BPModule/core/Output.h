#ifndef OUTPUT_H
#define OUTPUT_H

#include <iostream>

#include <boost/format.hpp>

namespace bpmodule {

class BPModuleException;


enum class OutputType
{
  Output,
  Success,
  Warning,
  Error,
  Debug
};

// in cpp file
void Output_(std::ostream & out, OutputType type, boost::format & bfmt);

void SetOut_Stdout(void);
bool SetOut_File(const std::string & filepath);
void SetColor(bool usecolor);
void SetDebug(bool debug);

std::ostream & GetOut(void);
void PrintException(const BPModuleException & ex);
std::string ExceptionString(const BPModuleException & ex);

std::string Line(char c, int n = 80);

template<typename T, typename... Targs>
void Output_(std::ostream & out, OutputType type, boost::format & bfmt, T targ, Targs... Fargs)
{
    bfmt % targ;
    Output_(out, type, bfmt, Fargs...);
}


template<typename... Targs>
void Output_(std::ostream & out, OutputType type, std::string fmt, Targs... Fargs)
{
    boost::format bfmt(fmt);
    Output_(out, type, bfmt, Fargs...);
}



template<typename... Targs>
void Output(std::ostream & out, std::string fmt, Targs... Fargs)
{
    Output_(out, OutputType::Output, fmt, Fargs...);
}


template<typename... Targs>
void Output(std::string fmt, Targs... Fargs)
{
    Output(GetOut(), fmt, Fargs...);
}


template<typename... Targs>
void Error(std::ostream & out, std::string fmt, Targs... Fargs)
{
    Output_(out, OutputType::Error, fmt, Fargs...);
}

template<typename... Targs>
void Error(std::string fmt, Targs... Fargs)
{
    Error(GetOut(), fmt, Fargs...);
}


template<typename... Targs>
void Warning(std::ostream & out, std::string fmt, Targs... Fargs)
{
    Output_(out, OutputType::Warning, fmt, Fargs...);
}

template<typename... Targs>
void Warning(std::string fmt, Targs... Fargs)
{
    Warning(GetOut(), fmt, Fargs...);
}

template<typename... Targs>
void Success(std::ostream & out, std::string fmt, Targs... Fargs)
{
    Output_(out, OutputType::Success, fmt, Fargs...);
}

template<typename... Targs>
void Success(std::string fmt, Targs... Fargs)
{
    Success(GetOut(), fmt, Fargs...);
}

template<typename... Targs>
void Debug(std::ostream & out, std::string fmt, Targs... Fargs)
{
    Output_(out, OutputType::Debug, fmt, Fargs...);
}

template<typename... Targs>
void Debug(std::string fmt, Targs... Fargs)
{
    Debug(GetOut(), fmt, Fargs...);
}


} // close namespace bpmodule


#endif
