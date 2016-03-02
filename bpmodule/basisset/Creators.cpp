#include <fstream>
#include <sstream>

#include "bpmodule/basisset/Creators.hpp"
#include "bpmodule/basisset/AMConvert.hpp"
#include "bpmodule/system/AtomicInfo.hpp"
#include "bpmodule/exception/Exceptions.hpp"
#include "bpmodule/output/Output.hpp"


using bpmodule::system::AtomicZNumberFromSym;
using bpmodule::system::Atom;
using bpmodule::system::Molecule;

namespace bpmodule {
namespace basisset {


namespace detail {

BasisMap ReadBasisFile(ShellType type, const std::string & path)
{
    BasisMap bm;

    std::ifstream f(path.c_str());
    if(!f.is_open())
        throw exception::BasisSetException("Unable to open basis set file",
                                           "path", path);

    std::string line;

    std::vector<BasisShell> gsv;
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
                BasisShell gs_s(type, 0, iscart); 
                BasisShell gs_p(type, 1, iscart); 

                for(int i = 0; i < nprim; i++)
                {
                    getline(f, line);
                    std::stringstream ss3(line);
                    double alpha, coef, coef2;
                    ss3 >> alpha >> coef >> coef2;

                    gs_s.AddPrimitive(alpha, coef);
                    gs_p.AddPrimitive(alpha, coef2);
                }

                gsv.push_back(gs_s);
                gsv.push_back(gs_p);
            }
            else
            {
                int iam = StringToAM(am);
                BasisShell gs(type, iam, iscart); 

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



system::Molecule SimpleCreator(const std::string & basispath,
                               const Molecule & mol,
                               const std::string & basislabel)
{
    detail::BasisMap bm = detail::ReadBasisFile(basispath);

    output::Output("Basis read with %1% elements\n", bm.size());

    return mol.Transform([& bm, & basislabel](const Atom & atom) { 
        Atom a(atom);  // copy the atom
        a.SetShells(basislabel, bm.at(atom.GetZ())); // set is basis set for this label
        return a;
    });
}


} // close namespace basisset
} // close namespace bpmodule

