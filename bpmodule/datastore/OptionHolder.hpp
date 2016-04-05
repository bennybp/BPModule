/*! \file
 *
 * \brief Storage of options data (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef BPMODULE_GUARD_DATASTORE__OPTIONHOLDER_HPP_
#define BPMODULE_GUARD_DATASTORE__OPTIONHOLDER_HPP_

#include <functional>

#include "bpmodule/datastore/OptionBase.hpp"
#include "bpmodule/datastore/OptionTypes.hpp"
#include "bpmodule/util/Serialization.hpp"

namespace bpmodule {
namespace datastore {


/*! \brief Creates an OptionHolder based on an OptionType
 *
 * Passes the rest of the arguments to the OptionHolder constructor
 */
std::unique_ptr<OptionBase>
CreateOptionHolder(std::string key, OptionType opttype, bool required,
                   const pybind11::object & validator, std::string help,
                   const pybind11::object & def);

/*! \brief Creates an OptionHolder based on an OptionType
 *
 * Passes the rest of the arguments to the OptionHolder constructor
 */
std::unique_ptr<OptionBase>
OptionHolderFromByteArray(OptionType opttype, const ByteArray & ba);





/*! \brief Holds the value and default for an option
 *
 * \note This is a template class, but definitions are in the
 *       cpp file. This class is then defined for the
 *       allowed option types.
 */
template<OptionType OPTTYPE>
class OptionHolder : public OptionBase
{
    public:
        //! The type I'm storing
        typedef typename OptionTypeInfo<OPTTYPE>::stored_type stored_type;


        //! A function that can validate me
        typedef std::function<OptionIssues (const OptionHolder<OPTTYPE> &)> ValidatorFunc;


        /*! \brief Constructs without a default
         *
         * This object will take ownership of the value and def pointers.
         * The default value will be validated (if given).
         *
         * The value is not set on construction, only the default
         *
         * \throw bpmodule::exception::OptionException
         *        If the default value is invalid, or
         *        there is a default argument supplied for a 'required' option.
         *
         * \throw bpmodule::exception::PythonCallException
         *       If there is a problem calling the validation function
         *
         * \param [in] key The key of this option
         * \param [in] validator A validator function for this object
         * \param [in] required True if this option is required
         * \param [in] help A help string for this option
         *
         */
        OptionHolder(const std::string & key,
                     bool required, const pybind11::object & validator,
                     const std::string & help);

        OptionHolder(const std::string & key,
                     bool required, const pybind11::object & validator,
                     const std::string & help, const stored_type & def);

        OptionHolder(const std::string & key,
                     bool required, const pybind11::object & validator,
                     const std::string & help, const pybind11::object & def);



        /*! \brief Copy constructor
         *
         * Data will be deep copied
         */
        OptionHolder(const OptionHolder & oph);




        OptionHolder(void)                                  = delete;
        OptionHolder(OptionHolder && oph)                   = delete;
        OptionHolder & operator=(const OptionHolder & oph)  = delete;
        OptionHolder & operator=(OptionHolder && oph)       = delete;
        virtual ~OptionHolder()                             = default;



        /*! \brief Change the stored value
         *
         * This object will copy construct a new value
         *
         * \exstrong
         */
        void Change(const stored_type & value);



        /*! \brief Get the current value
         *
         * If the value is not set, but a default exists, the default is returned.
         *
         * \throw bpmodule::exception::OptionException
         *        If the option does not have a value or a default
         */
        const stored_type & Get(void) const;




        /*! \brief Get the default value
         *
         * \throw bpmodule::exception::OptionException
         *        If the option does not have a default
         */
        const stored_type & GetDefault(void) const;



        ////////////////////////////////////////
        // Virtual functions from OptionBase
        ////////////////////////////////////////

        virtual OptionBasePtr Clone(void) const;

        virtual const char * TypeString(void) const noexcept;

        virtual OptionType Type(void) const noexcept;

        virtual bool HasValue(void) const noexcept;

        virtual bool HasDefault(void) const noexcept;

        virtual bool IsDefault(void) const;

        virtual void ResetToDefault(void) noexcept;

        virtual void Print(std::ostream & os) const;

        virtual OptionIssues GetIssues(void) const;

        virtual bool Compare(const OptionBase & rhs) const;

        virtual ByteArray ToByteArray(void) const;

        /////////////////////////////////////////
        // Python-related functions
        /////////////////////////////////////////
        virtual pybind11::object GetPy(void) const;

        virtual void ChangePy(const pybind11::object & obj);


    private:
        friend std::unique_ptr<OptionBase> OptionHolderFromByteArray(OptionType, const ByteArray &);

        // used in constructor delegation
        OptionHolder(const std::string & key,
                     bool required, const pybind11::object & validator,
                     const std::string & help, stored_type * def);

        // used in deserialization
        OptionHolder(std::string && key,bool required, 
                     std::string && help,
                     std::unique_ptr<stored_type> && val,
                     std::unique_ptr<stored_type> && def);

        //! A set value for the option
        std::unique_ptr<stored_type> value_;

        //! The default for the option
        std::unique_ptr<stored_type> default_;

        //! Validation function object
        ValidatorFunc validator_;


};

        



} //closing namespace datastore
} //closing namespace bpmodule


#endif
