#ifndef OPTION_MAP
#define OPTION_MAP

#include <unordered_map>
#include <string>

#include "BPModule/core/Exception.h"

#include <boost/lexical_cast.hpp>

// for friend
namespace bpmodule {
class OptionMap;
}

void swap(bpmodule::OptionMap & first, bpmodule::OptionMap & second);



namespace bpmodule {

class OptionMap
{
public:
    OptionMap(void) = default;
    ~OptionMap(void) = default;


    OptionMap(OptionMap && rhs);
    OptionMap & operator=(OptionMap rhs);
    OptionMap(const OptionMap & rhs);

    template<typename T>
    T Get(const std::string & key) const
    {
        //! \todo make invalid cast exception class
        const OpMapEntry & opm = GetOrThrow(key);
        const OptionHolder<T> * oh = dynamic_cast<const OptionHolder<T> *>(opm.oph);
        if(oh == nullptr)
            throw MapException("OptionMap", key, opm.oph->Type(), typeid(T).name());

        return oh->GetRef();
    }


    template<typename T>
    void Set(const std::string & key, const T & value, const std::string & help)
    {
        Erase(key);
        opmap_.insert(OpMapValue(key, OpMapEntry({new OptionHolder<T>(value), help}))); 
    }

    bool Has(const std::string & key) const;

    std::string GetHelp(const std::string & key) const;

    std::unordered_map<std::string, std::string> GetAllHelp(void) const;

    std::string GetValueStr(const std::string & key) const;

    std::unordered_map<std::string, std::string> GetAllValueStr(void) const;

    size_t Size(void) const;

    friend void ::swap(OptionMap & first, OptionMap & second);


private:

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
        virtual std::string ToString(void) const = 0;
        virtual const char * Type(void) const = 0;
        virtual ~OptionPlaceholder() { }
    
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
        OptionHolder(const OptionHolder<T> & g) : obj(g.obj) { }
    
        virtual OptionHolder * Clone(void) const { return new OptionHolder<T>(*this); }
        T & GetRef(void) { return obj; }
        const T & GetRef(void) const { return obj; }
        virtual std::string ToString(void) const { return boost::lexical_cast<std::string>(obj); }
        virtual const char * Type(void) const { return typeid(T).name(); }

    private:
        T obj;
    };


    struct OpMapEntry
    {
        OptionPlaceholder * oph;
        std::string help;
    };

    typedef std::unordered_map<std::string, OpMapEntry> OpMap;
    typedef OpMap::value_type OpMapValue;


    OpMap opmap_;

    size_t Erase(const std::string & key);

    const OpMapEntry & GetOrThrow(const std::string & key) const;


};

} // close namespace bpmodule


#endif

