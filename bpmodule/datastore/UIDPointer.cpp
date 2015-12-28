/*! \file
 *
 * \brief A smart pointer that keeps a unique ID (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#include <atomic>
#include <cstddef> // for size_t

namespace bpmodule {
namespace datastore {
namespace detail {

std::atomic<size_t> curid_(0);

}
}
}
