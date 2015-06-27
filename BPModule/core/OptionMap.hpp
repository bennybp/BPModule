#ifndef OPTION_MAP
#define OPTION_MAP

#include <map>
#include <string>
#include <memory>

#include "BPModule/core/Exception.hpp"


namespace boost {
namespace python {
namespace api {

class object;

}

class list;

}
}




namespace bpmodule {



//! An interface to a templated class that can hold anything
/*!
    This allows for use in containers, etc.

    \todo Serialization does not work
 */
class OptionPlaceholder
{
    public:
        //! Returns a pointer to a copy of this object
        virtual OptionPlaceholder * Clone(void) const = 0;
        virtual ~OptionPlaceholder() { }

        virtual const char * Type(void) const = 0;

        OptionPlaceholder(void) = default;
        OptionPlaceholder & operator=(const OptionPlaceholder & rhs) = delete;
        OptionPlaceholder & operator=(const OptionPlaceholder && rhs) = delete;
        OptionPlaceholder(const OptionPlaceholder & rhs) = delete;
        OptionPlaceholder(const OptionPlaceholder && rhs) = delete;
};



template<typename T>
class OptionHolder : public OptionPlaceholder
{
    public:
        OptionHolder(const T & m) : obj(m) { }
        OptionHolder(T && m) : obj(std::move(m)) { }
        OptionHolder(const OptionHolder & oph) : obj(oph.obj) {  };

        OptionHolder & operator=(const OptionHolder & oph) = delete;
        OptionHolder & operator=(OptionHolder && oph) = delete;
        OptionHolder(OptionHolder && oph) = default;

        virtual OptionHolder * Clone(void) const
        {
            return new OptionHolder<T>(*this);
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



class OptionMap
{
    public:
        OptionMap(void) = default;
        ~OptionMap(void) = default;

        OptionMap(OptionMap && rhs) = default;
        OptionMap & operator=(OptionMap && rhs) = default;

        OptionMap(const OptionMap & rhs);
        OptionMap & operator=(const OptionMap & rhs);

        template<typename T>
        T Get(const std::string & key) const
        {
            const OpMapEntry & opm = GetOrThrow_(key);
            const OptionHolder<T> * oh = dynamic_cast<const OptionHolder<T> *>(opm.value.get());
            if(oh == nullptr)
                throw MapException("OptionMap", key, opm.value->Type(), typeid(T).name());

            return oh->GetRef();
        }



        template<typename T>
        void Change(const std::string & key, const T & value)
        {
            auto v = std::unique_ptr<OptionPlaceholder>(new OptionHolder<T>(value));
            Change_(key, std::move(v));
        }



        bool Has(const std::string & key) const;

        const std::string GetHelp(const std::string & key) const;

        std::string GetType(const std::string & key) const;

        std::vector<std::string> GetKeys(void) const;

        size_t Size(void) const;



        // For python
        OptionMap(const boost::python::list & olist);
        


    private:
        struct OpMapEntry
        {
            bool changed;
            std::unique_ptr<OptionPlaceholder> value;
            std::string help;
        };

        typedef std::map<std::string, OpMapEntry> OpMap;
        typedef OpMap::value_type OpMapValue;


        OpMap opmap_;

        size_t Erase_(const std::string & key);

        const OpMapEntry & GetOrThrow_(const std::string & key) const;
        OpMapEntry & GetOrThrow_(const std::string & key);

        void Change_(const std::string & key, std::unique_ptr<OptionPlaceholder> && value);
        void InitDefault_(const std::string & key, std::unique_ptr<OptionPlaceholder> && def, const std::string & help);

        // for python
        std::unique_ptr<OptionPlaceholder> OptionPlaceholder_(const boost::python::api::object & value);
};


// specialize the templates
template<>
boost::python::api::object OptionMap::Get<>(const std::string & key) const;

template<>
void OptionMap::Change<>(const std::string & key, const boost::python::api::object & value);


} // close namespace bpmodule



#endif

