/*! \file
 *
 * \brief Base class for testing of Pulsar (source)
 */

#include "pulsar/testing/TesterBase.hpp"
#include "pulsar/output/GlobalOutput.hpp"
#include "pulsar/util/StringUtil.hpp"

namespace pulsar {

//Some utility functions
inline void print_line(){print_global_output(line('='));}
inline void print_buffer(){print_global_output("\n\n");}


TesterBase::TesterBase(const std::string& desc)
{
    print_line();
    print_global_output(desc+"\n");
    print_line();
}


void TesterBase::print_results()const
{
    print_buffer();
    print_line();

    const std::string msg = format_string("RESULTS: %? failed\n", nfailed_);
    if(nfailed_>0)
        print_global_error(msg);
    else
        print_global_success(msg);

    print_line();
    print_buffer();
}

size_t TesterBase::nfailed(void) const noexcept
{
    return nfailed_;
}

size_t TesterBase::ntest(void) const noexcept
{
    return ntest_;
}

void TesterBase::test(const std::string& desc, bool passed)
{
    const char * res = passed ? "Success" : "FAILED";
    std::string l = format_string("\n%3? : %? : %?\n", ntest_+1, res, desc);
    print_global_output(l);

    // none of the following can throw an exception
    if(!passed)
        ++nfailed_;

    ++ntest_;
}

}//End namespace pulsar

