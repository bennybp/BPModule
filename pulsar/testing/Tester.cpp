/*! \file
 *
 * \brief Definition of the Tester class
 * \author Ryan Richard (ryanmrichard1 <at> gmail.com)
 */

#include "pulsar/testing/Tester.hpp"
#include "pulsar/output/GlobalOutput.hpp"
#include "pulsar/util/StringUtil.hpp"
#include "pulsar/exception/Exceptions.hpp"
#include <cmath>

using pulsar::output::print_global_output;
using pulsar::exception::GeneralException;
using std::to_string;

namespace pulsar {
namespace testing {
//Some utility functions
inline void print_line(){print_global_output(util::line('='));}
inline void print_buffer(){print_global_output("\n\n");}


Tester::Tester(const std::string& desc)
{
    print_line();
    print_global_output(desc+"\n");
    print_line();
}


void Tester::print_results()const
{
    print_buffer();
    print_line();
    const std::string msg="RESULTS: "+to_string(nfailed_)+" failed\n";
    if(nfailed_>0)output::print_global_error(msg);
    else output::print_global_success(msg);
    print_line();
    print_buffer();
    if(nfailed_>0)
        throw GeneralException(to_string(nfailed_)+" tests failed");
}

void Tester::test(const std::string& desc, bool passed)
{
    print_global_output("\n"+to_string(ntest_)+" : "+
             (passed?"Success":"FAILED")+" : "+desc+"\n");
    if(!passed)++nfailed_;
    ++ntest_;
}

void Tester::test(const std::string& desc,double v1,double v2,double tol)
{
    test(desc,std::fabs(v1-v2)<tol);
}


}//End namespace testing 
}//End namespace pulsar

