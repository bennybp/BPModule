/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "bpmodule/system/System.hpp"
#include "bpmodule/math/Point.hpp"
#include "bpmodule/math/CombItr.hpp"



using bpmodule::system::SystemMap;
using bpmodule::system::System;
using bpmodule::math::Point;
typedef typename SystemMap::value_type Frag_t;

namespace bpmodule{
namespace system{        

SystemMap MakeNMers(const SystemMap& Frags, size_t N, double Dist){
    SystemMap NMers;
    //Make sure we work in two stupid scenarios i.e. N==0 or 1
    if(N==0)return NMers;
    if(N==1)return Frags;
    math::CombItr<SystemMap> Comb(Frags,N);
    while(!Comb.Done()){
        std::stringstream Name;
        std::vector<Point> CoMs;
        for(const Frag_t& Frag : *Comb){
            Name<<Frag.first;
            CoMs.push_back(Frag.second.CenterOfMass());
            if(CoMs.size()!=N)Name<<"_";
        }
        /* Here's the plan.  What we are doing here is sort of isomorphic to
         * finding bonds. Basically two fragments are "bonded" if their centers
         * of mass are within a distance Dist.  Once we've figured out the
         * number of bonds each fragment makes we care that together they
         * form one molecule.  A basic graph-theory result says that a graph
         * (with no loops or parallel edges) must have at least (N-1)(N-2)/2+1 
         * edges to be connected. So we count bonds, if we hit that many our
         * NMer is a go.
         */
        size_t NEdges=0;
        for(size_t i=0;i<N;++i)
            for(size_t j=0;j<i;++j)
                if(CoMs[i].Distance(CoMs[j])<Dist)++NEdges;
        if(NEdges>=((N-1)*(N-2)/2+1)){
            System NMer(Comb->begin()->second);
            for(const Frag_t& Frag: *Comb)//First operation is null
                NMer+=Frag.second;
            NMers.emplace(Name.str(),NMer);
        }
        ++Comb;       
    }

    return NMers;
}

}}//End namespaces
