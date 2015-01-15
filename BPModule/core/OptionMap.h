#ifndef OPTION_MAP
#define OPTION_MAP

#include <unordered_map>
#include <string>

#include "BPModule/core/Exception.h"

#include <boost/lexical_cast.hpp>

using boost::lexical_cast;

namespace bpmodule {

class OptionMap
{
public:
    OptionMap(void) = default;
    ~OptionMap(void) = default;

    friend void swap(OptionMap & first, OptionMap & second)
    {
        using std::swap;
        swap(first.opmap_, second.opmap_);
    } 

    OptionMap(OptionMap && rhs)
    {
        swap(*this, rhs);
    }

    // copy and swap
    OptionMap & operator=(OptionMap rhs)
    {
        swap(*this, rhs);
        return *this;
    }

    OptionMap(const OptionMap & rhs)
    {
        // need to Clone new elements
        for(auto & it : rhs.opmap_)
            opmap_.insert(OpMapValue(it.first, OpMapEntry({it.second.oph->Clone(), it.second.help})));
    }

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

    std::string GetString(const std::string & key)
    {
        return GetOrThrow(key).oph->ToString();
    }

    template<typename T>
    void Set(const std::string & key, const T & value, const std::string & help)
    {
        Erase(key);
        opmap_.insert(OpMapValue(key, OpMapEntry({new OptionHolder<T>(value), help}))); 
    }

    bool Has(const std::string & key) const
    {
        return opmap_.count(key);
    }

    std::unordered_map<std::string, std::pair<std::string, std::string>> Info(void) const
    {
        std::unordered_map<std::string, std::pair<std::string, std::string>> m;
        for(const auto & it : opmap_)
            m[it.first] = std::pair<std::string, std::string>(it.second.oph->ToString(), it.second.help);
        return m;
    }

    size_t Size(void) const
    {
        return opmap_.size();
    }


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


    size_t Erase(const std::string & key)
    {
        // delete pointer if necessary
        if(opmap_.count(key))
            delete opmap_.at(key).oph; // delete the ptr

        return opmap_.erase(key);       
    }




    const OpMapEntry & GetOrThrow(const std::string & key) const
    {
        if(opmap_.count(key))
            return opmap_.at(key);
        else
            throw MapException("OptionMap", key);
    }

};

} // close namespace bpmodule


#endif

