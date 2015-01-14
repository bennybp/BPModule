#ifndef OPTION_MAP
#define OPTION_MAP

#include <unordered_map>
#include <string>

#include <boost/lexical_cast.hpp>

using boost::lexical_cast;

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

    //! Returns a pointer to a copy of this object
    virtual OptionHolder * Clone(void) const { return new OptionHolder<T>(*this); }

    //! Returns a reference to the data
    T & GetRef(void) { return obj; }

    //! Returns a reference to the data
    const T & GetRef(void) const { return obj; }

    virtual std::string ToString(void) const { return boost::lexical_cast<std::string>(obj); }

private:
    T obj;
};



class OptionMap
{
public:
    OptionMap(void) = default;
    ~OptionMap(void) = default;

    OptionMap(OptionMap && rhs)
    {
        Swap(*this, rhs);
    }

    // copy and swap
    OptionMap & operator=(OptionMap rhs)
    {
        Swap(*this, rhs);
        return *this;
    }

    OptionMap(const OptionMap & rhs)
    {
        // need to Clone new elements
        for(auto & it : rhs.opmap_)
            opmap_[it.first] = it.second->Clone();

        // copy the help
        help_ = rhs.help_;
    }

    template<typename T>
    T Get(const std::string & key) const
    {
        //! \todo make invalid cast exception class
        const OptionPlaceholder * oph = opmap_.at(key);
        const OptionHolder<T> * oh = dynamic_cast<const OptionHolder<T> *>(oph);
        if(oh == nullptr)
            throw std::runtime_error("Invalid cast");
        return oh->GetRef();
    }

    std::string GetString(const std::string & key)
    {
        return opmap_.at(key)->ToString();
    }

    template<typename T>
    void Set(const std::string & key, const T & value, const std::string & help)
    {
        Erase(key);
        opmap_[key] = new OptionHolder<T>(value); 
        help_[key] = help;
    }

    bool Has(const std::string & key) const
    {
        return opmap_.count(key);
    }

    std::unordered_map<std::string, std::pair<std::string, std::string>> Dump(void) const
    {
        std::unordered_map<std::string, std::pair<std::string, std::string>> m;
        for(const auto & it : opmap_)
            m[it.first] = std::pair<std::string, std::string>(it.second->ToString(), help_.at(it.first));
        return m;
    }

    size_t Size(void) const
    {
        return opmap_.size();
    }

private:
    std::unordered_map<std::string, OptionPlaceholder *> opmap_;
    std::unordered_map<std::string, std::string> help_;


    void Merge(const OptionMap & rhs)
    {
        for(auto & it : rhs.opmap_)
        {
            Erase(it.first);
            opmap_[it.first] = it.second->Clone();
        }
    }

    size_t Erase(const std::string & key)
    {
        // erase any previously stored
        if(opmap_.count(key))
            delete opmap_[key]; // delete the ptr

        help_.erase(key);
        return opmap_.erase(key);       
    }


    friend void Swap(OptionMap & first, OptionMap & second)
    {
        using std::swap;
        swap(first.opmap_, second.opmap_);
        swap(first.help_, second.help_);
    } 


};


#endif
