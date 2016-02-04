/*! \file
 *
 * \brief A smart pointer that keeps a unique ID (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef BPMODULE_GUARD_DATASTORE__UIDPOINTER_HPP_
#define BPMODULE_GUARD_DATASTORE__UIDPOINTER_HPP_

#include <memory>
#include <atomic>

#include "bpmodule/python/Convert.hpp"
#include "bpmodule/output/Output.hpp"


namespace bpmodule {
namespace datastore {

// external counter for the ID
namespace detail {
extern std::atomic<size_t> curid_;
}


/*! \brief A smart pointer that keeps a unique ID
 *
 * Data is only obtainable in const form. Copying and
 * assignment are shallow, and preserve the UID. Changing
 * the internal pointer to something else changes the UID.
 *
 * No two objects of any type should share a UID.
 *
 * This of this as a shared_ptr, but with a UID that should be
 * preserved across program runs.
 *
 * This smart pointer is usable from python. See \ref python_smart_pointer.
 */
template<typename T>
class UIDPointer
{
public:
    /*! \brief Default constructor
     *
     * Is still given a UID
     */  
    UIDPointer()
        : uid_(detail::curid_++)
    { }


    /*! \brief Construct via copy
     *
     * Copy constructs from \p obj
     */  
    UIDPointer(const T & obj)
        : uid_(detail::curid_++), data_(std::make_shared<const T>(obj))
    {
        MakePyObj_();
    }


    /*! \brief Construct via move
     *
     * Move constructs from \p obj
     */  
    UIDPointer(T && obj)
        : uid_(detail::curid_++), data_(std::make_shared<const T>(obj))
    { 
        MakePyObj_();
    }


    UIDPointer(const UIDPointer & /*rhs*/) = default;
    UIDPointer(UIDPointer && /*rhs*/) = default;
    UIDPointer & operator=(const UIDPointer & /*rhs*/) = default;
    UIDPointer & operator=(UIDPointer && /*rhs*/) = default;

    /*! \brief Check if this UIDPointer contains data
     */ 
    operator bool(void) const noexcept
    {
        return Valid();
    }


    /*! \brief Check if this UIDPointer contains data
     */ 
    bool Valid(void) const noexcept
    {
        return bool(data_);
    }


    /*! \brief Get the UID for this object
     */ 
    size_t UID(void) const noexcept
    {
         return uid_;
    }



    /*! \brief Change the data in the UID pointer
     *
     * Will copy construct an object of type \p T. Will change the UID as
     * well.
     */ 
    void Set(const T & obj)
    {
        uid_ = detail::curid_++;
        data_ = std::make_shared<T>(obj);
        MakePyObj_();
    }


    /*! \brief Change the data in the UID pointer
     *
     * Will move construct an object of type \p T. Will change the UID as
     * well.
     */ 
    void Take(T && obj)
    {
        uid_ = detail::curid_++;
        data_ = std::make_shared<const T>(obj);
        MakePyObj_();
    }


    /*! \brief Dereference to the internal data
     *
     * \throw bpmodule::exception::GeneralException if there is no internal
     *        data (ie, if Valid() == false)
     */        
    const T & operator*(void) const
    {
        return Get();
    }


    /*! \brief Dereference to the internal data
     *
     * \throw bpmodule::exception::GeneralException if there is no internal
     *        data (ie, if Valid() == false)
     */        
    const T * operator->(void) const
    {
        if(!Valid())
            throw exception::GeneralException("Attempt to dereference null UIDPointer", "type", util::DemangleCppType<T>());

        return data_.get();
    }


    /*! \brief Dereference to the internal data
     *
     * \throw bpmodule::exception::GeneralException if there is no internal
     *        data (ie, if Valid() == false)
     */        
    const T & Get(void) const
    {
        if(!Valid())
            throw exception::GeneralException("Attempt to dereference null UIDPointer", "type", util::DemangleCppType<T>());

        return *data_;
    }


    /*! \brief Compare two UID pointers
     *
     * Will check if the UIDs are the same. Asserts that
     * the pointers are the same as well if the UIDs
     * are the same
     */ 
    bool operator==(const UIDPointer<T> & rhs)
    {
        if(uid_ == rhs.uid_)
        {
            exception::Assert<exception::GeneralException>(data_ == rhs.data_, "UIDs are the same, but pointers aren't");
            return true;
        }
        return false;
    }


    /*! \brief Used by python for smart pointer semantics
     * 
     * See \ref python_smart_pointer
     *
     * \throw bpmodule::exception::GeneralException if there is no internal
     *        data (ie, if Valid() == false)
     *
     * \note May raise a python exception if the sepcified method doesn't exist
     */
    pybind11::object Py__getattr__(const std::string & name)
    {
        if(!pyobj_)
            throw exception::GeneralException("Attempt to dereference nonexisting python object in UIDPointer", "type", util::DemangleCppType<T>());

        pybind11::object * noconst = const_cast<pybind11::object *>(pyobj_.get());
        return noconst->attr(name.c_str());
    }


private:
    size_t uid_;
    std::shared_ptr<const T> data_;  // actual data
    std::shared_ptr<const pybind11::object> pyobj_; // python object containing a reference

    void MakePyObj_(void)
    {
        try {
        pyobj_ = std::make_shared<const pybind11::object>(python::ConvertToPy(data_.get(), 
                                                          pybind11::return_value_policy::reference));
        }
        catch(exception::PythonConvertException &)
        {
            output::Debug("Object can't be converted to python object: %1% -- don't use from python!\n", util::DemangleCppType<T>());
        }
    }
};




/*! \brief Helps create a UIDPointer
 * 
 * Will copy construct from \p t
 *
 * For the python version, see RegisterUIDPointer.hpp
 */
template<typename T>
UIDPointer<T> MakeUIDPointer(const T & t)
{
    return UIDPointer<T>(t);
}



} // close namespace datastore
} // close namespace bpmodule

#endif
