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

        PropertyMap(PropertyMap && rhs) = default;
        PropertyMap & operator=(PropertyMap && rhs) = default;

        PropertyMap(const PropertyMap & rhs);
        PropertyMap & operator=(const PropertyMap & rhs);

        template<typename T>
        T Get(const std::string & key) const
        {
            const PropMapEntry & opm = GetOrThrow_(key);
            const PropHolder<T> * oh = dynamic_cast<const PropHolder<T> *>(opm.value.get());
            if(oh == nullptr)
                throw BPModuleException(
                                         "Bad cast",
                                         {
                                            { "Location", "PropertyMap" },
                                            { "From", opm.value->Type() },
                                            { "  To", typeid(T).name() }
                                         }
                                       );

            return oh->GetRef();
        }



        template<typename T>
        void Change(const std::string & key, const T & value)
        {
            auto v = std::unique_ptr<PropPlaceholder>(new PropHolder<T>(value));
            Change_(key, std::move(v));
        }



        bool Has(const std::string & key) const;

        std::string GetType(const std::string & key) const;

        std::vector<std::string> GetKeys(void) const;

        size_t Size(void) const;



        // For python
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
                PropHolder(const PropHolder & oph) : obj(oph.obj) {  };

                PropHolder & operator=(const PropHolder & oph) = delete;
                PropHolder & operator=(PropHolder && oph) = delete;
                PropHolder(PropHolder && oph) = default;

                virtual PropHolder * Clone(void) const
                {
                    return new PropHolder<T>(*this);
                }

                T & GetRef(void)
                {
                    return obj;
                }

                const T & GetRef(void) const
                {
                    return obj;
                }

                T & Get(void) const
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
            bool changed;
            std::unique_ptr<PropPlaceholder> value;
        };

        typedef std::map<std::string, PropMapEntry> PropMap;
        typedef PropMap::value_type PropMapValue;


        PropMap opmap_;

        size_t Erase_(const std::string & key);

        const PropMapEntry & GetOrThrow_(const std::string & key) const;
        PropMapEntry & GetOrThrow_(const std::string & key);

        void Change_(const std::string & key, std::unique_ptr<PropPlaceholder> && value);
        void InitDefault_(const std::string & key, std::unique_ptr<PropPlaceholder> && def);

        // for initializing from python
        std::unique_ptr<PropPlaceholder> PropPlaceholder_(const boost::python::api::object & value);
};


// specialize the templates
template<>
boost::python::api::object PropertyMap::Get<>(const std::string & key) const;

template<>
void PropertyMap::Change<>(const std::string & key, const boost::python::api::object & value);



// Some semantic typedefs
typedef PropertyMap OptionMap;



} // close namespace bpmodule



#endif

