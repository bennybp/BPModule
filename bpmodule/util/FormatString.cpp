/*! \file
 *
 * \brief Various string formatting functions (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 

#include <boost/python/long.hpp>

#include "bpmodule/python_helper/Types.hpp"
#include "bpmodule/util/FormatString.hpp"




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

            //! \todo should split types like before
            python_helper::PythonType pytype = python_helper::DeterminePyType2(it);
            pybind11::object str = it.attr("__str__");
            return str.call().cast<std::string>();

            /*
            switch(pytype)
            {
                case python_helper::PythonType::Int:
                {
                    // make this safe for large signed and unsigned numbers
                    //! \todo Not much we can do if l < min() though...
                    boost::python::long_ l = extract<boost::python::long_>(args[i]);
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
                    bfmt % static_cast<std::string>(args[i].attr("__str__").call().cast<std::string>());
                }
            }
            */
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

