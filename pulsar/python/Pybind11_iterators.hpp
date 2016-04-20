#ifndef PULSAR_GUARD_PYTHON__PYBIND11_ITERATORS_HPP_
#define PULSAR_GUARD_PYTHON__PYBIND11_ITERATORS_HPP_

#include "pulsar/python/Pybind11.hpp"

namespace pulsar{
namespace python {


//! \todo replace value_type with second template parameter


template<typename Container>
class PyCopyIterator
{
    public:
        typedef typename Container::const_iterator it_type;
        PyCopyIterator(const Container & cont,
                       it_type it,
                       pybind11::object ref)
            : cont_(cont), ref_(ref), it_(it) { }

        typename Container::value_type next(void)
        {
            if(it_ == cont_.end())
                throw pybind11::stop_iteration();
            return *(it_++); 
        }


    private:
        const Container & cont_;
        pybind11::object ref_;
        it_type it_;
};
    


template<typename Container>
void RegisterPyCopyIterator(pybind11::module & m, const char * contname)
{
    std::string itname = std::string(contname) + "_iterator";
    pybind11::class_<PyCopyIterator<Container>>(m, itname.c_str())
    .def("__next__", &PyCopyIterator<Container>::next)
    .def("__iter__", [](PyCopyIterator<Container> & it) -> PyCopyIterator<Container> & { return it; });
}

} // close namespace python
} // close namespace pulsar

#endif
