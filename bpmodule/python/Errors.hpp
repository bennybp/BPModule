/*! \file
 *
 * \brief Handling of python errors (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef _GUARD_ERRORS_HPP_
#define _GUARD_ERRORS_HPP_


namespace bpmodule {
namespace exception {
class GeneralException;
}
}


namespace bpmodule {
namespace python {
namespace detail {


/*! \brief Get the current python exception description as a string
 */
bpmodule::exception::GeneralException GetPyException(void);


} // close namespace detail
} // close namespace python
} // close namespace bpmodule

#endif
