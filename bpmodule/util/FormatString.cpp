/*! \file
 *
 * \brief Various string formatting functions (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include "bpmodule/util/FormatString.hpp"


namespace bpmodule {
namespace util {



std::string FormatString(const std::string & fmt, const std::vector<std::string> & args)
{
    try {

        boost::format bfmt(fmt);

        for(const auto & it : args)
            bfmt % it;


        //! \todo can I just convert boost::format to a string?
        std::stringstream ss;
        detail::FormatStream(ss, bfmt);
        return ss.str();
    }
    catch(const boost::io::format_error & ex)  // let other exceptions pass through
    {
        throw exception::GeneralException("Error in formatting a string or stream from a vector",
                                          "bfmterr", ex.what(),
                                          "fmt", fmt, "nargs", args.size());
    }
}



} // close namespace util
} // close namespace bpmodule

