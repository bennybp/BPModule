#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <stdexcept>

#include <fstream>
#include <vector>
#include <string>
#include <utility>



namespace bpmodule {

class BPModuleException : public std::exception
{
public:
    typedef std::pair<std::string, std::string> ExceptionInfoPair;
    typedef std::vector<ExceptionInfoPair> ExceptionInfo;
    virtual ExceptionInfo GetInfo(void) const = 0;

    virtual ~BPModuleException() { };
};


class BPOutputException : public BPModuleException
{
public:
    enum class Type
    {
        NullPtr,
        BadFile
    };

    BPOutputException(std::string filepath, const std::ofstream & of)
                : type_(Type::BadFile),
                  filepath(std::move(filepath)),
                  isopen_(of.is_open()), 
                  badbit_(of.bad()), 
                  eofbit_(of.eof()),
                  failbit_(of.fail())
    {
    }

    BPOutputException() // nullptr
                : type_(Type::NullPtr)
    {
    }

    const char * what(void) const noexcept
    {
        switch(type_)
        {
            case Type::BadFile:
                return "Bad file";
            default:
                return "UNKNOWN";
        }
    }

    virtual ExceptionInfo GetInfo(void) const
    {
        if(type_ == Type::BadFile)
        {
            ExceptionInfo ex(5);
            ex[0] = ExceptionInfoPair("File", filepath);
            ex[1] = ExceptionInfoPair("IsOpen", isopen_ ? "1" : "0");
            ex[2] = ExceptionInfoPair("Badbit", badbit_ ? "1" : "0");
            ex[3] = ExceptionInfoPair("EOFbit", eofbit_ ? "1" : "0");
            ex[4] = ExceptionInfoPair("Failbit", failbit_ ? "1" : "0");
            return ex;
        }
        else
            return ExceptionInfo();
    }

private:
    Type type_;
    std::string filepath;
    bool isopen_, badbit_, eofbit_, failbit_;
 
};



class MapException : public BPModuleException
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


class ModuleException : public BPModuleException
{
public:
    enum class Type
    {
        MissingFile,
        MissingFunction,
        BadKey
    };

    // for missing file
    ModuleException(std::string filename)
                : type_(Type::MissingFile), str1_(filename)
    {
    }

    ModuleException(Type type, std::string str1, std::string str2)
                : type_(type), str1_(str1), str2_(str2)
    {
    }

    const char * what(void) const noexcept
    {
        switch(type_)
        {
            case Type::MissingFile:
                return "Missing file";
            case Type::MissingFunction:
                return "Missing function";
            case Type::BadKey:
                return "Bad key";
            default:
                return "UNKNOWN";
        }
    }

    virtual ExceptionInfo GetInfo(void) const
    {
        ExceptionInfo ex(2);

        switch(type_)
        {
            case Type::MissingFile:
                ex[0] = ExceptionInfoPair("File", str1_);
                break;
            case Type::MissingFunction:
                ex[0] = ExceptionInfoPair("File", str1_);
                ex[1] = ExceptionInfoPair("Function", str2_);
                break;
            case Type::BadKey:
                ex[0] = ExceptionInfoPair("File", str1_);
                ex[1] = ExceptionInfoPair("Key", str2_);
                break;
            default:
                break;
        }
        
        return ex;
    }

private:
    Type type_;
    std::string str1_;
    std::string str2_;
};

} // close namespace bpmodule
#endif
