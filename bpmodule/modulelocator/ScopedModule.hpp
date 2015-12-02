/*! \file
 *
 * \brief An RAII wrapper around a module pointer
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef _GUARD_SCOPEDMODULE_HPP_
#define _GUARD_SCOPEDMODULE_HPP_

#include <memory>

// forward declarations
namespace boost
{
template<typename T>
class shared_ptr;
}


namespace bpmodule {

namespace modulebase {
class ModuleBase;
}

namespace modulelocator {
class ModuleLocator;


} // close modulelocator
} // close bpmodule

// end forward declarations


namespace bpmodule {
namespace modulelocator {

//template<typename T>
//using ScopedModule = std::unique_ptr<T, std::function<void(modulebase::ModuleBase *)>>;


/*! \brief Contains a module that automatically deletes the object when leaving scope
 *
 * This is derived from unique_ptr, but simplified slightly, and with a custom
 * deletion function. In addition,
 * safeguards are put in place to prevent manipulating the raw pointer
 *
 * \todo Should be expanded with other functions
 * \todo Exception safety, noexcept, and related documentation
 */
template<typename T>
class ScopedModule : private std::unique_ptr<T, std::function<void(modulebase::ModuleBase *)>>
{
    public:
        typedef std::unique_ptr<T, std::function<void(modulebase::ModuleBase *)>>   BASE;
        typedef typename BASE::pointer      pointer;       //!< The type of pointer stored
        typedef typename BASE::element_type element_type;  //!< The type of data stored
        typedef typename BASE::deleter_type deleter_type;  //!< Type of deleter

        /*! \brief Construct from pointer
         *
         * This object will take ownership of the pointer
         *
         * \param [in] ptr Pointer to take ownership of
         * \param [in] d Deleter function
         */
        ScopedModule(T * ptr, const deleter_type & d)
            : BASE(ptr, d) { }

        /*! \brief Move construct
         *
         * Ownership of the pointer is transferred
         *
         * \param [in] rhs The other ScopedModule to transfer ownership from
         */
        ScopedModule(ScopedModule && rhs) = default;


        /*! \brief Move assign
         *
         * Ownership of the pointer is transferred
         *
         * \param [in] rhs The other ScopedModule to transfer ownership from
         */
        ScopedModule & operator=(ScopedModule && rhs) = default;


        // Don't need these
        ScopedModule()                                     = delete;
        ScopedModule(const ScopedModule & rhs)             = delete;
        ScopedModule & operator=(const ScopedModule & rhs) = delete;

        //! \brief Access the functions of the underlying pointer
        pointer operator->() const { return BASE::operator->(); }

    private:
        /*! \brief Release ownership
         *
         * Ownership is given up, but the deleter is not called
         */
        pointer release(void) { return BASE::release(); }


        /*! \brief Get the associated deletion function
         */
        deleter_type & get_deleter(void) { return BASE::get_deleter(); }

        /*! \brief Get the associated deletion function
         */
        const deleter_type & get_deleter(void) const { return BASE::get_deleter(); }
};


} // close namespace modulelocator
} // close namespace bpmodule

#endif
