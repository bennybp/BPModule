/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "pulsar/math/Irrep.hpp"

namespace pulsar{
namespace math{

const std::map<Irrep,std::string> irrep_to_string={
    {Irrep::A,"A"},
    {Irrep::Ag,"Ag"},
    {Irrep::Au,"Au"},
    {Irrep::Ap,"A'"},
    {Irrep::App,"A''"},
    {Irrep::B,"B"},
    {Irrep::Bg,"Bg"},
    {Irrep::Bu,"Bu"},
    {Irrep::A1,"A1"},
    {Irrep::A1g,"A1g"},
    {Irrep::A1u,"A1u"},
    {Irrep::A2,"A2"},
    {Irrep::A2g,"A2g"},
    {Irrep::A2u,"A2u"},
    {Irrep::B1,"B1"},
    {Irrep::B1g,"B1g"},
    {Irrep::B1u,"B1u"},
    {Irrep::B2,"B2"},
    {Irrep::B2g,"B2g"},
    {Irrep::B2u,"B2u"},
    {Irrep::E1,"E1"},
    {Irrep::E1g,"E1g"},
    {Irrep::E1u,"E1u"},
    {Irrep::E2,"E2"},
    {Irrep::E2g,"E2g"},
    {Irrep::E2u,"E2u"}
};

//The unicode usage is is going to backfire, but it looks cool....
const std::map<Spin,std::string> spin_to_string={
    {Spin::alpha,"α"},
    {Spin::beta,"β"}
};

}
}
