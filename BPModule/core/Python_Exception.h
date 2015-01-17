#ifndef PYTHON_EXCEPTION_H
#define PYTHON_EXCEPTION_H

#include "BPModule/code/Exception.h"

#include <boost/python.hpp>


namespace bpmodule {

class PythonConversionException : public BPModuleException
{
public:

    enum class Type
    {
        MissingKey,
        InvalidCast
    };

    MapException(std::string mapname, std::string key) noexcept
            : type_(Type::MissingKey), mapname_(std::move(mapname)), key_(std::move(key))
    { }

    MapException(std::string mapname, std::string key, std::string castfrom, std::string castto) noexcept
            : type_(Type::InvalidCast), mapname_(std::move(mapname)), key_(std::move(key)),
              castfrom_(castfrom), castto_(castto)
    { }

    const char * what(void) const noexcept
    {
        switch(type_)
        {
            case Type::MissingKey:
                return "Missing key";
            case Type::InvalidCast:
                return "Invalid cast";
            default:
                return "UNKNOWN";
        }
    }

    virtual ExceptionInfo GetInfo(void) const
    {
        ExceptionInfo ex(4);
        ex[0] = ExceptionInfoPair("Mapname", mapname_);
        ex[1] = ExceptionInfoPair("Key", key_);
        ex[2] = ExceptionInfoPair("Cast from", castfrom_);
        ex[3] = ExceptionInfoPair("Cast to", castto_);
        return ex;
    }

private:
    Type type_;
    std::string mapname_;
    std::string key_;
    std::string castfrom_, castto_; // for invalid cast
};


} // close namespace bpmodule
#endif
