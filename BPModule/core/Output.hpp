#ifndef OUTPUT_H
#define OUTPUT_H

#include <iostream>
#include <sstream>

#include <boost/format.hpp>

#include "BPModule/python_helper/BoostPython_fwd.hpp"
namespace bpy = boost::python;


namespace bpmodule {


namespace output {


enum class OutputType
{
    Output,
    Success,
    Changed,
    Warning,
    Error,
    Debug
};

// in cpp file
void Output_(std::ostream & out, OutputType type, boost::format & bfmt);
bool Valid(void);
void SetOut_Stdout(void);
void SetOut_Stderr(void);
bool SetOut_File(const std::string & filepath);
void SetColor(bool usecolor);
void SetDebug(bool debug);
void Flush(void);
std::ostream & GetOut(void);
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



/////////////////////
// Output
/////////////////////
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
std::string OutputStr(std::string fmt, Targs... Fargs)
{
    std::stringstream ss;
    Output(ss, fmt, Fargs...);
    return ss.str();
}



/////////////////////
// Changed
/////////////////////
template<typename... Targs>
void Changed(std::ostream & out, std::string fmt, Targs... Fargs)
{
    Output_(out, OutputType::Changed, fmt, Fargs...);
}

template<typename... Targs>
void Changed(std::string fmt, Targs... Fargs)
{
    Changed(GetOut(), fmt, Fargs...);
}

template<typename... Targs>
std::string ChangedStr(std::string fmt, Targs... Fargs)
{
    std::stringstream ss;
    Changed(ss, fmt, Fargs...);
    return ss.str();
}



/////////////////////
// Error
/////////////////////
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
std::string ErrorStr(std::string fmt, Targs... Fargs)
{
    std::stringstream ss;
    Error(ss, fmt, Fargs...);
    return ss.str();
}



/////////////////////
// Warning
/////////////////////
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
std::string WarningStr(std::string fmt, Targs... Fargs)
{
    std::stringstream ss;
    Warning(ss, fmt, Fargs...);
    return ss.str();
}



/////////////////////
// Success
/////////////////////
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
std::string SuccessStr(std::string fmt, Targs... Fargs)
{
    std::stringstream ss;
    Success(ss, fmt, Fargs...);
    return ss.str();
}



/////////////////////
// Debug
/////////////////////
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

template<typename... Targs>
std::string DebugStr(std::string fmt, Targs... Fargs)
{
    std::stringstream ss;
    Debug(ss, fmt, Fargs...);
    return ss.str();
}

} // close namespace output



////////////////////////////////
// Wrappers for use from Python
// (in Output_python.cpp)
////////////////////////////////
namespace export_python {

void Output_Wrap(bpmodule::output::OutputType type, const std::string fmt, const bpy::list & args);
void Output_Wrap_Output(const std::string fmt, const bpy::list & args);
void Output_Wrap_Success(const std::string fmt, const bpy::list & args);
void Output_Wrap_Changed(const std::string fmt, const bpy::list & args);
void Output_Wrap_Warning(const std::string fmt, const bpy::list & args);
void Output_Wrap_Error(const std::string fmt, const bpy::list & args);
void Output_Wrap_Debug(const std::string fmt, const bpy::list & args);

} // close namespace export_python



} // close namespace bpmodule


#endif
