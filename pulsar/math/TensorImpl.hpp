#ifndef PULSAR_GUARD_TENSOR__TENSORIMPL_HPP_
#define PULSAR_GUARD_TENSOR__TENSORIMPL_HPP_

#include<array>
#include<iostream>
#include<iomanip>
#include<pulsar/util/IterTools.hpp>


namespace pulsar{
namespace math {

template<size_t Rank, typename DataType>
class TensorImpl
{
    public:
        virtual std::array<size_t, Rank> sizes(void) const = 0;
        virtual DataType get_value(std::array<size_t, Rank> idx) const = 0;
        virtual void set_value(std::array<size_t, Rank> idx, DataType val) = 0;

        size_t size(int dim) const { return sizes().at(dim); }
        ///Prints the tensor by calling get_value many times (i.e. this is slow)
        std::ostream& print(std::ostream& os)const;
};

///Specialization so that Python calls the right virtual function
template<size_t Rank,typename DataType>
class TensorImpl_Py : public TensorImpl<Rank,DataType>{
    public:
        std::array<size_t,Rank> sizes()const
        {
            PYBIND11_OVERLOAD_PURE(
                std::array<size_t,Rank>,
                TensorImpl,
                sizes
            );
        }
        
        DataType get_value(std::array<size_t,Rank> idx)const
        { 
          PYBIND11_OVERLOAD_PURE(
              DataType,
              TensorImpl,
              get_value,
              idx
          );
        }
        
        void set_value(std::array<size_t,Rank> idx,DataType val)
        {
            PYBIND11_OVERLOAD_PURE(
                    void,
                    TensorImpl,
                    set_value,
                    idx,val
            );
        }
};


template<size_t Rank,typename DataType>
std::ostream& TensorImpl<Rank,DataType>::print(std::ostream& os)const
{
    if(Rank>2){
        os<<"Tensors of rank 3+ can't be printed yet (feel free to implement it)"
          <<std::endl;
        return os;
    }
    for(size_t i: pulsar::util::Range<0>(Rank==2?sizes[0]:1))
    {
        std::array<size_t,Rank> Idx;
        if(Rank==2)Idx[0]=i;
        os<<"{"<<std::endl;
        int counter=0;
        for(size_t j: pulsar::util::Range<0>(sizes[Rank-1]))
        {
            Idx[Rank-1]=j;
            os<<std::setprecision(13)<<get_value(Idx);
            if(++counter==5)//5*13 digits + 5 decimal points +4 commas=74 chars
            {
                os<<std::endl;
                counter=0;
            }
            else os<<",";
        }
        os<<"}"<<std::endl;
    }
    return os;
}

template<size_t Rank,typename DataType>
std::ostream& operator<<(std::ostream& os, const TensorImpl<Rank,DataType>& t)
{
   return t.print(os);
}

} // close namespace math
} // close namespace pulsar


#endif
