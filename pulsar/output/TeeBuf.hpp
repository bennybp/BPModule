/*! \file
 *
 * \brief Output tee to a string (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef PULSAR_GUARD_OUTPUT__TEEBUF_HPP_
#define PULSAR_GUARD_OUTPUT__TEEBUF_HPP_

#include <iostream>
#include <string>
#include <stdexcept>

namespace pulsar{

/*! \brief Stream buffer that tees output to two ostreams
 *
 * Output will go to both stream buffers
 *
 * If a particular stream buffer is null, no output is written to that stream
 */ 
class TeeBuf : public std::streambuf
{
    public:
        /*! \brief Constructor
         */ 
        TeeBuf(std::streambuf * sb1, std::streambuf * sb2) noexcept
            : sb1_(sb1), sb2_(sb2)
        {
        }

        TeeBuf & operator=(TeeBuf &&)      = default;
        TeeBuf(TeeBuf &&)                  = default;
        TeeBuf(const TeeBuf &)             = delete;
        TeeBuf & operator=(const TeeBuf &) = delete;


    protected:
        virtual std::streamsize xsputn(const char * s, std::streamsize n)
        {
            if(sb1_ == nullptr && sb2_ == nullptr)
                return n;

            std::streamsize n1 = n;
            std::streamsize n2 = n;

            if(sb1_ != nullptr)
                n1 = sb1_->sputn(s, n);
            if(sb2_ != nullptr)
                n2 = sb2_->sputn(s, n);

            if(sb1_ != nullptr && n1 != n)
                throw std::runtime_error("Bad return from first stream buffer");

            if(sb2_ != nullptr && n2 != n)
                throw std::runtime_error("Bad return from second stream buffer");

            return n;
        }   
        
        virtual int overflow(int c)
        {
            if (c == EOF)
                return !EOF;
            else
            {
                int ret1 = 0;
                int ret2 = 0;

                if(sb1_)
                    ret1 = sb1_->sputc(static_cast<char>(c));

                if(sb2_)
                    ret2 = sb2_->sputc(static_cast<char>(c));

                if(sb1_ && sb2_ && ret1 != ret2)
                    throw std::runtime_error("Different return values in TeeBufToString::overflow");

                return ret1;
            }   
        }   
        
        virtual int sync()
        {
            int ret1 = 0;
            int ret2 = 0;

            if(sb1_)
                ret1 = sb1_->pubsync();

            if(sb2_)
                ret2 = sb2_->pubsync();

            if(sb1_ && sb2_ && ret1 != ret2)
                throw std::runtime_error("Different return values in TeeBufToString::sync");

            return ret1;
        }   
        
    private:
        std::streambuf * sb1_, * sb2_;
};      

} // close namespace pulsar


#endif
