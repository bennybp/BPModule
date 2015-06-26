//#include <iostream>
//#include <fstream>
#include <memory>

#include <cassert>

#include <boost/format.hpp>

#include "BPModule/core/Exception.h"
#include "BPModule/core/Output.h"


// This should be safe, right?
// The destructor to file_ should only be called when this cpp file is
// unloading. In which case all the other functions will be unavailable anyway

namespace {
  std::ostream * out_ = nullptr;
  std::unique_ptr<std::ofstream> file_;
  std::string filepath_;
  bool usefile_ = false;
  bool color_ = false;
  bool debug_ = false;
}


namespace bpmodule {
namespace output {

void Output_(std::ostream & out, OutputType type, boost::format & bfmt)
{
    if(type == OutputType::Debug && !debug_)
        return;
    if(color_)
    {
        std::string st = bfmt.str();
        if(st.size() == 0)
            return;

        // strip out the last newline character. Makes it look ugly
        bool newline = (st.back() == '\n');
        if(newline)
            st = st.substr(0, st.size()-1);

        switch(type)
        {
            case OutputType::Success:
                out << "\033[32m" << st << "\033[0m";
                break;
            case OutputType::Debug:
                out << "\033[35m" << st << "\033[0m";
                break;
            case OutputType::Warning:
                out << "\033[33m" << st << "\033[0m";
                break;
            case OutputType::Error:
                out << "\033[37;41m" << st << "\033[0m";
                break;
            default:
                out << st;
                break;

        }
        if(newline)
            out << "\n";
    }
    else
        out << bfmt;
}



void SetOut_Stdout(void)
{
    out_ = &std::cout;
    usefile_ = false;
    file_.reset();
}



void SetOut_Stderr(void)
{
    out_ = &std::cerr;
    usefile_ = false;
    file_.reset();
}



bool SetOut_File(const std::string & filepath)
{
    file_ = std::unique_ptr<std::ofstream>(new std::ofstream(filepath.c_str()));
    if(!file_->is_open())
    {
        SetOut_Stdout();
        Error("Error opening file: %1%", filepath);
        Error("Using stdout instead!");
        return false;
    }
    else
    {
        out_ = file_.get();
        usefile_ = true;
        filepath_ = filepath;
        return true;
    }
}



void SetColor(bool usecolor)
{
    color_ = usecolor;
}



void SetDebug(bool debug)
{
    debug_ = debug;
    Debug("Debugging now: %1%\n", debug ? "On" : "Off");
}



std::string Line(char c, int n)
{
    return std::string(n, c) + "\n";
}



bool Valid(void)
{
    assert(!usefile_ || file_);
    if(out_ == nullptr)
        return false;

    if(usefile_ && !file_->good())
        return false;

    if(!out_->good())
        return false;

    else
        return true;
}

std::ostream & GetOut(void)
{
    assert(!usefile_ || file_);

    if(out_ == nullptr)
    {
        out_ = &std::cerr;
        throw BPOutputException();
    }
    else if(usefile_ && !file_->good())
    {
        out_ = &std::cerr;
        throw BPOutputException(filepath_, *file_);
    }
    else
        return *out_;
}



void PrintException(const BPModuleException & ex)
{
    BPModuleException::ExceptionInfo exinfo = ex.GetInfo();
    Output(ExceptionString(ex)); // Just output - color is handled by ExceptionString
}



std::string ExceptionString(const BPModuleException & ex)
{
    BPModuleException::ExceptionInfo exinfo = ex.GetInfo();
    std::stringstream ss;
    Error(ss, Line('*'));
    Error(ss, "Exception thrown!\n");
    Error(ss, "what() = %1%\n", ex.what());
    for(auto & it : exinfo)
        if(it.second.size())
          Error(ss, "%|16| : %|-16|\n", it.first, it.second);
    return ss.str();
}


} // close namespace output
} // close namespace bpmodule
