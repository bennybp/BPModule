/*! \file
 *
 * \brief Forward declarations of some boost::python objects
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef _GUARD_BOOSTPYTHON_FWD_HPP_
#define _GUARD_BOOSTPYTHON_FWD_HPP_

namespace boost {
namespace python {

namespace api {
class object;
}

using api::object;
class list;
class dict;

}
}


namespace bpmodule {
namespace python_helper {

enum class PythonType;

}
}


#endif
