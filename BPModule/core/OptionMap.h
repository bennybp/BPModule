#ifndef OPTION_MAP
#define OPTION_MAP

#include <map>
#include <string>

#include "BPModule/core/Exception.h"

#include <boost/lexical_cast.hpp>


namespace bpmodule {

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
        //! \todo make invalid cast exception class
        const OpMapEntry & opm = GetOrThrow(key);
        const OptionHolder<T> * oh = dynamic_cast<const OptionHolder<T> *>(opm.oph.get());
        if(oh == nullptr)
            throw MapException("OptionMap", key, opm.oph->Type(), typeid(T).name());

        return oh->GetRef();
    }

    template<typename T>
    void Set(const std::string & key, const T & value)
    {
        // copy existing help if it exists
        std::string help;
        if(Has(key))
            help = GetHelp(key);

        Erase(key);
        Set(key, value, help);
    }

    template<typename T>
    void Set(const std::string & key, const T & value, const std::string & help)
    {
        Erase(key);
        opmap_.insert(OpMapValue(
                                 key, 
                                 OpMapEntry{
                                            std::unique_ptr<OptionPlaceholder>(new OptionHolder<T>(value)),
                                            help
                                            }
                                )); 
    }

    bool Has(const std::string & key) const;

    std::string 
    GetHelp(const std::string & key) const;

    std::string GetType(const std::string & key) const;

    std::map<std::string, std::string>
    GetAllHelp(void) const;

    std::vector<std::string>
    GetKeys(void) const;

    size_t Size(void) const;


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
        OptionHolder(T && m) : obj(std::move(m)) { }
        OptionHolder(const OptionHolder & oph) : obj(oph.obj) {  };
    
        virtual OptionHolder * Clone(void) const { return new OptionHolder<T>(*this); }

        T & GetRef(void) { return obj; }
        const T & GetRef(void) const { return obj; }

        virtual const char * Type(void) const { return typeid(T).name(); }

        OptionHolder & operator=(const OptionHolder & oph) = delete;
        OptionHolder & operator=(OptionHolder && oph) = delete;
        OptionHolder(OptionHolder && oph) = default;

    private:
        T obj;
    };


    struct OpMapEntry
    {
        std::unique_ptr<OptionPlaceholder> oph;
        std::string help;
    };

    typedef std::map<std::string, OpMapEntry> OpMap;
    typedef OpMap::value_type OpMapValue;


    OpMap opmap_;

    size_t Erase(const std::string & key);

    const OpMapEntry & GetOrThrow(const std::string & key) const;


};

} // close namespace bpmodule


#endif

