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



} // close namespace math
} // close namespace pulsar


#endif
