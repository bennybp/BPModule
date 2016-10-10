/*! \file
 *
 * \brief Base class for a test module (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef PULSAR_GUARD_MODULEBASE__SYSTEMFRAGMENTER_HPP_
#define PULSAR_GUARD_MODULEBASE__SYSTEMFRAGMENTER_HPP_

#include "pulsar/modulebase/ModuleBase.hpp"
#include "pulsar/system/System.hpp"

namespace pulsar {
namespace modulebase {

using SNType = std::set<std::string>; //< The type of the serial number

///A struct to hold info associated with an NMer
struct NMerInfo {

    NMerInfo() : SN(), NMer(), Weight(1.0)
    {
    }
    ///\todo Do we need the SN in this struct?
    SNType SN; //< The serial number of the NMer
    system::System NMer; //< The actual nmer
    double Weight; //< The weight of the nmer
};

using NMerSetType = std::map<SNType, NMerInfo>; //< The type of a set of NMers

/*! \brief A module that fragments a system and makes all n-mers
 *
 *  There is really only one reason people worry about fragments in
 *  electronic structure theory and that is to get at properties of parts
 *  of a larger system.  In SAPT and MBE-like methods the fragments are used
 *  to understand the many-body interactions among molecules in a larger cluster
 *  or crystal.  For BSSE methods we can obtain more accurate relative energies
 *  by ensuring each part of the system is computed consistently.
 *
 *  For an \f$N\f$ atom system there are \f$B_N\f$, ways to assign the atoms
 *  into fragments, where \f$B_N\f$ is the \f$N\f$-th Bell number, which is
 *  basically infinity for any \f$N\f$ typically considered in quantum
 *  chemistry.  This means that it is unlikely any two quantum chemists are
 *  going to want the same fragmentation scheme.  Thus we allow you to
 *  develop your own scheme via making fragmentation a module.
 *
 *  When designing your fragmentation module you are free to use whatever
 *  parameters you'd like as criteria. There are some common criteria like
 *  bonds and distances that we'd like you to use the same terms for though.
 *
 *  At the end of the day what this module aims to do is return a set of
 *  \f$n\f$-mers (unions of \f$n\f$ fragments) that were obtained via
 *  an algorithm.  For each \f$n\f$-mer we typically want a unique,systematic,
 *  tag, which I will call its serial number, that tells us which fragments
 *  were combined to form that \f$n\f$-mer.  Finally we also want to know
 *  the intrinsic weight of properties associated with that \f$n\f$-mer when
 *  we sum up \f$n\f$-mer properties.  The combination of this data
 *  is what I have called an NMerInfo struct (I probably should use a tuple,
 *  but find the get<n> syntax clunky).
 *
 *  Furthermore it is likely that you will want some common functions, which
 *  we put in this base class as protected functions you may use in your
 *  algorithm, but you are free to ignore them if they don't do what you want.
 *  If you find a better way to do any of them please make a pull request.
 *
 *  It is also usually the case that we want to screen based on who tightly
 *  cluster the \f$n\f$-mer is.  The standard deviation, \f$\sigma\f$, is a
 *  measure of how tightly a
 *  group of \f$N\f$ points are clustered together.  \f$\sigma\f$ is defined
 *  as:
 *  \f[
 *    \sigma=\sqrt{\frac{\sum_{i=1}^{N}\left(r_i-r_0\right)^2}{N}},
 *  \f]
 *  where \f$r_i\f$ is the position of the \f$i\f$-th point in the cluster
 *  and \f$r_0\f$ is the average position.  If we insist that our fragments
 *  are clustered to within sigma we have that our points must satisfy:
 *  \f{eqnarray*}{
 *    \sigma\ge&\sqrt{\frac{\sum_{i=1}^{N}\left(r_i-r_0\right)^2}{N}}\\
 *    N\sigma^2\ge&\sum_{i=1}^{n}\left(r_i-r_0\right)^2+
 *                  \sum_{j=n}^N\left(r_i-r_0\right)^2\\
 *    N\sigma^2-\sum_{i=1}^{n}\left(r_i-r_0\right)^2\ge&
 *            \sum_{j=n}^N\left(r_i-r_0\right)^2\\
 *    N\sigma^2-\sum_{i=1}^{n}\left(r_i-r_0\right)^2\ge&0
 *  \f}
 *  where in the second line we arbitrarily split our range in 2 and in
 *  the last line we realize that the sum of the squares of real numbers
 *  must be greater than 0.  This ultimately shows us
 *  that if the squared distance from the mean of the first \f$n\f$ points
 *  exceeds \f$N\f$ times the threshold squared it is
 *  impossible for the remaining \f$N-n\f$ points to be clustered tightly
 *  enough even if they all literally sat at the mean point.
 *
 *  For an \f$N\f$-mer the logical choice of \f$r_0\f$ is the center of
 *  mass and \f$r_i\f$ is the center of mass for each monomer in the
 *  \f$N\f$-mer.
 *
 *
 */
class SystemFragmenter : public ModuleBase {
protected:

    /** \brief Function for taking all unions of a set of fragments
     *
     *   For a lot of methods that depend on fragments you need to consider
     *   pairs of fragments, triples of fragments, etc.  This function takes
     *   a SystemMap of fragments and takes all N-way unions of them.
     *
     *   \param[in] Frags The monomers we want the unions of
     *
     *   The truncation order is read from option "TRUNCATION_ORDER" and the
     *   distance threholds are read in from option "DISTANCE_THRESHOLDS"
     *   \param[in] Dist The maximum distance
     */
    NMerSetType make_nmers(const NMerSetType& Frags);

    /////////////////////////////////////////
    // To be implemented by derived classes
    /////////////////////////////////////////
    /*! \copydoc fragmentize
     *
     *   This function should produce the fundamental fragments.
     *   Creation of the n-mers (unions of fragments taken n at a time)
     *   will be handled automatically by the fragmentize function
     *    if requested)
     * \note To be implemented by derived classes
     */
    virtual NMerSetType fragmentize_(const system::System & mol) = 0;


public:
    typedef SystemFragmenter BaseType;

    SystemFragmenter(ID_t id)
    : ModuleBase(id, "SystemFragmenter")
    {
    }

    /*! \brief Fragment the system
     *
     *   The main call users of this class should call
     */
    NMerSetType fragmentize(const system::System & mol)
    {
        return ModuleBase::call_function(&SystemFragmenter::fragmentize_, mol);
    }
};

class SystemFragmenter_Py : public SystemFragmenter {
public:
    using SystemFragmenter::SystemFragmenter;

    MODULEBASE_FORWARD_PROTECTED_TO_PY


    virtual NMerSetType fragmentize_(const system::System & mol)
    {
        return call_py_override<NMerSetType>("fragmentize_", mol);
    }

    ///Python's dictionaries are stupid, this rolls the STL set into a string
    std::map<std::string, NMerInfo> fragmentize_py(const system::System& mol);



};

} // close namespace modulebase
} // close namespace pulsar

#endif
