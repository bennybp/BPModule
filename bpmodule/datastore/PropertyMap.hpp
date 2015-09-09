#ifndef _GUARD_PROPERTYMAP_HPP_
#define _GUARD_PROPERTYMAP_HPP_

#include <map>
#include <string>
#include <memory>

#include "bpmodule/python_helper/BoostPython_fwd.hpp"
#include "bpmodule/exception/GeneralException.hpp"


namespace bpmodule {
namespace datastore {

class PropertyMap
{
    public:
        PropertyMap(void) = default;
        ~PropertyMap(void) = default;

        PropertyMap(const PropertyMap & rhs) = default;
        PropertyMap & operator=(const PropertyMap & rhs) = default;

        PropertyMap(PropertyMap && rhs) = default;
        PropertyMap & operator=(PropertyMap && rhs) = default;


        /*! \brief Determine if this object contains data for a key
         *
         * \throw Should never throw, but can't guarantee that.
         *
         * \exstrong
         *
         * \param key The key to the data
         * \return True if the key exists, false otherwise
         */
        bool Has(const std::string & key) const;



        /*! \brief Determine if this object contains data of a specific type for a key
         *
         * \throw Should never throw, but can't guarantee that.
         *
         * \exstrong
         *
         * \tparam T Type to compare to
         *
         * \param key The key to the data
         * \return True if the key exists, false otherwise
         */
        template<typename T>
        bool Has(const std::string & key) const
        {
            if(!Has(key))
                return false;

            return GetOrThrow_(key).IsType<T>();
        }



        /*! \brief Get a string representing the type for a given key
         *
         * \throw bpmodule::exception::GeneralException
         *        if data doesn't exist for this key 
         *
         * \exstrong
         *
         * \return A string representing the type for a key
         */
        std::string GetType(const std::string & key) const;

        std::vector<std::string> GetKeys(void) const;

        size_t Size(void) const;


    protected:  // to be exposed selectively by derived classes
        /*! \brief Return a const reference to the underlying data
         *
         * \throw bpmodule::exception::GeneralException
         *        if data doesn't exist for this key or 
         *        is of the wrong type
         *
         * \tparam T The type of the data
         *
         * \param key The key to the data
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
         *        if data doesn't exist for this key or 
         *        is of the wrong type
         *
         * \tparam T The type of the data
         *
         * \param key The key to the data
         * \return A copy of the data
         */
        template<typename T>
        T GetCopy(const std::string & key) const
        {
            return GetRef<T>(key);
        }


        template<typename T>
        void Set(const std::string & key, const T & value)
        {
            auto v = PropPlaceholderPtr(new PropHolder<T>(value));
            Set_(key, std::move(v));
        }


        template<typename T>
        void Take(const std::string & key, T && value)
        {
            auto v = PropPlaceholderPtr(new PropHolder<T>(std::move(value)));
            Set_(key, std::move(v));
        }
        

        // since data is read-only once added, it makes sense that
        // the other map can be const. Changing/replacing it here
        // will only change this map, not the other
        void SetRef(const PropertyMap & pm, const std::string & key, const std::string & newkey);
        void SetRef(const PropertyMap & pm, const std::string & key);

        size_t Erase(const std::string & key);


        // construct from a python list of tuples
        PropertyMap(const boost::python::list & olist);


    private:
        //! An interface to a templated class that can hold anything
        /*!
            This allows for use in containers, etc.
         */
        class PropPlaceholder
        {
            public:
                /*! \brief Returns a pointer to a copy of this object
                 *
                 * The memory is not managed, so you must remember to delete it.
                 */
                virtual PropPlaceholder * Clone(void) const = 0;


                virtual ~PropPlaceholder() { }


                //! \brief Returns a string representing the type
                virtual const char * Type(void) const = 0;

                PropPlaceholder(void) = default;
                PropPlaceholder & operator=(const PropPlaceholder & rhs) = delete;
                PropPlaceholder & operator=(const PropPlaceholder && rhs) = delete;
                PropPlaceholder(const PropPlaceholder & rhs) = delete;
                PropPlaceholder(const PropPlaceholder && rhs) = delete;
        };


        typedef std::shared_ptr<PropPlaceholder> PropPlaceholderPtr;

        template<typename T>
        class PropHolder : public PropPlaceholder
        {
            public:
                PropHolder(const T & m) : obj(m) { }
                PropHolder(T && m) : obj(std::move(m)) { }

                ~PropHolder() = default;

                PropHolder(const PropHolder & oph) = delete;
                PropHolder(PropHolder && oph) = delete;
                PropHolder & operator=(const PropHolder & oph) = delete;
                PropHolder & operator=(PropHolder && oph) = delete;


                virtual PropHolder * Clone(void) const
                {
                    return new PropHolder<T>(obj);
                }

                T & GetRef(void)
                {
                    return obj;
                }

                const T & GetRef(void) const
                {
                    return obj;
                }

                const char * Type(void) const
                {
                    return typeid(T).name();
                }

                template<typename U>
                constexpr bool IsType(void) const
                {
                    return std::is_same<T, U>::value;
                }

            private:
                T obj;
        };


        struct PropMapEntry
        {
            // may be more added here in the future
            PropPlaceholderPtr value;
        };


        typedef std::map<std::string, PropMapEntry> PropMap;
        typedef PropMap::value_type PropMapValue;

        PropMap opmap_;


        const PropMapEntry & GetOrThrow_(const std::string & key) const;
        PropMapEntry & GetOrThrow_(const std::string & key);


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


        void Set_(const std::string & key, PropPlaceholderPtr && value);
        size_t Erase_(const std::string & key);

        // Creating a PropPlaceHolder from python object
        PropPlaceholderPtr PropPlaceholder_(const boost::python::object & value);

};


// specialize templates for python
template<>
boost::python::object PropertyMap::GetCopy<>(const std::string & key) const;

template<>
void PropertyMap::Set<>(const std::string & key, const boost::python::object & value);


} // close namespace datastore
} // close namespace bpmodule



#endif

