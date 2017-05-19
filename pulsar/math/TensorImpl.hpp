#ifndef PULSAR_GUARD_TENSOR__TENSORIMPL_HPP_
#define PULSAR_GUARD_TENSOR__TENSORIMPL_HPP_

#include<array>
#include<iostream>
#include<iomanip>
#include "pulsar/util/Pybind11.hpp"
#include "pulsar/util/IterTools.hpp"
#include "pulsar/util/Serialization.hpp"
#include <bphash/types/memory.hpp>

namespace pulsar{

///Provides the minimal interface a tensor must implement to work with Pulsar
template<size_t Rank, typename DataType>
class TensorImpl
{
    public:
        /*! \brief For serialization only
         * 
         * \warning NOT FOR USE OUTSIDE OF SERIALIZATION
         * \todo Replace if cereal fixes this
         */
        TensorImpl() = default;

        ///Should return an arry of each dimension's length
        virtual std::array<size_t, Rank> sizes(void) const = 0;
        
        ///Should return the element stored at \p idx
        virtual DataType get_value(std::array<size_t, Rank> idx) const = 0;
        
        ///Should set \p idx to \p val
        virtual void set_value(std::array<size_t, Rank> idx, DataType val) = 0;


        ///Returns the length of dimension \p dim
        size_t size(int dim) const { return sizes().at(dim); }
        
        ///Prints the tensor by calling get_value many times (i.e. this is slow)
        std::ostream & print(std::ostream& os) const;

    protected:
        BPHASH_DECLARE_HASHING_FRIENDS
        DECLARE_SERIALIZATION_FRIENDS

        template<class Archive>
        void save(Archive & ) const
        {
            // reserved for future use
        }

        template<class Archive>
        void load(Archive & )
        {
            // reserved for future use
        }

        virtual void hash(bphash::Hasher &) const
        {

        }
};


///Specialization so that Python calls the right virtual function
template<size_t Rank,typename DataType>
class TensorImpl_Py : public TensorImpl<Rank,DataType>{
    private:
        //g++ is giving me problems with passing these as argument to a macro
        using SizeArray=std::array<size_t,Rank>;///<Type of the lengths
        using Base_t=TensorImpl<Rank,DataType>;///<Type of the base class
    public:
        ///\copydoc sizes
        std::array<size_t,Rank> sizes()const
        {
            PYBIND11_OVERLOAD_PURE(SizeArray,Base_t,sizes);
        }
        
        ///\copydoc get_value
        DataType get_value(std::array<size_t,Rank> idx)const
        { 
          PYBIND11_OVERLOAD_PURE(DataType,Base_t,get_value,idx);
        }
        
        ///\copydoc set_value
        void set_value(std::array<size_t,Rank> idx,DataType val)
        {
            PYBIND11_OVERLOAD_PURE(void,Base_t,set_value,idx,val);
        }
};

/********************* Implementations ***************************************/

template<size_t Rank,typename DataType>
std::ostream& TensorImpl<Rank,DataType>::print(std::ostream& os)const
{
    if(Rank>2){
        os<<"Tensors of rank 3+ can't be printed yet (feel free to implement it)"
          <<std::endl;
        return os;
    }
    for(size_t i: pulsar::Range<0>(Rank==2?sizes[0]:1))
    {
        std::array<size_t,Rank> Idx;
        if(Rank==2)Idx[0]=i;
        os<<"{"<<std::endl;
        int counter=0;
        for(size_t j: pulsar::Range<0>(sizes[Rank-1]))
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
typedef TensorImpl<3, double> Rank3DImpl;
typedef TensorImpl<2, double> MatrixDImpl;
typedef TensorImpl<1, double> VectorDImpl;
using PyRank3DImpl=TensorImpl_Py<3,double>;
using PyMatrixDImpl=TensorImpl_Py<2,double>;
using PyVectorDImpl=TensorImpl_Py<1,double>;

} // close namespace pulsar


#endif
