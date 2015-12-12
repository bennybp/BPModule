#ifndef _GUARD_ASSERT_HPP_
#define _GUARD_ASSERT_HPP_


namespace bpmodule {
namespace exception {

PRAGMA_WARNING_PUSH
PRAGMA_WARNING_IGNORE_UNUSED_PARAMETERS

/*! \brief Asserts that a condition is true, throwing an exception if it is not
 *
 * If \p condition is false, an exception of type EX is thrown.
 * The \p description and \p args are forwarded to the constructor
 * of the exception.
 */
template<typename EX, typename ... Targs>
void Assert(bool condition, const std::string & description, Targs &&... args)
{
    #ifndef NDEBUG
    if(!condition)
        throw EX(description, std::forward<Targs>(args)...);
    #endif
}

PRAGMA_WARNING_POP


} // close namespace exception
} // close namespace bpmodule

#endif

