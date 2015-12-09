#ifndef _GUARD_UIDPOINTER_HPP_
#define _GUARD_UIDPOINTER_HPP_

#include <memory>
#include <atomic>

namespace bpmodule {
namespace datastore {

// external counter for the ID
namespace detail {
extern std::atomic<size_t> curid_;
}


template<typename T>
class UIDPointer
{
public:
    UIDPointer()
        : uid_(detail::curid_++), data_(nullptr)
    { }

    UIDPointer(const T & obj)
        : uid_(detail::curid_++), data_(std::make_shared<const T>(obj))
    { }

    UIDPointer(T && obj)
        : uid_(detail::curid_++), data_(std::make_shared<const T>(obj))
    { }


    // copying and moving the uid is ok
    UIDPointer(const UIDPointer & rhs) = default;
    UIDPointer(UIDPointer && rhs) = default;
    UIDPointer & operator=(const UIDPointer & rhs) = default;
    UIDPointer & operator=(UIDPointer && rhs) = default;

    operator bool(void) const
    {
        return bool(data_);
    }


    size_t UID(void) const
    {
         return uid_;
    }


    void Set(const T & obj)
    {
        uid_ = detail::curid_++;
        data_ = std::make_shared<T>(obj);
    }


    void Take(T && obj)
    {
        uid_ = detail::curid_++;
        data_ = std::make_shared<T>(obj);
    }


    //! \todo noexcept?
    const T & operator*(void)
    {
        return *data_;
    }

    //! \todo noexcept?
    const T * operator->(void)
    {
        return data_.get();
    }



private:
    size_t uid_;
    std::shared_ptr<const T> data_;
};



} // close namespace datastore
} // close namespace bpmodule

#endif
