#include <fstream>
#include <sstream>

#include "bpmodule/basisset/Creators.hpp"
#include "bpmodule/basisset/AMConvert.hpp"
#include "bpmodule/molecule/AtomicInfo.hpp"
#include "bpmodule/exception/BasisSetException.hpp"
#include "bpmodule/output/Output.hpp"


using bpmodule::molecule::AtomicZNumberFromSym;

namespace bpmodule {
namespace basisset {


namespace detail {

BasisMap ReadBasisFile(const std::string & path)
{
    BasisMap bm;

    std::ifstream f(path.c_str());
    if(!f.is_open())
        throw exception::BasisSetException("Unable to open basis set file",
                                           "path", path);

    std::string line;

    std::vector<GaussianBasisShell> gsv;
    std::string sym;

    // cartesian or spherical basis
    std::string cart;
    getline(f, cart);

    // skip over header
    while(getline(f, line))
    {
        if(line == "****")
            break;
    }

    while(getline(f, line))
    {
        if(line == "")
            continue;

        // atomic symbol
        std::stringstream ss(line);
        ss >> sym;

        getline(f, line);
        while(line != "****")
        {
            std::string am;
            int nprim;

            std::stringstream ss2(line);
            ss2 >> am >> nprim;

            bool iscart = (cart == "cartesian" ? true : false);

            if(am == "SP")
            {
                GaussianBasisShell gs_s(0, iscart); 
                GaussianBasisShell gs_p(1, iscart); 

                for(int i = 0; i < nprim; i++)
                {
                    getline(f, line);
                    std::stringstream ss3(line);
                    double alpha, coef;
                    ss3 >> alpha >> coef;

                    gs_s.AddPrimitive(alpha, coef);
                    gs_p.AddPrimitive(alpha, coef);
                }

                gsv.push_back(gs_s);
                gsv.push_back(gs_p);
            }
            else
            {
                int iam = StringToAM(am);
                GaussianBasisShell gs(iam, iscart); 

                for(int i = 0; i < nprim; i++)
                {
                    getline(f, line);
                    std::stringstream ss3(line);
                    double alpha, coef;
                    ss3 >> alpha >> coef;
                    gs.AddPrimitive(alpha, coef);
                }
                gsv.push_back(gs);
            }
            getline(f, line);
        }   

        if(gsv.size())
        {
            bm[AtomicZNumberFromSym(sym)] = gsv;
            gsv.clear();
        }
    }

    return bm;
}

} // close namespace detail



BasisSet SimpleCreator(const std::string & basispath, const molecule::Molecule & mol)
{
    detail::BasisMap bm = detail::ReadBasisFile(basispath);

    output::Output("Basis read with %1% elements\n", bm.size());
    BasisSet bs;

    for(const auto & atom : mol)
    {
        auto gsv = bm.at(atom.z);
        for(const auto & gb : gsv)
            bs.AddShell(GaussianShell(gb, 0, atom.id, atom.xyz));
    }

    return bs;
}


} // close namespace basisset
} // close namespace bpmodule

