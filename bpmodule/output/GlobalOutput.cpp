/*! \file
 *
 * \brief Global output printing (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include <iostream>
#include <memory>

#include "bpmodule/output/GlobalOutput.hpp"


namespace {


using namespace bpmodule::output;


bool usecolor_ = false;

//std::unique_ptr<std::ofstream> file_;
//std::string filepath_;
std::unique_ptr<OutputStream> globalout_;


void CreateGlobalOut_(void)
{
    globalout_ = std::unique_ptr<OutputStream>(new OutputStream(std::cout.rdbuf()));
}


}


namespace bpmodule {
namespace output {


OutputStream & GetGlobalOut(void)
{
    if(!globalout_)
        CreateGlobalOut_();
    return *globalout_;
}

void SetGlobalOut_Stdout(void)
{
    if(globalout_)
    {
        globalout_->flush();
        globalout_.reset();
    }

    CreateGlobalOut_();
}


bool ColorEnabled(void) noexcept
{
    return usecolor_;
}

void EnableColor(bool enabled) noexcept
{
    usecolor_ = enabled;
}


} // close namespace output
} // close namespace bpmodule
