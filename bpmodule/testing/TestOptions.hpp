/*! \file
 *
 * \brief Various test for options (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef _GUARD_TESTOPTIONS_HPP_
#define _GUARD_TESTOPTIONS_HPP_

#include <string>


namespace bpmodule {
namespace testing {


/*! \brief Returns the limits of a type as a python tuple
 */ 
template<typename T>
boost::python::tuple Limits(void)
{ 
    return boost::python::make_tuple( 
                                      std::numeric_limits<T>::lowest(),
                                      std::numeric_limits<T>::max()
                                    );
}



} // close namespace testing
} // close namespace bpmodule



#endif
