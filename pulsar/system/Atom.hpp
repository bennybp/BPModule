/*\file
 *
 * \brief Atoms and atomic centers (header)
*/


#ifndef PULSAR_GUARD_SYSTEM__ATOM_HPP_
#define PULSAR_GUARD_SYSTEM__ATOM_HPP_
#include <iostream>
#include <set>
#include <map>

#include "pulsar/system/CoordType.hpp"
#include "pulsar/system/BasisShellInfo.hpp"
#include "pulsar/util/StringUtil.hpp"
#include "bphash/Hasher.hpp"
#include "bphash/Hash.hpp"



namespace pulsar{
namespace system {

//Macro to define getter/setters and reduce code duplication
#define GetSetX(PFxnName,PName,PType)\
PType Get##PFxnName(void) const noexcept{return PName ##_;}\
void Set##PFxnName(PType value)noexcept{PName##_ =value;}   

/*! \brief A center in a system
 *
 * Atoms contain a unique index, which must be set on construction.
 * This would generally be the input ordering, but is otherwise arbitrary.
 *
 * Generally, Atoms would be created by one of the CreateAtom
 * free functions, which fill in much of the information with defaults
 * given a Z number (and optionally an isotope number).
 *
 * Many values which are traditionally integers are instead represented
 * as doubles, which allows for fractional occupation.
 *
 * Data is stored within the Atom object, therefore copying will not
 * create aliases.
 *
 * Follows PointConcept via derivation from math::Point
 */
class Atom : public math::Point
{
    private:
        int Z_;        //!< Atomic Z number (as integer. Also stored as a (double) weight)
        int isonum_;   //!< Isotope number

        double mass_;           //!< Atomic mass (abundance-weighted isotope masses)
        double isotopemass_;    //!< Mass of the selected isotope
        double charge_;         //!< Charge on the center
        double multiplicity_;   //!< Electronic multiplicity
        double nelectrons_;     //!< Number of assigned electrons
        double covradius_;      //!< The covalent radius
        double vdwradius_;      //!< The van der waals radius


        //! Information stored about the basis set on the atom
        struct BasisInfo_
        {
            // Stores string which is stored lowercase and trimmed
            typedef std::set<std::string> SetType_;

            SetType_ description;            //!< Description of basis
            BasisShellInfoVector shells;     //!< Actual basis

            bool operator==(const BasisInfo_ & rhs) const;


            //! \name Serialization and Hashing
            ///@{

            template<class Archive>
            void serialize(Archive & ar)
            {
                ar(description, shells);
            }

            void hash(bphash::Hasher & h) const;

            ///@}
        };

        std::map<std::string, BasisInfo_> bshells_; //!< Basis functions associated with this atom/center




        //! \name Serialization and Hashing
        ///@{

        DECLARE_SERIALIZATION_FRIENDS
        friend class bphash::Hasher;


        template<class Archive>
        void serialize(Archive & ar)
        {
            // we aren't serializing the base class, so we do this manually
            ar(cereal::base_class<math::Point>(this), Z_, isonum_,
                                                mass_, isotopemass_,
                                                charge_, multiplicity_,
                                                nelectrons_, covradius_,
                                                vdwradius_, bshells_);
        }

        void hash(bphash::Hasher & h) const;

        ///@}


    public:
        /*! \brief Constructor
         */
        Atom(CoordType xyz, int Z, int isonum, double mass,
             double isotopemass, double charge, double multiplicity,
             double nelectrons,double covradius,double vdwradius);

        /*! \brief For serialization only
         * 
         * \warning NOT FOR USE OUTSIDE OF SERIALIZATION
         * \todo Replace if cereal fixes this
         */
        Atom() = default;

        Atom(const Atom &)             = default;
        Atom & operator=(const Atom &) = default;
        Atom(Atom &&)                  = default;
        Atom & operator=(Atom &&)      = default;


        /* \brief Equality comparison
         *
         * This compares all components individually
         */
        bool operator==(const Atom & rhs) const;

        /* \brief Inequality comparison
         *
         * This compares all components individually
         */
        bool operator!=(const Atom & rhs) const;



        /*! \name General properties
         */ 
        ///@{ 

        /*
         * I'm aware that a class with lots of getters/setters is bad
         * form. However, we are leaving it open to future optimizations,
         * such as storing the data elsewhere
         */


        /*! \brief Get the name of the element */
        std::string GetName(void) const;

        /*! \brief Get the symbol of the element */
        std::string GetSymbol(void) const;

        /*! \brief Get/Set the atomic Z number (number of protons) */
        GetSetX(Z,Z,int);
        
        /*! \brief Get/Set the isotope number (number of protons + neutrons) */
        GetSetX(Isonum,isonum,int);

        /*! \brief Get/Set the atomic mass (isotope masses weighted by abundance) */
        GetSetX(Mass,mass,double);

        /*! \brief Get/Set the mass of the isotope */
        GetSetX(IsotopeMass,isotopemass,double);

        /*! \brief Get/Set the charge on this atom/center */
        GetSetX(Charge,charge,double);

        /*! \brief Get/Set the electronic multiplicity of this atom/center  */
        GetSetX(Multiplicity,multiplicity,double);

        /*! \brief Get/Set the number of electrons assigned to this atom/center */
        GetSetX(NElectrons,nelectrons,double);

        /*! \brief Get/Set the covalent radius of the atom*/
        GetSetX(CovRadius,covradius,double);
        
        /*! \brief Get/Set the van der Waals radius of the atom*/
        GetSetX(VDWRadius,vdwradius,double);
        ///@}



        /*! \name Basis Set information
         */ 
        ///@{ 

        /*! \brief See is this atom has a basis set assigned with the given label
         */
        bool HasShells(const std::string & label) const
        {
            return bshells_.count(label);
        }

        /*! \brief Number of shells with this label on this atom
         */
        int NShell(const std::string & label) const
        {
            if(!HasShells(label))
                return 0;
            return bshells_.at(label).shells.size();
        }


        /*! \brief Get a set of all the basis set labels on this atom
         */
        std::set<std::string> GetAllBasisLabels(void) const
        {
            std::set<std::string> ret;
            for(const auto & it : bshells_)
                ret.insert(it.first);
            return ret;
        }

        /*! \brief Get the description of the shells in a basis set
         * 
         * Typically used to store the basis set name (6-31G, etc)
         */
        std::string GetBasisDescription(const std::string & label) const
        {
            if(!HasShells(label))
                return "none";
            else
                return util::Join(bshells_.at(label).description, ", ");
        }


        /*! \brief Get information for a particular assigned basis set
         * 
         * If a basis set with the given label doesn't exist on this center, an
         * empty BasisShellInfoVector is returned
         */
        BasisShellInfoVector GetShells(const std::string & label) const
        {
            if(HasShells(label))
                return bshells_.at(label).shells;
            else
                return BasisShellInfoVector();
        }

        /*! \brief Set all the shells for a basis set with a given label
         * 
         * Existing basis set information (for that label) is overwritten
         */
        void SetShells(const std::string & label, const std::string & shelldesc, const BasisShellInfoVector & shells)
        {
            // Braces around shelldesc = convert to a set of strings
            bshells_[label] = BasisInfo_{ {shelldesc}, shells};
        }


        /*! \brief Append a shell to a basis set with a given label
         */
        void AddShell(const std::string & label, const std::string & shelldesc, const BasisShellInfo & shell)
        {
            bshells_[label].shells.push_back(shell);
            bshells_[label].description.insert(util::Trim_Copy(util::ToLower_Copy(shelldesc)));
        }



        ///@}


        /*! \name Printing */
        ///@{

        void Print(std::ostream & os) const;

        ///@}

        bphash::HashValue MyHash(void) const;

};

//! \todo What to do about printing
std::ostream& operator<<(std::ostream& os,const Atom& A);




/* \brief Create an atom given an ID, coordinates, and atomic number
 *
 * The rest of the data is filled in automatically
 */
Atom CreateAtom(CoordType xyz, int Z);


/*! \copydoc CreateAtom(CoordType xyz, int Z) */
Atom CreateAtom(double x, double y, double z, int Z);



/* \brief Create an atom given an ID, coordinates, atomic number, and isotope number
 *
 * The rest of the data is filled in automatically
 */
Atom CreateAtom(CoordType xyz, int Z, int isonum);


/*! \copydoc CreateAtom(CoordType xyz, int Z, int isonum) */ 
Atom CreateAtom(double x, double y, double z, int Z, int isonum);


///@{\name Ghost Atom Functions
/** When making/checking for ghost atoms please use these functions to ensure
 *  consistent definitions of ghost atoms.  A ghost atom has the same basis
 *  functions as the atom it is replacing, but no other properties, i.e.
 *  a charge of 0, a mass of 0, a multiplicity of 0, etc.  Ghost atoms
 *  must be made from atoms that have basis functions already because the
 *  Z information will be lost.  In practice this poses no problems for
 *  any existing BSSE method.
 */
///Makes a copy of AtomI that is a ghost atom
Atom MakeGhost(const Atom& AtomI);
///Returns true if AtomI is a ghost atom
bool IsGhost(const Atom& AtomI);
///@}

///@{\name Point Charge Functions
/** When making/checking for point charges please use these functions to ensure
 *  consistent definitions of point charge.  A point charge has a charge, but
 *  no other properties.
 */
///Makes a charge from a charge and coordinates
Atom MakeCharge(double Charge,CoordType xyz);
///Copies AtomI into a point charge
inline Atom MakeCharge(const Atom& AtomI){
    return MakeCharge(AtomI.GetCharge(),AtomI.GetCoords());
}
///Makes a point charge from a charge and coordinates
inline Atom MakeCharge(double Charge, double x, double y, double z){
    return MakeCharge(Charge, {x,y,z});
}
///Returns true if AtomI is a point charge
bool IsCharge(const Atom& AtomI);
///@}

///@{\name Dummy Atom Functions
/** When making/checking for point charges please use these functions to ensure
 *  consistent definitions of point charge.  A point charge has a charge, but
 *  no other properties.
 */
///Makes a dummy atom from coordinates
Atom MakeDummy(CoordType xyz);

///Copies AtomI into a dummy atom
inline Atom MakeDummy(const Atom& AtomI){
    return MakeDummy(AtomI.GetCoords());
}
///Makes a dummy atom from component coordinates
inline Atom MakeDummy(double x, double y, double z){
    return MakeDummy({x,y,z});
}
///Returns true if AtomI is a dummy atom
bool IsDummy(const Atom& AtomI);
///@}


#undef GetSetX


} // close namespace system
} // close namespace pulsar

///Allows atoms to be hashed
namespace std{
    template<> struct hash<pulsar::system::Atom>
    {
        size_t operator()(const pulsar::system::Atom& atom)const{
            return bphash::convert_hash<size_t>(atom.MyHash());
        }
    };
}

#endif

