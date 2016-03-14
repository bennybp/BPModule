/*! \file
 *
 * \brief Output tee to a string (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef BPMODULE_GUARD_OUTPUT__TEEBUFTOSTRING_HPP_
#define BPMODULE_GUARD_OUTPUT__TEEBUFTOSTRING_HPP_

#include <iostream>
#include <string>


namespace bpmodule {
namespace output {


/*! \brief Stream buffer that tees output to a string
 *
 * Output will go to the streambuf, as well as the given string
 */ 
class TeeBufToString : public std::streambuf
{
    public:
        /*! \brief Constructor
         *
         * \param [in] sb The streambuf to tee from
         * \param [in] str The string to copy output to
         */ 
        TeeBufToString(std::streambuf * sb, std::string * str) noexcept
            : sb_(sb), str_(str)
        { }

        TeeBufToString & operator=(TeeBufToString &&)      = default;
        TeeBufToString(TeeBufToString &&)                  = default;
        TeeBufToString(const TeeBufToString &)             = delete;
        TeeBufToString & operator=(const TeeBufToString &) = delete;

        void SetString(std::string * str) noexcept
        {
            str_ = str;
        }

    protected:
        virtual std::streamsize xsputn(const char * s, std::streamsize n)
        {
            std::streamsize n1 = sb_->sputn(s, n);

            if(str_ != nullptr)
                str_->append(s, n);
    
            return n1;
        }   
        
        virtual int overflow(int c)
        {
            if (c == EOF)
            {
                return !EOF;
            }   
            else
            {
                if(str_ != nullptr)
                    str_->append(1, static_cast<char>(c));
                return sb_->sputc(static_cast<char>(c));
            }   
        }   
        
        
        virtual int sync()
        {
            return sb_->pubsync();
        }   
        
    private:
        std::streambuf * sb_;
        std::string * str_;
};      


} // close namespace output
} // close namespace bpmodule


#endif
