// Parallelization
#include "BPModule/python_helper/Python_stdconvert.hpp"

#include <map>
#include <unordered_map>

#include <boost/python.hpp>


using namespace boost::python;


namespace bpmodule {
namespace python_helper {
namespace export_python {

BOOST_PYTHON_MODULE(bpmodule_python_helper)
{
    // converting pairs
    to_python_converter<std::pair<int, int>, PairConverter<int, int>>();
    to_python_converter<std::pair<long, long>, PairConverter<long, long>>();
    to_python_converter<std::pair<double, double>, PairConverter<double, double>>();
    to_python_converter<std::pair<std::string, std::string>, PairConverter<std::string, std::string>>();

    // converting vectors
    to_python_converter<std::vector<int>, VectorConverter<int>>();
    to_python_converter<std::vector<long>, VectorConverter<long>>();
    to_python_converter<std::vector<double>, VectorConverter<double>>();
    to_python_converter<std::vector<std::string>, VectorConverter<std::string>>();

    // convert map to dict
    to_python_converter<std::map<std::string, std::string>, MapConverter<std::map<std::string, std::string>>>();
    to_python_converter<std::unordered_map<std::string, std::string>, MapConverter<std::unordered_map<std::string, std::string>>>();
}


} // close namespace export_python
} // close namespace python_helper
} // close namespace bpmodule

