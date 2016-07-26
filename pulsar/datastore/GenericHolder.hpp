/*! \file
 *
 * \brief Storage of generic data (inner class) 
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef PULSAR_GUARD_DATASTORE__GENERICHOLDER_HPP_
#define PULSAR_GUARD_DATASTORE__GENERICHOLDER_HPP_

#include "pulsar/util/Mangle.hpp"
#include "pulsar/datastore/GenericBase.hpp"

#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>


namespace pulsar{
namespace datastore {
namespace detail {


/*! \brief A container that can hold anything
 *
 * \tparam T The type of the data this object is holding
 */ 
template<typename T, bool = cereal::traits::is_output_serializable<T, cereal::BinaryOutputArchive>::value &&
                            cereal::traits::is_input_serializable<T, cereal::BinaryInputArchive>::value >
class GenericHolder;


template<typename T>
class GenericHolderBase : public GenericBase
{
    public:
        /*! \brief Construct via copying a data object
         * 
         * Will invoke copy constructor for type T
         *
         * \throwno Throws an exception only if the move
         *          constructor for T throws an exception
         *
         *  \param [in] m The object to copy
         */
        GenericHolderBase(const T & m)
            : obj(std::make_shared<const T>(m))
        { }


        /*! \brief Construct via moving a data object
         * 
         * Will invoke move constructor for type T
         *
         * \throwno Throws an exception only if the move
         *          constructor for T throws an exception
         *
         * \param [in] m The object to move
         */
        GenericHolderBase(T && m)
            : obj(std::make_shared<const T>(std::move(m)))
        { }


        // no other constructors, etc
        GenericHolderBase(void)                                       = delete;
        GenericHolderBase(const GenericHolderBase & oph)              = delete;
        GenericHolderBase(GenericHolderBase && oph)                   = delete;
        GenericHolderBase & operator=(const GenericHolderBase & oph)  = delete;
        GenericHolderBase & operator=(GenericHolderBase && oph)       = delete;
        virtual ~GenericHolderBase()                                  = default;


        /*! Return a const reference to the underlying data
         *
         * \exnothrow
         *
         * \return A const reference to the underlying data
         */ 
        std::shared_ptr<const T> get(void) const noexcept
        {
            return obj;
        }


        ////////////////////////////////////////
        // Virtual functions from GenericBase
        ////////////////////////////////////////
        virtual const char * type(void) const noexcept
        {
            return typeid(T).name();
        }

        virtual const std::type_info & type_info(void) const noexcept
        {
            return typeid(T);
        }

        
        virtual std::string demangled_type(void) const
        {
            return util::demangle_cpp_or_py_type(*obj);
        }


    private:
        //! The actual data
        std::shared_ptr<const T> obj;
};



template<typename T>
class GenericHolder<T, false> : public GenericHolderBase<T>
{
    public:
        using GenericHolderBase<T>::GenericHolderBase;

        virtual bool is_serializable(void) const
        {
            return false;
        }
};


template<typename T>
class GenericHolder<T, true> : public GenericHolderBase<T>
{
    public:
        using GenericHolderBase<T>::GenericHolderBase;

        virtual bool is_serializable(void) const
        {
            return true;
        }
};



} //closing namespace detail
} //closing namespace datastore
} //closing namespace pulsar


#endif
