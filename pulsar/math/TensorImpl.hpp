#ifndef PULSAR_GUARD_TENSOR__TENSORIMPL_HPP_
#define PULSAR_GUARD_TENSOR__TENSORIMPL_HPP_


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


} // close namespace math
} // close namespace pulsar


#endif
