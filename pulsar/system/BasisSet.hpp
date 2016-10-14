/*! \file
 *
 * \brief Main BasisSet object (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#ifndef PULSAR_GUARD_SYSTEM__BASISSET_HPP_
#define PULSAR_GUARD_SYSTEM__BASISSET_HPP_

#include <functional>

#include "pulsar/system/BasisSetShell.hpp"
#include "pulsar/system/BasisShellInfo.hpp"


namespace pulsar{
namespace system {

/*! \brief A basis set for a QM calculation
 *
 * Basis shell information is stored compacted. That is,
 * common exponents/coefficients are stored once, with
 * shells containing pointers to the data. Therefore, extreme
 * care should be taken when modifying shell data. Note that
 * this is also done with coordinates - each xyz coordinate is
 * stored only once, with shells containing a pointer to the
 * appropriate entry.
 *
 * Shell information that is actually stored is referred to as
 * the unique shells. That is, information from each unique
 * shell can be shared among several shells.
 */
class BasisSet
{
    public:
        typedef std::vector<BasisSetShell>::const_iterator const_iterator;
        typedef BasisSetShell value_type;
        typedef std::function<BasisShellInfo (const BasisShellInfo &, const CoordType &)> TransformerFunc;

        /*! \brief Construct a basis set object
         *
         * This information is required before construction. The parameters
         * refer to the maximum expected to be stored. The actual stored
         * may be less, since information is stored compacted.
         *
         * \param [in] nshells Maximum number of shells expected to be stored
         * \param [in] nprim Maximum number of primitives expected to be stored. This
         *                   is also the number of exponents (alpha) to be stored.
         * \param [in] ncoef Maximum number of coefficients to be stored.
         * \param [in] nxyz Maximum number of coordinates to be stored. If not sure,
         *                  use 3 * \p nprim.
         */
        BasisSet(size_t nshells, size_t nprim, size_t ncoef, size_t nxyz);

        BasisSet(const BasisSet & rhs);
        BasisSet & operator=(const BasisSet & rhs);
        BasisSet(BasisSet && rhs)                  = default;
        BasisSet & operator=(BasisSet && rhs)      = default;

        bool operator==(const BasisSet & rhs) const;
        bool operator!=(const BasisSet & rhs) const;


        /// \name General information
        ///@{


        /// Return a set of the stored types
        std::set<ShellType> get_types(void) const;

        /// Return the number of shells currently stored
        size_t n_shell(void) const noexcept;

        /// Return the number of unique shells stored
        size_t n_unique_shell(void) const noexcept;

        /// Return the number of primitives stored
        size_t n_primitives(void) const;

        /// Return the number of coefficients stored
        size_t n_coefficients(void) const;

        /*! \brief Return the number of basis functions
         *         represented by the basis set
         *
         * That is, the sum of the number of functions for all
         * shells. This takes into account whether a shell is
         * marked as spherical or cartesian. This takes into account general
         * contractions.
         */
        size_t n_functions(void) const;

        /*! \brief Return the maximum number of primitives contained in a shell
         */
        size_t max_n_primitives(void) const;

        /*! \brief Return the maximum angular momentum stored in this basis set
         * 
         * This takes combined shells into account (ie, spd will count only as d).
         */
        int max_am(void) const;

        /*! \brief Return a set of all angular momentum used in this basis set
         * 
         * This takes combined shells into account (ie, spd will count only as d).
         */
        std::set<int> all_am(void) const;

        /*! \brief Return the maximum number of basis functions stored in a shell
         * 
         * This takes into account combined shells (ie, spd = 10 cartesian or 9 spherical)
         */
        size_t max_n_functions(void) const;


        /*! \brief Find some maximum property (ncartesian, etc)
         */
        size_t max_property(std::function<size_t(const BasisSetShell &)> func) const;

        ///@}

        /// \name Shell information and manipulation
        ///@{

        /* \brief Obtain information about a shell
         * 
         * \throw pulsar::BasisSetException if the
         *        shell index is out of range
         *
         * \param [in] i Index of the shell to obtain
         */
        const BasisSetShell & shell(size_t i) const;

        /* \brief Obtain information about a unique shell
         * 
         * Information within a unique shell can be shared among
         * multiple shells.
         *
         * The coordinates, center, and ID are not well-defined
         * and belong to an arbitrary shell.
         *
         * \throw pulsar::BasisSetException if the
         *        shell index is out of range
         *
         * \param [in] i Index of the shell to obtain
         */
        const BasisSetShell & unique_shell(size_t i) const;


        /*! \brief Obtain the index of where a shell's functions start
          * in the overall basis set
          */
        size_t shell_start(size_t i) const;


        /* \brief Obtain information about a shell as a shell_info object
         * 
         * The shell_info will not contain information about the center or
         * the unique ID of the shell.
         *
         * \throw pulsar::BasisSetException if the
         *        shell index is out of range
         *
         * \param [in] i Index of the shell to obtain
         */
        BasisShellInfo shell_info(size_t i) const;


        /*! \brief Add a shell to this basis set
         * 
         * Will determine if this shell has data in common with another
         * stored shell
         *
         * \throw pulsar::BasisSetException if the
         *        shell information will not fit in what has been
         *        allocated already.
         * 
         * \param [in] bshell Basis shell information to add
         * \param [in] center ID for what atom/center this shell belongs to. Should
         *                    correspond to an entry in a System.
         * \param [in] xyz Coordinates for this shell
         */
        void add_shell(const BasisShellInfo & bshell,
                      const CoordType & xyz);

        ///@}


        //! \todo make a printer class?
        void print(std::ostream & os) const;

        /*! \brief Reduce memory useage by removing empty space due to
         *         common information
         *
         * add_shell will link information for common shell information,
         * however if more space was allocated in the contructor than is
         * necessary, this results in empty space. This function removes that
         * space.
         *
         * Is called internally, but you can call it whenever you want also.
         *
         * \return A BasisSet matching this, but with more efficient storage.
         */
        BasisSet shrink_fit(void) const;

        /*! \brief General transformation function
         * 
         * This will loop over all shells.
         */
        BasisSet transform(TransformerFunc Transformer) const;


        // iterate over shells
        const_iterator begin(void) const;
        const_iterator end(void) const;

        /*! \brief Obtain a hash of this BasisSet */
        bphash::HashValue my_hash(void) const;


        /*! \brief For serialization only
         * 
         * \warning NOT FOR USE OUTSIDE OF SERIALIZATION
         * \todo Replace if cereal fixes this
         */
        BasisSet() = default;


    private:
        ID_t curid_;
        std::vector<BasisSetShell> shells_;
        std::vector<size_t> unique_shells_;
        std::vector<double> storage_; // storage for alpha and coef
        std::vector<size_t> shellstart_;

        // for filling
        size_t max_nxyz_;
        size_t max_nalpha_;
        size_t max_ncoef_;
        double * xyz_base_ptr_;
        double * alpha_base_ptr_;
        double * coef_base_ptr_;

        size_t xyz_pos_;
        size_t alpha_pos_;
        size_t coef_pos_;


        /// Adds a shell, copying the information from bshell
        void add_shell_(const BasisShellBase & bshell, const CoordType & xyz);

        /// Adds a shell, copying the information from bshell
        void add_shell_(const BasisSetShell & bshell);


        /*! \brief Set the internal pointers to the proper locations */
        void ResetPointers_(void);


        /*! \brief Allocate enough memory for the given information
         *
         * Also set up some pointers
         */
        void allocate_(size_t nshells, size_t nprim, size_t ncoef, size_t nxyz);


        //! \name Serialization and Hashing
        ///@{

        DECLARE_SERIALIZATION_FRIENDS
        BPHASH_DECLARE_HASHING_FRIENDS

        template<class Archive>
        void save(Archive & ar) const
        {
            // serialize the size info
            ar(max_nxyz_, max_nalpha_, max_ncoef_);
            ar(xyz_pos_, alpha_pos_, coef_pos_, curid_);

            // serialize the storage and shell info
            ar(storage_, shells_, unique_shells_, shellstart_);

            // offsets for xyz, alpha, and coef for each shell
            std::vector<ptrdiff_t> offsets;
            offsets.reserve(3*shells_.size());

            const uintptr_t base = reinterpret_cast<uintptr_t>(storage_.data());
            for(const auto & it : shells_)
            {
                offsets.push_back(reinterpret_cast<uintptr_t>(it.alpha_ptr())-base);
                offsets.push_back(reinterpret_cast<uintptr_t>(it.all_coefs_ptr())-base);
                offsets.push_back(reinterpret_cast<uintptr_t>(it.coords_ptr())-base);
            }

            ar(offsets);
        }

        template<class Archive>
        void load(Archive & ar)
        {
            // load the size info
            ar(max_nxyz_, max_nalpha_, max_ncoef_);
            ar(xyz_pos_, alpha_pos_, coef_pos_, curid_);

            // storage and shell info
            ar(storage_, shells_, unique_shells_, shellstart_);

            // offsets for xyz, alpha, and coef for each shell
            std::vector<ptrdiff_t> offsets;
            ar(offsets);

            // now loop over the shells and set the pointers
            const uintptr_t base = reinterpret_cast<uintptr_t>(storage_.data());

            size_t offsetidx = 0;
            for(auto & it : shells_)
            {
                it.set_ptrs_(reinterpret_cast<double *>(base + offsets.at(offsetidx)),
                            reinterpret_cast<double *>(base + offsets.at(offsetidx+1)),
                            reinterpret_cast<double *>(base + offsets.at(offsetidx+2)));
                offsetidx += 3;
            }
        }


        void hash(bphash::Hasher & h) const;


        ///@}
};


//! The common tag for a null basis set (ie, for 3- and 2-center ERI)
#define NULL_BASIS "NULL_BASIS"



} // close namespace system
} // close namespace pulsar


#endif
