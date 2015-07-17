#ifndef PROPERTY_MAP
#define PROPERTY_MAP

#include <map>
#include <string>
#include <memory>

#include "BPModule/core/BoostPython_fwd.hpp"
#include "BPModule/core/Exception.hpp"


namespace bpmodule {




class PropertyMap
{
    public:
        PropertyMap(void) = default;
        ~PropertyMap(void) = default;

        PropertyMap(const PropertyMap & rhs);
        PropertyMap & operator=(const PropertyMap & rhs);

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
        T & GetRef(const std::string & key)
        {
            PropHolder<T> * ph = GetOrThrow_Cast_<T>(key);
            return ph->GetRef();
        }

        template<typename T>
        T GetCopy(const std::string & key) const
        {
            return GetRef<T>(key);
        }


        template<typename T>
        void Add(const std::string & key, const T & value)
        {
            auto v = std::unique_ptr<PropPlaceholder>(new PropHolder<T>(value));
            Add_(key, std::move(v));
        }


        template<typename T>
        void Add(const std::string & key, T && value)
        {
            auto v = std::unique_ptr<PropPlaceholder>(new PropHolder<T>(std::move(value)));
            Add_(key, std::move(v));
        }
        

        template<typename T>
        void Change(const std::string & key, const T & value)
        {
            auto v = std::unique_ptr<PropPlaceholder>(new PropHolder<T>(value));
            Change_(key, std::move(v));
        }

        template<typename T>
        void Change(const std::string & key, T && value)
        {
            auto v = std::unique_ptr<PropPlaceholder>(new PropHolder<T>(std::move(value)));
            Change_(key, std::move(v));
        }


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
            std::unique_ptr<PropPlaceholder> value;
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
                throw BPModuleException(
                                         "Bad cast",
                                         {
                                            { "Location", "PropertyMap" },
                                            { "From", pme.value->Type() },
                                            { "  To", typeid(T).name() }
                                         }
                                       );

            return ph;
        }


        template<typename T>
        PropHolder<T> * GetOrThrow_Cast_(const std::string & key)
        {
            PropMapEntry & pme = GetOrThrow_(key);
            PropHolder<T> * ph = dynamic_cast<PropHolder<T> *>(pme.value.get());
            if(ph == nullptr)
                throw BPModuleException(
                                         "Bad cast",
                                         {
                                            { "Location", "PropertyMap" },
                                            { "From", pme.value->Type() },
                                            { "  To", typeid(T).name() }
                                         }
                                       );

            return ph;
        }


        void Add_(const std::string & key, std::unique_ptr<PropPlaceholder> && value);
        void Change_(const std::string & key, std::unique_ptr<PropPlaceholder> && value);
        size_t Erase_(const std::string & key);

        // Creating a PropPlaceHolder from python object
        std::unique_ptr<PropPlaceholder> PropPlaceholder_(const boost::python::api::object & value);

};


// specialize templates for python
template<>
boost::python::api::object PropertyMap::GetCopy<>(const std::string & key) const;

template<>
void PropertyMap::Add<>(const std::string & key, const boost::python::api::object & value);

template<>
void PropertyMap::Change<>(const std::string & key, const boost::python::api::object & value);


} // close namespace bpmodule



#endif

