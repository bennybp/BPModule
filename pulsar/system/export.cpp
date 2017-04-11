/*! \file
 *
 * \brief Python exports for the system/system library
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include "pulsar/util/Pybind11.hpp"
#include "pulsar/util/PythonHelper.hpp"
#include "pulsar/util/Serialization.hpp"
#include "pulsar/system/AMConvert.hpp"
#include "pulsar/system/AtomicInfo.hpp"
#include "pulsar/system/System.hpp"
#include "pulsar/system/Space.hpp"
#include "pulsar/system/BasisSet.hpp"
#include "pulsar/system/AOOrdering.hpp"
#include "pulsar/system/SphericalTransform.hpp"
#include "pulsar/system/CrystalFunctions.hpp"
#include "pulsar/math/RegisterMathSet.hpp"




namespace pulsar{

std::vector<double> list_to_vector(const pybind11::list& ls){
    std::vector<double> v;
    for(auto i:ls)v.push_back(pybind11::cast<double>(i));
    return v;
}
void set_all_coefs(BasisShellBase& bs,pybind11::list cs){
                  bs.set_all_coefs(list_to_vector(cs));
}
void set_coefs(BasisShellBase& bs,int n,pybind11::list cs){
                   bs.set_coefs(n,list_to_vector(cs));
}
void set_alphas(BasisShellBase& bs,pybind11::list as){
    bs.set_alphas(list_to_vector(as));
}

void export_system(pybind11::module & m)
{

    ///////////////
    // Angular momentum
    ///////////////
    m.def("string_to_am", string_to_am);
    m.def("am_to_string", am_to_string);


    ///////////////
    // Ordering
    ///////////////
    pybind11::class_<AOOrderingMaps>(m, "AOOrderingMaps")
    .def(pybind11::init<>())
    .def(pybind11::init<const AOOrderingMaps &>())
    .def_readwrite("spherical_order", &AOOrderingMaps::spherical_order)
    .def_readwrite("cartesian_order", &AOOrderingMaps::cartesian_order)
    ;

    m.def("all_ao_orderings", all_ao_orderings, pybind11::return_value_policy::copy);
    m.def("spherical_ordering", spherical_ordering);
    m.def("cartesian_ordering", cartesian_ordering);
    m.def("spherical_index", spherical_index);
    m.def("cartesian_index", cartesian_index);
    m.def("full_spherical_index", full_spherical_index);
    m.def("full_cartesian_index", full_cartesian_index);
    m.def("make_basis_reorder_map", make_basis_reorder_map);
    m.def("make_ao_basis_ordering", make_ao_basis_ordering);

    ////////////////////////////
    // Spherical transformation
    ////////////////////////////
    pybind11::class_<SphericalTransformCoefficient>(m, "SphericalTransformCoefficient")
    .def(pybind11::init<>())
    .def(pybind11::init<const SphericalTransformCoefficient &>())
    .def_readwrite("sphidx", &SphericalTransformCoefficient::sphidx)
    .def_readwrite("cartidx", &SphericalTransformCoefficient::cartidx)
    .def_readwrite("coef", &SphericalTransformCoefficient::coef)
    ;

    m.def("all_spherical_transforms", all_spherical_transforms, pybind11::return_value_policy::copy);

    ///////////////
    // Basis set
    ///////////////

    // Enumeration for basis set shell types
    pybind11::enum_<ShellType>(m, "ShellType")
    .value("SphericalGaussian", ShellType::SphericalGaussian)
    .value("CartesianGaussian", ShellType::CartesianGaussian)
    .value("Slater", ShellType::Slater)
    ;

    // BasisShellBase class
    pybind11::class_<BasisShellBase> bshellbase(m, "BasisShellBase");
    bshellbase.def("get_type", &BasisShellBase::get_type)
              .def("am", &BasisShellBase::am)
              .def("general_am", &BasisShellBase::general_am)
              .def("n_primitives", &BasisShellBase::n_primitives)
              .def("n_coefficients", &BasisShellBase::n_coefficients)
              .def("n_general_contractions", &BasisShellBase::n_general_contractions)
              .def("n_functions", &BasisShellBase::n_functions)
              .def("general_n_functions", &BasisShellBase::general_n_functions)
              .def("is_combined_am", &BasisShellBase::is_combined_am)
              .def("my_hash", &BasisShellBase::my_hash)
              .def("alpha", &BasisShellBase::alpha)
              .def("coef", &BasisShellBase::coef)
              .def("get_alpha", &BasisShellBase::get_alpha)
              .def("set_alpha", &BasisShellBase::set_alpha)
              .def("get_coef", &BasisShellBase::get_coef)
              .def("set_coef", &BasisShellBase::set_coef)
              .def("get_alphas", &BasisShellBase::get_alphas)
              .def("set_alphas",&set_alphas)
              //.def("set_alphas", &BasisShellBase::set_alphas)
              .def("get_coefs", &BasisShellBase::get_coefs)
              .def("set_coefs",&set_coefs)
              //.def("set_coefs", &BasisShellBase::set_coefs)
              .def("get_all_coefs", &BasisShellBase::get_all_coefs)
              .def("set_all_coefs",&set_all_coefs)
              //.def("set_all_coefs", &BasisShellBase::set_all_coefs)
              .def("set_primitive", static_cast<void (BasisShellBase::*)(size_t, double, double)>(&BasisShellBase::set_primitive))
              .def("set_primitive", static_cast<void (BasisShellBase::*)(size_t, double, const std::vector<double> &)>(&BasisShellBase::set_primitive))
    ;
    
    // BasisShellInfo class
    pybind11::class_<BasisShellInfo> bshell(m, "BasisShellInfo", bshellbase);
    bshell.def(pybind11::init<ShellType, int, int, int>())
          //RMR-Once Pybind11 conversions work again get rid of this...
          .def("__init__",
             [](BasisShellInfo& bs,ShellType type,int am,int nprim,int ngen,
                pybind11::list as,pybind11::list cs){
                  new (&bs) BasisShellInfo(type,am,nprim,ngen,list_to_vector(as),list_to_vector(cs));
            })
          //...for this
          //.def(pybind11::init<ShellType,int,int,int,const std::vector<double>&,const std::vector<double>&>())
          .def(pybind11::init<const BasisShellInfo&>())
          .def(pybind11::self == pybind11::self)
          .def(pybind11::self != pybind11::self)
    ;
    

    // BasisSetShell class
    pybind11::class_<BasisSetShell>(m, "BasisSetShell", bshell)
    .def("get_coords", &BasisSetShell::get_coords)
    .def("set_coords", &BasisSetShell::get_coords)
    .def("get_coord", &BasisSetShell::get_coord)
    .def("set_coord", &BasisSetShell::get_coord)
    .def("coord", &BasisSetShell::get_coord)
    .def("my_hash", &BasisSetShell::my_hash)
    .def(pybind11::self == pybind11::self)
    .def(pybind11::self != pybind11::self)
    ;



    pybind11::class_<BasisSet>(m, "BasisSet")
    .def(pybind11::init<size_t, size_t, size_t, size_t>())
    .def(pybind11::init<const BasisSet&>())
    .def("print", &BasisSet::print)
    .def("add_shell", &BasisSet::add_shell)
    .def("get_types", &BasisSet::get_types)
    .def("n_shell", &BasisSet::n_shell)
    .def("n_unique_shell",&BasisSet::n_unique_shell)
    .def("shell", &BasisSet::shell, pybind11::return_value_policy::reference_internal)
    .def("unique_shell", &BasisSet::unique_shell, pybind11::return_value_policy::reference_internal)
    .def("shell_start", &BasisSet::shell_start)
    .def("shell_info", &BasisSet::shell_info)
    .def("n_primitives", &BasisSet::n_primitives)
    .def("n_coefficients", &BasisSet::n_coefficients)
    .def("n_coefficients", &BasisSet::n_coefficients)
    .def("n_functions", &BasisSet::n_functions)
    .def("max_n_primitives", &BasisSet::max_n_primitives)
    .def("max_am", &BasisSet::max_am)
    .def("all_am", &BasisSet::all_am)
    .def("max_n_functions", &BasisSet::max_n_functions)
    .def("transform", &BasisSet::transform)
    .def("shrink_fit", &BasisSet::shrink_fit)
    .def("my_hash", &BasisSet::my_hash)
    .def(pybind11::self == pybind11::self)
    .def(pybind11::self != pybind11::self)
    .def("__iter__", [](const BasisSet & t) { return pybind11::make_iterator(t.begin(), t.end()); },
                     pybind11::keep_alive<0, 1>() )
    ;

    // BasisInfo
    pybind11::class_<BasisInfo>(m, "BasisInfo")
    .def(pybind11::init<>())
    .def(pybind11::init<const BasisInfo &>())
    .def_readwrite("description", &BasisInfo::description) 
    .def_readwrite("shells", &BasisInfo::shells)
    .def(pybind11::self == pybind11::self)
    .def(pybind11::self != pybind11::self)
    ;


    ////////////////////
    // System, etc
    ////////////////////

    pybind11::class_<IsotopeData>(m, "IsotopeData")
    .def_readonly("isonum", &IsotopeData::isonum)
    .def_readonly("mass", &IsotopeData::mass)
    .def_readonly("mass_low", &IsotopeData::mass_low)
    .def_readonly("mass_high", &IsotopeData::mass_high)
    .def_readonly("abund", &IsotopeData::abund)
    .def_readonly("abund_low", &IsotopeData::abund_low)
    .def_readonly("abund_high", &IsotopeData::abund_high)
    ;


    pybind11::class_<AtomicData>(m, "AtomicData")
    .def_readonly("Z", &AtomicData::Z)
    .def_readonly("sym", &AtomicData::sym)
    .def_readonly("name", &AtomicData::name)
    .def_readonly("mass", &AtomicData::mass)
    .def_readonly("mass_low", &AtomicData::mass_low)
    .def_readonly("mass_high", &AtomicData::mass_high)
    .def_readonly("isotopes", &AtomicData::isotopes)
    ;


    // Atomic info lookup
    m.def("atomic_info_from_z", atomic_info_from_z, pybind11::return_value_policy::copy);
    m.def("atomic_info_from_symbol", atomic_info_from_symbol, pybind11::return_value_policy::copy);
    m.def("isotope_info_from_z", isotope_info_from_z, pybind11::return_value_policy::copy);
    m.def("isotope_info_from_symbol", isotope_info_from_symbol, pybind11::return_value_policy::copy);
    m.def("most_common_isotope_from_z", most_common_isotope_from_z);
    m.def("most_common_isotope_from_symbol", most_common_isotope_from_symbol);

    m.def("atomic_mass_from_z", atomic_mass_from_z);
    m.def("atomic_mass_from_symbol", atomic_mass_from_symbol);
    m.def("isotope_mass_from_z", isotope_mass_from_z);
    m.def("isotope_mass_from_symbol", isotope_mass_from_symbol);
    m.def("atomic_symbol_from_z", atomic_symbol_from_z);
    m.def("atomic_z_from_symbol", atomic_z_from_symbol);
    m.def("atomic_name_from_z", atomic_name_from_z);
    m.def("atomic_name_from_symbol", atomic_name_from_symbol);
    m.def("atomic_multiplicity_from_z", atomic_multiplicity_from_z);
    m.def("atomic_multiplicity_from_symbol", atomic_multiplicity_from_symbol);


    // Other free functions
    m.def("inertia_tensor",inertia_tensor);
    m.def("get_connectivity",get_connectivity);
    m.def("translate", translate<std::array<double, 3>>);
    m.def("rotate", rotate<std::array<double, 9>>);
    m.def("center_of_mass", center_of_mass);
    m.def("center_of_nuclear_charge", center_of_nuclear_charge);
    m.def("system_to_angstroms",system_to_angstroms);
    m.def("update_subsystem",update_subsystem);
    m.def("Frac2Cart",Frac2Cart);
    m.def("MakeSuperCell",MakeSuperCell);
    m.def("CarveUC",CarveUC,pybind11::arg("SC"),pybind11::arg("Sides"),
                            pybind11::arg("MinScale")=1.0,
                            pybind11::arg("MaxScale")=2.0);
    m.def("CleanUC",CleanUC);


    // Atom structure
    // Atom class
    pybind11::class_<Atom, Point>(m, "Atom")
    .def(pybind11::init<const Atom &>())
    .def_readwrite("Z", &Atom::Z)
    .def_readwrite("isotope", &Atom::isotope)
    .def_readwrite("mass", &Atom::mass)
    .def_readwrite("isotope_mass", &Atom::isotope_mass)
    .def_readwrite("charge", &Atom::charge)
    .def_readwrite("multiplicity", &Atom::multiplicity)
    .def_readwrite("nelectrons", &Atom::nelectrons)
    .def_readwrite("cov_radius", &Atom::cov_radius)
    .def_readwrite("vdw_radius", &Atom::vdw_radius)
    .def_readwrite("basis_sets", &Atom::basis_sets)
    .def("my_hash", &Atom::my_hash)
    .def("print", &Atom::print)
    .def(pybind11::self == pybind11::self)
    .def(pybind11::self != pybind11::self)
    .def("__getstate__",[](const Atom &a){return __getstate__(a);})
    .def("__setstate__",[](Atom &a,const pybind11::str& b){__setstate__(a,b);})
    ;
   

    // Atom creators, ghost/dummy/point charge, etc
    m.def("create_atom", static_cast<Atom (*)(CoordType, int)>(create_atom));
    m.def("create_atom", static_cast<Atom (*)(CoordType, int, int)>(create_atom));
    m.def("make_ghost_atom", make_ghost_atom);
    m.def("make_point_charge", static_cast<Atom (*)(const CoordType &, double)>(make_point_charge));
    m.def("make_point_charge", static_cast<Atom (*)(const Atom &,double)>(make_point_charge));
    m.def("make_dummy_atom", static_cast<Atom (*)(const Atom &)>(make_dummy_atom));
    m.def("make_dummy_atom", static_cast<Atom (*)(const CoordType &)>(make_dummy_atom));
    m.def("is_ghost_atom", is_ghost_atom);
    m.def("is_point_charge", is_point_charge);
    m.def("is_dummy_atom", is_dummy_atom);


    // Space
    pybind11::class_<Space>(m,"Space",pybind11::metaclass())
      .def(pybind11::init<>())
      .def(pybind11::init<const Space&>())
      .def(pybind11::init<const std::array<double,3>&,
                           const std::array<double,3>&>())
      .def("is_periodic", &Space::is_periodic)
      .def_readonly_static("infinity",&Space::infinity)
      .def_readonly("lattice_sides",&Space::lattice_sides)
      .def_readonly("lattice_angles",&Space::lattice_angles)
      .def(pybind11::self == pybind11::self)
      .def(pybind11::self != pybind11::self)
    ;

 
    // Export AtomSetUniverse
    // No need to export AtomSet (at the moment)
    register_Universe<AtomSetUniverse>(m, "AtomSetUniverse");

    pybind11::class_<System, std::shared_ptr<System>>(m,"System")
    .def(pybind11::init<const AtomSetUniverse&, bool>())
    .def(pybind11::init<const System &>())
    .def(pybind11::init<const System &,bool>())
    .def("get_sum_mass",&System::get_sum_mass)
    .def("get_sum_charge",&System::get_sum_charge)
    .def("get_sum_n_electrons",&System::get_sum_n_electrons)
    .def_readwrite("mass",&System::mass)
    .def_readwrite("charge",&System::charge)
    .def_readwrite("nelectrons",&System::nelectrons)
    .def_readwrite("multiplicity",&System::multiplicity)
    .def_readwrite("space",&System::space)
    .def("compare_info",&System::compare_info)
    .def("clear",&System::clear)
    .def("size",&System::size)
    .def("count", &System::count)
    .def("insert", static_cast<System &(System::*)(const Atom &)>(&System::insert),
                   pybind11::return_value_policy::reference)
    .def("get_universe", &System::get_universe)
    .def("as_universe", &System::as_universe)
    .def("get_basis_set", &System::get_basis_set)
    .def("my_hash", &System::my_hash)
    .def("print", &System::print)
    .def("union_assign", &System::union_assign, pybind11::return_value_policy::reference)
    .def("set_union", &System::set_union)
    .def("intersection_assign", &System::intersection_assign, pybind11::return_value_policy::reference)
    .def("intersection", &System::intersection)
    .def("difference_assign", &System::difference_assign, pybind11::return_value_policy::reference)
    .def("difference", &System::difference)
    .def("complement", &System::complement)
    .def("is_proper_subset_of", &System::is_proper_subset_of)
    .def("is_subset_of", &System::is_subset_of)
    .def("is_proper_superset_of", &System::is_proper_superset_of)
    .def("is_superset_of", &System::is_superset_of)
    .def("transform", &System::transform)
    .def("partition", &System::partition)
    .def("print",&System::print)
    .def(pybind11::self += pybind11::self, pybind11::return_value_policy::reference)
    .def(pybind11::self + pybind11::self)
    .def(pybind11::self -= pybind11::self, pybind11::return_value_policy::reference)
    .def(pybind11::self - pybind11::self)
    .def(pybind11::self /= pybind11::self, pybind11::return_value_policy::reference)
    .def(pybind11::self / pybind11::self)
    .def(pybind11::self >= pybind11::self)
    .def(pybind11::self > pybind11::self)
    .def(pybind11::self <= pybind11::self)
    .def(pybind11::self < pybind11::self)
    .def(pybind11::self == pybind11::self)
    .def(pybind11::self != pybind11::self)
    .def("__str__",[](const System& t){std::stringstream ss;t.print(ss);return ss.str();})
    .def("__len__",         &System::size)
    .def("__contains__",    &System::count)
    .def("__iter__", [](const System & t) { return pybind11::make_iterator(t.begin(), t.end()); },
                     pybind11::keep_alive<0, 1>() )
    .def("__getstate__",[](const System &a){return __getstate__(a);})
    .def("__setstate__",[](System &a,const pybind11::str& b){__setstate__(a,b);})
    ;

}

} // close namespace pulsar

