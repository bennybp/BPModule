#pragma once
#include "pulsar/system/BasisSet.hpp"


namespace pulsar{

/*! \brief Pulsar side code factorization to accomodate Python API to
 *  integrals
 *
 */
class PythonIntegralHelper
{
protected:
    std::vector<BasisSet> bs_;
    std::vector<double> buffer_;
public:
        /*! \brief initialize the basis sets
         *
         */
        template<typename...Args>
        void initialize(Args...args)
        {
            bs_=std::vector<BasisSet>({args...});
        }

        ///Returns the number of basis functions in the current shell n-tuple
        template<typename...Args>
        size_t size(Args...args)const{
            std::vector<size_t> shells({args...});
            size_t total=1;
            for(size_t i=0;i<shells.size();++i)
                total*=bs_[i].shell(shells[i]).n_functions();
            return total;
        }

        size_t size(const std::vector<std::vector<size_t>>& shells)const{
            size_t n_funcs=1;
            for(size_t i=0;i<shells[0].size();++i)//Assume vectors of same size
                for(size_t j=0;j<shells.size();++j)
                    n_funcs*=bs_[j].shell(shells[j][i]).n_functions();
            return n_funcs;
        }

        template<typename...Args>
        pybind11::list int_2_py(const double* ints,Args...args)const
        {
            pybind11::list rv;
            for(size_t i=0;i<size(args...);++i)
                   rv.append(ints[i]);
            return rv;
        }

        template<typename...Args>
        pybind11::list multi_int_2_py(const double* ints,Args...args)const
        {
            pybind11::list rv;
            std::vector<std::vector<size_t>> shells({args...});
            for(size_t i=0;i<size(shells);++i)
                    rv.append(ints[i]);
            return rv;
        }

        const double* py_2_int(pybind11::list& ints)
        {
            buffer_=std::vector<double>(ints.size());
            for(size_t i=0;i<ints.size();++i)
                buffer_[i]=ints[i].cast<double>();
            return buffer_.data();
        }
};
}
