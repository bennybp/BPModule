/*! \file
 *
 * \brief Various string formatting functions (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 

#include "bpmodule/python/Types.hpp"
#include "bpmodule/util/FormatString.hpp"
#include "bpmodule/python/Convert.hpp"




namespace bpmodule {
namespace util {




/////////////////////////////////
// PYTHON STUFF
/////////////////////////////////
namespace export_python {


//! \todo pybind11 is missing begin() const
std::string FormatStringPy(const std::string & fmt, pybind11::list args)
{
    try {

        boost::format bfmt(fmt);

        for(auto it : args)
        {

            python::PythonType pytype = python::DeterminePyType(it);

            switch(pytype)
            {
                case python::PythonType::Int:
                {
                    // make this safe for large signed and unsigned numbers
                    //! \todo Not much we can do if l < min() though...
                    pybind11::int_ l = python::ConvertToCpp<pybind11::int_>(it);
                    if(l > std::numeric_limits<intmax_t>::max())
                        bfmt % python::ConvertToCpp<uintmax_t>(l);
                    else
                        bfmt % python::ConvertToCpp<intmax_t>(l);
                    break;
                }
                case python::PythonType::Bool:
                {   
                    bfmt % python::ConvertToCpp<bool>(it);
                    break;
                }
                case python::PythonType::Float:
                {   
                    bfmt % python::ConvertToCpp<long double>(it);
                    break;
                }
                case python::PythonType::String:
                {   
                    bfmt % python::ConvertToCpp<std::string>(it);
                    break;
                }
                default: //! \todo Throw exception when printing unknown python object?
                {   
                    pybind11::object ob = it.attr("__str__");
                    pybind11::object s = ob.call();
                    bfmt % python::ConvertToCpp<std::string>(s);
                }
            }
        }

        //! \todo can I just convert boost::format to a string?
        std::stringstream ss;
        detail::FormatStream(ss, bfmt);
        return ss.str();
    }
    catch(const boost::io::format_error & ex)  // let other exceptions pass through
    {
        throw exception::GeneralException("Error in formatting a string or stream from python",
                                          "bfmterr", ex.what(),
                                          "fmt", fmt, "nargs", args.size());
    }
}


} // close namespace export_python



} // close namespace utilt
} // close namespace bpmodule

