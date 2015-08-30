#ifndef PROPERTY_MAP
#define PROPERTY_MAP

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


        bool Has(const std::string & key) const;

        std::string GetType(const std::string & key) const;

        std::vector<std::string> GetKeys(void) const;

        size_t Size(void) const;


    protected:  // to be exposed selectively by derived classes
        template<typename T>
        const T & GetRef(const std::string & key) const
        {
            const PropHolder<T> * ph = GetOrThrow_Cast_<T>(key);
            return ph->GetRef();
        }


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

            \todo Serialization does not work
         */
        class PropPlaceholder
        {
            public:
                //! Returns a pointer to a copy of this object
                virtual PropPlaceholder * Clone(void) const = 0;
                virtual ~PropPlaceholder() { }

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

