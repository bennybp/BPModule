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

    operator bool(void) const noexcept
    {
        return Valid();
    }

    bool Valid(void) const noexcept
    {
        return bool(data_);
    }

    size_t UID(void) const noexcept
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
    const T & operator*(void) const
    {
        return *data_;
    }

    //! \todo noexcept?
    const T & Get(void) const
    {
        return *data_;
    }

    //! \todo noexcept?
    const T * operator->(void) const
    {
        return data_.get();
    }


    bool operator==(const UIDPointer<T> & rhs)
    {
        return uid_ == rhs.uid_ && data_ == rhs.data_;
    }

private:
    size_t uid_;
    std::shared_ptr<const T> data_;
};


/*! \brief Helps create a UIDPointer
 */
template<typename T>
UIDPointer<T> MakeUIDPointer(const T & t)
{
    return UIDPointer<T>(t);
}



} // close namespace datastore
} // close namespace bpmodule

#endif
