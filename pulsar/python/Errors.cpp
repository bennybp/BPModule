/*! \file
 *
 * \brief Formatting of python exceptions (source)
 */

#include "pulsar/python/Errors.hpp"
#include "pulsar/python/Types.hpp"
#include "pulsar/exception/Exceptions.hpp"
#include "pulsar/util/Format.hpp"
#include "pulsar/util/StringUtil.hpp"

#include <pybind11/pybind11.h>

#include <algorithm> // std::reverse

using pulsar::GeneralException;
using pulsar::util::format_string;
using pulsar::util::join;

namespace pulsar {
namespace python {
namespace detail {

/*! \brief Formats a traceback (breaks out line numbers and file names)
 */
static std::string format_traceback(pybind11::object tb)
{
    std::vector<std::string> v;

    // loop over all the frames and grab the info
    // This will result in a "reverse" traceback
    while(tb && !tb.is_none())
    {
        // line number is easy
        long lineno = tb.attr("tb_lineno").cast<long>();

        // filename is kind of buried
        pybind11::object fname = tb.attr("tb_frame");
        fname = fname.attr("f_code");
        fname = fname.attr("co_filename");
        std::string fname_string = fname.cast<std::string>();

        v.push_back(format_string("Line %? of %?",
                                  lineno, fname_string));

        // get the next frame
        tb = tb.attr("tb_next");
    }

    // put into the order generally printed by pulsar
    // (ie, most recent on top)
    std::reverse(v.begin(), v.end());

    // join with newlines and return a single string
    return join(v, "\n");
}


std::pair<std::string, std::string> get_py_exception(void)
{
    // Note: Only handles the built-in string exceptions and GeneralException.
    //       For more, add them here or use isinstance, etc

    try {
        //! \todo Get traceback info?
        if(PyErr_Occurred() != NULL)
        {
            PyObject *type = nullptr;
            PyObject *value = nullptr;
            PyObject *traceback = nullptr;

            PyErr_Fetch(&type, &value, &traceback);

            pybind11::object type_obj(type, false); // false = take ownership
            pybind11::object value_obj(value, false);
            pybind11::object traceback_obj(traceback, false);

            if(!value_obj)
                return {"Python error object is null", ""};


            // Get the traceback as a string
            std::string tb_str;
            if(traceback_obj)
                tb_str = format_traceback(traceback_obj);


            // the type of the exception
            std::string extype = get_py_class(value_obj);

            /*
            if(extype == "GeneralException") 
            {
                // The python version of GeneralException
                // is directly convertible to a string (via __str__)
                pybind11::object gex = value_obj.attr("gex");
                return gex.cast<const GeneralException &>().what();
            }
            */

            std::string errstr;

            if(has_attr(value_obj, "__str__"))
            {
                // Is the type convertible to a string?
                // This handles TypeError, RuntimeError, etc
                // This also handles GeneralException
                pybind11::object s_attr = value_obj.attr("__str__");
                errstr = s_attr.call().cast<std::string>();
            }
            else
            {
                std::string cls = get_py_class(value_obj);
                errstr = format_string("Unknown python exception type %?", cls);
            }

            return {errstr, tb_str};
        }
        else
            return {"(no python error)", ""};
    }
    catch(std::exception & ex)
    {
        std::string errstr("ERROR: EXCEPTION THROWN IN GETTING PYTHON ERROR\n");
        errstr += ex.what();
        return {errstr, ""};
    }
    catch(...)
    {
        return {"ERROR: UNKNOWN EXCEPTION THROWN IN GETTING PYTHON EXCEPTION", ""};
    }
}


} // close namespace detail
} // close namespace python
} // close namespace pulsar
