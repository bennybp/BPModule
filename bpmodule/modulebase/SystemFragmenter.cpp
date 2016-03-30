/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include<limits>//for max of double
#include "bpmodule/modulebase/SystemFragmenter.hpp"
#include "bpmodule/datastore/OptionMap.hpp"
#include "bpmodule/math/CombItr.hpp"
#include "bpmodule/math/Point.hpp"

using bpmodule::datastore::OptionMap;
using bpmodule::system::System;
using bpmodule::system::SystemMap;
using bpmodule::math::Point;
typedef typename SystemMap::value_type Frag_t;

namespace bpmodule{
namespace modulebase{

SystemMap SystemFragmenter::Fragmentize(const System & mol){
    const OptionMap& DaOptions=Options();
    size_t N=2;//DaOptions.Get<size_t>("TRUNCATION_ORDER");
    std::map<size_t,double> Truncs;//=
        //DaOptions.Get<std::map<size_t,double>>("DISTANCE_THRESHOLDS");
    SystemMap Frags=
            ModuleBase::CallFunction(&SystemFragmenter::Fragmentize_, mol);
    SystemMap NMers=Frags;
    for(size_t n=2;n<=N;++n){
        //Defaults to about 1e308 a.u., or about 1e271 times the size of
        //the observable universe...I think that is equivalent to no cut-off
        double dist=(Truncs.count(n)==1?
                     Truncs.at(n):std::numeric_limits<double>::max());
        SystemMap Temp=MakeNMers(Frags,n,dist);
        NMers.insert(Temp.begin(),Temp.end());
    }
    return NMers;
}
    
SystemMap SystemFragmenter::MakeNMers(const SystemMap& Frags, 
                                      size_t N, double Dist){
    SystemMap NMers;
    //Make sure we work in two stupid scenarios i.e. N==0 or 1
    if(N==0)return NMers;//Empty set
    if(N==1)return Frags;//The fragments we were given
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
}}//end namespaces
