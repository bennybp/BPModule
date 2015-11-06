/*! \file
 *
 * \brief Various string formatting functions (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 

#include <boost/python/long.hpp>

#include "bpmodule/util/FormatString.hpp"
#include "bpmodule/python_helper/Convert.hpp"


using namespace boost::python;



namespace bpmodule {
namespace util {




/////////////////////////////////
// PYTHON STUFF
/////////////////////////////////
namespace export_python {


std::string FormatStringPy(const std::string & fmt, const boost::python::list & args)
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
                bfmt % static_cast<std::string>(extract<std::string>(args[i].attr("__str__")()));
            }
        }
    }

    //! \todo can I just convert boost::format to a string?
    std::stringstream ss;
    detail::FormatStream(ss, bfmt);
    return ss.str();
}


} // close namespace export_python



} // close namespace utilt
} // close namespace bpmodule

