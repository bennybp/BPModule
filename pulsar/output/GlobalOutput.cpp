/*! \file
 *
 * \brief global_ output printing (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include <iostream>
#include <memory>

#include "pulsar/output/GlobalOutput.hpp"


namespace {


using namespace pulsar::output;


bool usecolor_ = false;

//std::unique_ptr<std::ofstream> file_;
//std::string filepath_;
std::unique_ptr<OutputStream> globalout_;


void create_global_out_(void)
{
    globalout_ = std::unique_ptr<OutputStream>(new OutputStream(std::cout.rdbuf()));
}


}


namespace pulsar{
namespace output {


OutputStream & get_global_output(void)
{
    if(!globalout_)
        create_global_out_();
    return *globalout_;
}

void set_global_out_to_stdout(void)
{
    if(globalout_)
    {
        globalout_->flush();
        globalout_.reset();
    }

    create_global_out_();
}


bool color_enabled(void) noexcept
{
    return usecolor_;
}

void enable_color(bool enabled) noexcept
{
    usecolor_ = enabled;
}


} // close namespace output
} // close namespace pulsar
