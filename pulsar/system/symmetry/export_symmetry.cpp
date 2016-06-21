/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "pybind11/stl.h"
#include "pybind11/functional.h"
#include "pybind11/operators.h"
#include "pulsar/system/symmetry/Symmetrizer.hpp"
#include "pulsar/system/symmetry/SymmetryElement.hpp"
#include "pulsar/system/symmetry/SymmetryGroup.hpp"
#include "pulsar/system/System.hpp"

namespace pulsar{
namespace system{
namespace export_python{
void export_symmetry(pybind11::module & m)
{
    pybind11::class_<Symmetrizer>(m, "Symmetrizer")
    .def(pybind11::init<>())
    .def("get_symmetry",&Symmetrizer::get_symmetry);
    
    pybind11::class_<SymmetryElement>(m,"SymmetryElement")
    .def(pybind11::init<const std::array<double,9>&,
                        const std::string&,const std::string&>())
    .def(pybind11::init<const SymmetryElement&>())
    .def_readonly("element_matrix",&SymmetryElement::element_matrix)
    .def_readonly("schoenflies_symbol",&SymmetryElement::schoenflies_symbol)
    .def_readonly("hm_symbol",&SymmetryElement::hm_symbol)
    ;
    
    pybind11::class_<MirrorPlane>
    (m,"MirrorPlane",pybind11::base<SymmetryElement>())
    .def(pybind11::init<const std::array<double,3>&>())
    ;
    
    pybind11::class_<Rotation>
    (m,"Rotation",pybind11::base<SymmetryElement>())
    .def(pybind11::init<const std::array<double,3>&,size_t,size_t>())
    ;
    
    pybind11::class_<ImproperRotation>
    (m,"ImproperRotation",pybind11::base<SymmetryElement>())
    .def(pybind11::init<const std::array<double,3>&,size_t,size_t>())
    ;
    
    m.attr("Identity")=pybind11::cast(SymmetryElement(Identity));
    m.attr("CoI")=pybind11::cast(SymmetryElement(CoI));
    
    pybind11::class_<SymmetryGroup>(m,"SymmetryGroup")
    .def(pybind11::init<const std::unordered_set<SymmetryElement>&,
                        const std::string&,const std::string&>())
    .def(pybind11::init<const SymmetryGroup&>())
    .def_readonly("symmetry_elements",&SymmetryGroup::symmetry_elements)
    .def_readonly("schoenflies_symbol",&SymmetryGroup::schoenflies_symbol)
    .def_readonly("hm_symbol",&SymmetryGroup::hm_symbol)
    .def("order",&SymmetryGroup::order)
    .def("finite",&SymmetryGroup::finite)
    ;
//For PGs w/o an n
#define ExportPG(name)\
    pybind11::class_<PointGroup::name>\
    (m,#name,pybind11::base<SymmetryGroup>())\
    .def(pybind11::init<const std::unordered_set<SymmetryElement>&>())\
    .def(pybind11::init<>());
 
//For PGs w/ an n
#define ExportPGn(name)\  
    pybind11::class_<PointGroup::name>\
    (m,#name,pybind11::base<SymmetryGroup>())\
    .def(pybind11::init<const std::unordered_set<SymmetryElement>&,size_t>())\
    .def(pybind11::init<size_t>());
    
    ExportPG(Cs)
    ExportPG(Ci)
    ExportPG(Coov)
    ExportPG(Dooh)
    ExportPG(Kh)
    ExportPGn(Cn)
    ExportPGn(Cnv)
    ExportPGn(Cnh)
    ExportPGn(Sn)
    ExportPGn(Dn)
    ExportPGn(Dnh)
    ExportPGn(Dnd)

#undef ExportPGn
#undef ExportPG   
}

}}}
