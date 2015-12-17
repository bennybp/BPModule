#ifndef _GUARD_UIDPOINTER_HPP_
#define _GUARD_UIDPOINTER_HPP_

#include <memory>
#include <atomic>

#include "bpmodule/python/Convert.hpp"


namespace bpmodule {
namespace datastore {

// external counter for the ID
namespace detail {
extern std::atomic<size_t> curid_;
}

//! \todo some of these names may interfere with the contained type (for python getattr)
template<typename T>
class UIDPointer
{
public:
    UIDPointer()
        : uid_(detail::curid_++)
    { }

    UIDPointer(const T & obj)
        : uid_(detail::curid_++), data_(std::make_shared<const T>(obj))
    {
        pyobj_ = std::make_shared<const pybind11::object>(python::ConvertToPy(data_.get(), pybind11::return_value_policy::reference));
    }

    UIDPointer(T && obj)
        : uid_(detail::curid_++), data_(std::make_shared<const T>(obj))
    { 
        pyobj_ = std::make_shared<const pybind11::object>(python::ConvertToPy(data_.get(), pybind11::return_value_policy::reference));
    }


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
        pyobj_ = std::make_shared<const pybind11::object>(python::ConvertToPy(data_.get(), pybind11::return_value_policy::reference));
    }


    void Take(T && obj)
    {
        uid_ = detail::curid_++;
        data_ = std::make_shared<const T>(obj);
        pyobj_ = std::make_shared<const pybind11::object>(python::ConvertToPy(data_.get(), pybind11::return_value_policy::reference));
    }


    //! \todo noexcept?
    const T & operator*(void) const
    {
        if(!Valid())
            throw bpmodule::exception::GeneralException("Attempt to dereference null UIDPointer", "type", util::DemangleCppType<T>());

        return *data_;
    }

    //! \todo noexcept?
    const T & Get(void) const
    {
        if(!Valid())
            throw bpmodule::exception::GeneralException("Attempt to dereference null UIDPointer", "type", util::DemangleCppType<T>());

        return *data_;
    }

    //! \todo noexcept?
    const T * operator->(void) const
    {
        if(!Valid())
            throw bpmodule::exception::GeneralException("Attempt to dereference null UIDPointer", "type", util::DemangleCppType<T>());

        return data_.get();
    }


    bool operator==(const UIDPointer<T> & rhs)
    {
        return uid_ == rhs.uid_ && data_ == rhs.data_;
    }

    pybind11::object Py__getattr__(const std::string & name)
    {
        if(!Valid())
            throw bpmodule::exception::GeneralException("Attempt to dereference null UIDPointer", "type", util::DemangleCppType<T>());

        pybind11::object * noconst = const_cast<pybind11::object *>(pyobj_.get());
        return noconst->attr(name.c_str());
    }

private:
    size_t uid_;
    std::shared_ptr<const T> data_;
    std::shared_ptr<const pybind11::object> pyobj_; // data as a python object
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
