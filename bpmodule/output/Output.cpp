/*! \file
 *
 * \brief Output and printing functionality (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include <fstream>
#include <memory>
#include <cassert>


#include "bpmodule/output/Output.hpp"
#include "bpmodule/python/Pybind11.hpp"



namespace {


using namespace bpmodule::output;

// "Global" variables for this file
//std::unique_ptr<std::ofstream> file_;
//std::string filepath_;
std::unique_ptr<OutputStream> globalout_;
bool color_ = false;
bool debug_ = false;


void CreateGlobalOut_(void)
{
    globalout_ = std::unique_ptr<OutputStream>(new OutputStream(std::cout.rdbuf()));
}


}


namespace bpmodule {
namespace output {


OutputStream & GetGlobalOut(void)
{
    if(!globalout_)
        CreateGlobalOut_();
    return *globalout_;
}

void SetOut_Stdout(void)
{
    if(globalout_)
    {
        globalout_->flush();
        globalout_.reset();
    }

    CreateGlobalOut_();
}


/*
bool SetOut_File(const std::string & filepath)
{
    file_ = std::unique_ptr<std::ofstream>(new std::ofstream(filepath.c_str()));
    if(!file_->is_open())
    {
        SetOut_Stdout();
        GlobalError("Error opening file: %?", filepath);
        GlobalError("Using stdout instead!");
        return false;
    }
    else
    {
        SetCoutBuf_(file_->rdbuf());
        usefile_ = true;
        filepath_ = filepath;
        return true;
    }
}
*/


void Flush(void)
{
    if(Valid())
    {
        std::cout.flush();
        if(globalout_)
            globalout_->flush();
    }
}



void SetColor(bool usecolor) noexcept
{
    color_ = usecolor;
}



void SetDebug(bool debug) noexcept
{
    debug_ = debug;
    GlobalDebug("Debugging now: %?\n", debug ? "On" : "Off");
}



bool Valid(void)
{
    /*
    assert(!usefile_ || file_);

    if(usefile_ && !file_->good())
        return false;

    if(!(std::cout.good()))
        return false;
    if(globalout_ && !globalout_->good())
        return false;

    else
        return true;
    */
    return true;
}




namespace detail {

void Output_(std::ostream & out, OutputType type, const std::string & str)
{
    if(type == OutputType::Debug && !debug_)
        return;
    if(color_)
    {
        std::string st = str;
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
            case OutputType::Changed:
                out << "\033[91m" << st << "\033[0m";
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
        out << str;
}

} // close namespace detail




} // close namespace output
} // close namespace bpmodule
