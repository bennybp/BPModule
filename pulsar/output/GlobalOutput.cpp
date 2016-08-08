/*! \file
 *
 * \brief Global output printing (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include <iostream>
#include <fstream>
#include <memory>

#include "pulsar/output/GlobalOutput.hpp"
#include "pulsar/parallel/InitFinalize.hpp"
#include "pulsar/util/Filesystem.hpp"
#include "pulsar/output/TeeBuf.hpp"

namespace {


using namespace pulsar::output;


bool usecolor_ = false;

std::unique_ptr<OutputStream> globalout_;
std::unique_ptr<std::ofstream> outfile_;
std::unique_ptr<TeeBuf> tbuf_;

}


namespace pulsar{
namespace output {


OutputStream & get_global_output(void)
{
    if(!globalout_)
        throw std::runtime_error("Global output not set up correctly");

    return *globalout_;
}


void create_global_output(const std::string & path,
                          const std::string & base,
                          bool use_stdout)
{
    if(globalout_)
    {
        globalout_->flush();
        globalout_.reset();
        outfile_.reset();
        tbuf_.reset();
    }

    // get my rank
    auto rank = parallel::get_proc_id();

    // create the output file
    std::string fullpath = util::join_path(path, base);
    fullpath += ".";
    fullpath += std::to_string(rank);

    std::cout << "Using file " << fullpath << " as output for rank " << rank << "\n";

    //! \todo truncate or append?
    std::ofstream f(fullpath.c_str(), std::fstream::out | std::fstream::trunc);
    outfile_ = std::unique_ptr<std::ofstream>(new std::ofstream(fullpath.c_str(),
                                              std::fstream::out | std::fstream::trunc));

    if(!outfile_->is_open())
        throw std::runtime_error("Unable to open file for output");


    // Disable output to stdout except for rank 0
    // (and disable for rank 0 if requested)
    auto out2 = std::cout.rdbuf();

    if(!use_stdout || rank != 0)
        out2 = nullptr;


    tbuf_ = std::unique_ptr<TeeBuf>(new TeeBuf(outfile_->rdbuf(), out2));

    globalout_ = std::unique_ptr<OutputStream>(new OutputStream(tbuf_.get()));
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
