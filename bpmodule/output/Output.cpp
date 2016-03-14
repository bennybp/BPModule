/*! \file
 *
 * \brief Output and printing functionality (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include <fstream>
#include <memory>
#include <cassert>


#include "bpmodule/output/Output.hpp"
//#include "bpmodule/python/Convert.hpp"
#include "bpmodule/python/Pybind11.hpp"



namespace {


// "Global" variables for this file
std::streambuf * coutbuf_ = nullptr;
std::unique_ptr<std::ofstream> file_;
std::string filepath_;
bool usefile_ = false;
bool color_ = false;
bool debug_ = false;


void SetCoutBuf_(std::streambuf * buf)
{
    if(coutbuf_ == nullptr)
        coutbuf_ = std::cout.rdbuf();
    std::cout.rdbuf(buf);
}

void ResetCoutBuf_(void)
{
    //! \todo exceptions if coutbuf_ == nullptr?
    if(coutbuf_ != nullptr)
        std::cout.rdbuf(coutbuf_);
}



}


namespace bpmodule {
namespace output {


void SetOut_Stdout(void)
{
    ResetCoutBuf_();
    usefile_ = false;
    file_.reset();
}



void SetOut_Stderr(void)
{
    SetCoutBuf_(std::cerr.rdbuf());
    usefile_ = false;
    file_.reset();
}



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



void Flush(void)
{
    if(Valid())
        std::cout.flush();    
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
    assert(!usefile_ || file_);

    if(usefile_ && !file_->good())
        return false;

    if(!(std::cout.good()))
        return false;

    else
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
