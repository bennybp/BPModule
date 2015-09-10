/*! \file
 *
 * \brief Storage of generic data via (key, value) pair (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef _GUARD_PROPERTYMAP_HPP_
#define _GUARD_PROPERTYMAP_HPP_

#include <map>
#include <memory>
#include <cstring> // for strcmp

#include "bpmodule/python_helper/BoostPython_fwd.hpp"
#include "bpmodule/exception/GeneralException.hpp"


namespace bpmodule {
namespace datastore {


/*! \brief A mapping of strings to arbirtrary data with some copy-on-write semantics
 *
 * A PropertyMap can store an arbitrary data type, as long that data type
 * has a copy and move constructor. Once stored, the data itself is immutable.
 *
 * Multiple PropertyMap can store references to the same data.
 * Since the data itself is read-only, modifications in one map cannot affect
 * another. Erasing or replacing the data in one map leaves the references
 * in any other maps referencing this data untouched.
 */
class PropertyMap
{
    public:
        PropertyMap(void)  = default;
        ~PropertyMap(void) = default;


        /*! \brief Copies a property map
         * 
         * Underlying data is NOT copied, rather just the references.
         *
         * \param [in] rhs The object to copy
         */
        PropertyMap(const PropertyMap & rhs)             = default;



        /*! \brief Moves the contents of another object to this
         * 
         * References to data are removed from the other object.
         *
         * \param [in] rhs The object to copy
         */ 
        PropertyMap(PropertyMap && rhs)                  = default;



        /*! \brief Assigns the contents of another object to this
         *
         * \copydetails PropertyMap::PropertyMap(const PropertyMap &)
         */ 
        PropertyMap & operator=(const PropertyMap & rhs) = default;



        /*! \brief Moves the contents of another object to this
         *
         * \copydetails PropertyMap::PropertyMap(PropertyMap &&)
         */ 
        PropertyMap & operator=(PropertyMap && rhs)      = default;



        /*! \brief Construct via python list
         * 
         * \todo Wrap or document python exceptions
         *
         * \throw bpmodule::exception::GeneralException if there is a problem (type conversion, etc).
         *
         * \param [in] olist A python list containing objects to copy into this map
         */  
        PropertyMap(const boost::python::list & olist);



        /*! \brief Determine if this object contains data for a key
         *
         * \param [in] key The key to the data
         * \return True if the key exists, false otherwise
         */
        bool Has(const std::string & key) const;



        /*! \brief Determine if this object contains data of a specific type for a key
         *
         * \tparam T Type to compare to
         *
         * \param [in] key The key to the data
         * \return True if the key exists, false otherwise
         */
        template<typename T>
        bool HasType(const std::string & key) const
        {
            if(!Has(key))
                return false;

            return GetOrThrow_(key).value->IsType<T>();
        }



        /*! \brief Get a string representing the type for a given key
         *
         * \throw bpmodule::exception::GeneralException
         *        if the key doesn't exist 
         *
         * \param [in] key The key to the data
         * \return A string representing the type for a key
         */
        std::string GetType(const std::string & key) const;



        /*! \brief Obtain all the keys contained in this object
         * 
         * \return A vector of strings containing all the keys
         */
        std::vector<std::string> GetKeys(void) const;



        /*! \brief Return the number of elements contained
         *
         * \exnothrow
         *
         * \return Number of elements in this container
         */
        size_t Size(void) const noexcept;



    protected:
        /////////////////////////////////////////////////
        // to be exposed selectively by derived classes
        /////////////////////////////////////////////////

        /*! \brief Return a const reference to the underlying data
         *
         * \throw bpmodule::exception::GeneralException
         *        if the key doesn't exist or 
         *        is of the wrong type
         *
         * \tparam T The type of the data
         *
         * \param [in] key The key to the data
         * \return A const referance to the data
         */
        template<typename T>
        const T & GetRef(const std::string & key) const
        {
            const PropHolder<T> * ph = GetOrThrow_Cast_<T>(key);
            return ph->GetRef();
        }



        /*! \brief Return a copy of the underlying data
         *
         * \throw bpmodule::exception::GeneralException
         *        if the key doesn't exist or 
         *        is of the wrong type
         *
         * \tparam T The type of the data
         *
         * \param [in] key The key to the data
         * \return A copy of the data
         */
        template<typename T>
        T GetCopy(const std::string & key) const
        {
            return GetRef<T>(key);
        }



        /*! \brief Set the data associated with a given key via copy
         * 
         * If the key exists, the data is overwritten.
         * Then, the data itself may remain if another PropertyMap contains the same data
         * (set via SetRef, etc).
         *
         * \exstrong
         *
         * \tparam T The type of the data
         *
         * \param [in] key The key to the data
         * \param [in] value The data to store
         */
        template<typename T>
        void Set(const std::string & key, const T & value)
        {
            auto v = PropPlaceholderPtr(new PropHolder<T>(value));
            Set_(key, std::move(v));
        }



        /*! \brief Set the data associated with a given key via move semantics
         * 
         * \copydetails Set
         */
        template<typename T>
        void Take(const std::string & key, T && value)
        {
            auto v = PropPlaceholderPtr(new PropHolder<T>(std::move(value)));
            Set_(key, std::move(v));
        }
        

        /*! \brief Store the same data as found in another object
         * 
         * This function does not copy or take the actual data. Rather, only a
         * pointer is copied. Erasing or replacing data in this map does not
         * affect the other map. If the key already exists in this map, it is
         * replaced.
         *
         * \throw bpmodule::exception::GeneralException if the key is not found in the other map or
         *        another error occurs.
         *
         * \exstrong
         *
         * \param [in] other The other object from which to get the data
         * \param [in] key The key of the object in the other object
         */
        void SetRef(const PropertyMap & other, const std::string & key);



        /*! \copydoc SetRef
         * \param [in] newkey The key under which to store the data in this object
         */
        void SetRef(const PropertyMap & other, const std::string & key, const std::string & newkey);



        /*! \brief Remove a key from this data store
         * 
         * The data itself may remain if another PropertyMap contains the same data
         * (set via SetRef, etc).
         *
         * The key does not have to exist. If the key doesn't exists, nothing will happen.
         *
         * \exstrong
         *
         * \param [in] key The key to the data
         * \return The number of elements removed
         */
        size_t Erase(const std::string & key);





    private:
        /*! \brief An interface to a templated class that can hold anything
         *
         *  This allows for use in containers, etc.
         */
        class PropPlaceholder
        {
            public:
                PropPlaceholder(void) noexcept         = default;
                virtual ~PropPlaceholder() noexcept    = default;

                PropPlaceholder & operator=(const PropPlaceholder & rhs)  = delete;
                PropPlaceholder & operator=(const PropPlaceholder && rhs) = delete;
                PropPlaceholder(const PropPlaceholder & rhs)              = delete;
                PropPlaceholder(const PropPlaceholder && rhs)             = delete;



                /*! \brief Returns a string representing the type
                 *
                 * \exnothrow
                 *
                 * \return A string representing the type (obtained via typeid().name())
                 */
                virtual const char * Type(void) const noexcept = 0;



                /*! \brief Determines if the contained type matches a given type
                 *
                 * \exnothrow
                 *
                 * \tparam U The type to compare to
                 *
                 * \return True if the contained object is of type U, false otherwise
                 */ 
                template<typename U>
                bool IsType(void) const noexcept
                {
                    return (strcmp(typeid(U).name(), Type()) == 0);
                }
        };





        /*! \brief A container that can hold anything
         *
         * \tparam T The type of the data this object is holding
         */ 
        template<typename T>
        class PropHolder : public PropPlaceholder
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
                PropHolder(const T & m) : obj(m) { }


                /*! \brief Construct via moving a data object
                 * 
                 * Will invoke move constructor for type T
                 *
                 * \throwno Throws an exception only if the move
                 *          constructor for T throws an exception
                 *
                 * \param [in] m The object to move
                 */
                PropHolder(T && m) : obj(std::move(m)) { }


                // no other constructors, etc
                PropHolder(void)                                = delete;
                PropHolder(const PropHolder & oph)              = delete;
                PropHolder(PropHolder && oph)                   = delete;
                PropHolder & operator=(const PropHolder & oph)  = delete;
                PropHolder & operator=(PropHolder && oph)       = delete;
                virtual ~PropHolder()                           = default;


                /*! Return a reference to the underlying data
                 *
                 * \exnothrow
                 *
                 * \return A reference to the underlying data
                 */ 
                T & GetRef(void) noexcept
                {
                    return obj;
                }


                /*! Return a const reference to the underlying data
                 *
                 * \exnothrow
                 *
                 * \return A const reference to the underlying data
                 */ 
                const T & GetRef(void) const noexcept
                {
                    return obj;
                }


                virtual constexpr const char * Type(void) const noexcept
                {
                    return typeid(T).name();
                }


            private:
                //! The actual data
                T obj;
        };




        ////////////////////////////////
        // Actual storage of the data //
        ////////////////////////////////

        //! A pointer to a generic object
        typedef std::shared_ptr<PropPlaceholder> PropPlaceholderPtr;


        /*! \brief Stores a pointer to a placeholder, plus some other information
         *
         * May be expanded in the future. 
         */
        struct PropMapEntry
        {
            // may be more added here in the future
            PropPlaceholderPtr value;
        };



        //! The container to use to store the data
        typedef std::map<std::string, PropMapEntry> PropMap;


        //! Map actually containing the data
        PropMap opmap_;




        ////////////////////////////////
        // Private functions          //
        ////////////////////////////////

        /*! \brief Obtains a PropMapEntry or throws if key doesn't exist
         * 
         * \throw bpmodule::exception::GeneralException if key doesn't exist
         *
         * \exstrong
         *
         * \param [in] key Key of the data to get
         * \return PropMapEntry containing the data for the given key
         */ 
        PropMapEntry & GetOrThrow_(const std::string & key);



        //! \copydoc GetOrThrow_
        const PropMapEntry & GetOrThrow_(const std::string & key) const;




        /*! \brief Obtains a pointer to a PropHolder cast to desired type
         * 
         * \throw bpmodule::exception::GeneralException if key 
         *        doesn't exist or if the cast fails.
         *
         * \param [in] key Key of the data to get
         * \return PropHolder containing the data for the given key
         */ 
        template<typename T>
        const PropHolder<T> * GetOrThrow_Cast_(const std::string & key) const
        {
            const PropMapEntry & pme = GetOrThrow_(key);
            const PropHolder<T> * ph = dynamic_cast<const PropHolder<T> *>(pme.value.get());
            if(ph == nullptr)
                throw exception::GeneralException(
                                         "Bad cast",
                                         {
                                            { "Location", "PropertyMap" },
                                            { "From", pme.value->Type() },
                                            { "  To", typeid(T).name() }
                                         }
                                       );

            return ph;
        }



        /*! \brief Sets the data for a given key via a PropPlaceholderPtr
         * 
         * \exstrong
         *
         * \param [in] key Key of the data to set
         * \param [in] value Pointer to the data to set
         * \return PropHolder containing the data for the given key
         */ 
        void Set_(const std::string & key, PropPlaceholderPtr && value);



        /*! \brief Create a PropPlaceHolder from python object
         * 
         * \throw bpmodule::exception::GeneralException if there is a problem (type conversion, etc)
         *
         * \param [in] value A python object containing data to copy
         */ 
        static PropPlaceholderPtr PropPlaceholder_(const boost::python::object & value);

};


/// \copydoc GetCopy
template<>
boost::python::object PropertyMap::GetCopy<>(const std::string & key) const;



/// \copydoc Set
template<>
void PropertyMap::Set<>(const std::string & key, const boost::python::object & value);


} // close namespace datastore
} // close namespace bpmodule



#endif

