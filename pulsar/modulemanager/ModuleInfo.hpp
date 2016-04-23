/*! \file
 *
 * \brief Structure storing information about a module (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef PULSAR_GUARD_MODULEMANAGER__MODULEINFO_HPP_
#define PULSAR_GUARD_MODULEMANAGER__MODULEINFO_HPP_


#include "pulsar/datastore/OptionMap.hpp"


namespace pulsar{
namespace modulemanager {

/*! \brief Structure that stores information about a module
 *
 * \par Hashing
 * The hash value of a point represents a unique with respect to
 * all the module information and the values for the options.
 * See the hashing section of OptionMap for details on that.
 */
struct ModuleInfo
{
    std::string name;                  //!< Some descriptive name for the module
    std::string type;                  //!< Type of the supermodule (c_module, python_module, etc)
    std::string base;                  //!< Base class of this module (EnergyMethod, etc)
    std::string path;                  //!< Full path for the supermodule (including the filename for an SO file)
    std::string version;               //!< Arbitrary version string of the supermodule
    std::string description;           //!< A description of the module
    std::vector<std::string> authors;  //!< Authors of the module
    std::vector<std::string> refs;     //!< References for the module (articles, web pages, etc)
    datastore::OptionMap options;      //!< Options for the module


    /*! \brief Print out this info
    */
    void Print(std::ostream & os) const;


    /*! \brief Comapre two ModuleInfo
     *  
     *  See also the OptionMap comparison operator for details about that
     */
    bool operator==(const ModuleInfo & rhs) const;


    util::Hash MyHash(void) const;    

    private:
        //! \name Serialization and Hashing
        ///@{

        DECLARE_SERIALIZATION_FRIENDS
        DECLARE_HASHING_FRIENDS

        /* We have to split load/save since the
         * the shared_ptr points to const data, and
         * cereal can't serialize to const data
         */
        template<class Archive>
        void serialize(Archive & ar)
        {
            ar(name, type, base, path);
            ar(version, description, authors, refs, options);
        }

        void hash(util::Hasher & h) const;

        ///@}
};


} // close namespace modulemanager
} // close namespace pulsar


#endif
