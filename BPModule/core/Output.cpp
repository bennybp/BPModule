#include <iostream>
#include <fstream>
#include <memory>

#include <boost/format.hpp>

// This should be safe, right?
// The destructor to file_ should only be called when this cpp file is
// unloading. In which case all the other functions will be unavailable anyway


namespace {
  std::ostream * out_ = nullptr;
  std::unique_ptr<std::ofstream> file_;
}


namespace bpmodule {

void Output_(std::ostream & out, boost::format & fmt)
{
    out << fmt;
}

void SetOut_Stdout(void)
{
    out_ = &std::cout;
}

void SetOut_Stderr(void)
{
    out_ = &std::cerr;
}

void SetOut_File(const std::string & filepath)
{
    file_ = std::unique_ptr<std::ofstream>(new std::ofstream(filepath.c_str()));
    if(!file_->is_open())
        std::cerr << "Error opening file!";
    else
        out_ = file_.get();
}

std::ostream & GetOut(void)
{
    if(out_ == nullptr)
        return std::cerr;
    else
        return *out_;
}

} // close namespace bpmodule
