/*! \file
 *
 * \brief Output and printing functionality (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include <fstream>
#include <memory>
#include <cassert>

#include <boost/python/long.hpp>
#include <boost/python/list.hpp>

#include "bpmodule/output/Output.hpp"
#include "bpmodule/python_helper/Convert.hpp"


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

void Output_(std::ostream & out, OutputType type, const boost::format & bfmt)
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



void Flush(void)
{
    if(Valid())
        GetOut().flush();    
}



void SetColor(bool usecolor) noexcept
{
    color_ = usecolor;
}



void SetDebug(bool debug) noexcept
{
    debug_ = debug;
    Debug("Debugging now: %1%\n", debug ? "On" : "Off");
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
        Error("General output destination is a null pointer. Using stderr");
    }
    else if(usefile_ && !file_->good())
    {
        out_ = &std::cerr;
        Error("General output file %1% no longer \"good\". Using stderr", filepath_);
    }

    return *out_;
}


//////////////////////////
// PYTHON FUNCTIONS
//////////////////////////
void OutputPy_(std::ostream & os, output::OutputType type, const std::string & fmt, const boost::python::list & args)
{
    boost::format bfmt(fmt);

    int len = boost::python::extract<int>(args.attr("__len__")());

    for(int i = 0; i < len; i++)
    {
        python_helper::PythonType pytype = python_helper::DeterminePyType(args[i]);

        switch(pytype)
        {
            case python_helper::PythonType::Int:
            {
                // make this safe for large signed and unsigned numbers
                //! \todo Not much we can do if l < min() though...
                boost::python::long_ l = boost::python::extract<boost::python::long_>(args[i]);
                if(l > std::numeric_limits<intmax_t>::max())
                    bfmt % python_helper::ConvertToCpp<uintmax_t>(l);
                else
                    bfmt % python_helper::ConvertToCpp<intmax_t>(l);
                break;
            }
            case python_helper::PythonType::Bool:
            {
                bfmt % python_helper::ConvertToCpp<bool>(args[i]);
                break;
            }
            case python_helper::PythonType::Float:
            {
                bfmt % python_helper::ConvertToCpp<long double>(args[i]);
                break;
            }
            case python_helper::PythonType::String:
            {
                bfmt % python_helper::ConvertToCpp<std::string>(args[i]);
                break;
            }
            default: //! \todo Throw exception when printing unknown python object?
            {
                bfmt % static_cast<std::string>(boost::python::extract<std::string>(args[i].attr("__str__")()));
            }
        }
    }

    output::Output_(os, type, boost::str(bfmt));
}


} // close namespace output
} // close namespace bpmodule
