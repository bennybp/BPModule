/*! \file
 *
 * \brief Various string formatting functions (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 

#include "bpmodule/python_helper/Types.hpp"
#include "bpmodule/util/FormatString.hpp"
#include "bpmodule/python_helper/Convert.hpp"




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

            python_helper::PythonType pytype = python_helper::DeterminePyType(it);

            switch(pytype)
            {
                case python_helper::PythonType::Int:
                {
                    // make this safe for large signed and unsigned numbers
                    //! \todo Not much we can do if l < min() though...
                    pybind11::int_ l = python_helper::ConvertToCpp<pybind11::int_>(it);
                    if(l > std::numeric_limits<intmax_t>::max())
                        bfmt % python_helper::ConvertToCpp<uintmax_t>(l);
                    else
                        bfmt % python_helper::ConvertToCpp<intmax_t>(l);
                    break;
                }
                case python_helper::PythonType::Bool:
                {   
                    bfmt % python_helper::ConvertToCpp<bool>(it);
                    break;
                }
                case python_helper::PythonType::Float:
                {   
                    bfmt % python_helper::ConvertToCpp<long double>(it);
                    break;
                }
                case python_helper::PythonType::String:
                {   
                    bfmt % python_helper::ConvertToCpp<std::string>(it);
                    break;
                }
                default: //! \todo Throw exception when printing unknown python object?
                {   
                    pybind11::object ob = it.attr("__str__");
                    pybind11::object s = ob.call();
                    bfmt % python_helper::ConvertToCpp<std::string>(s);
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

