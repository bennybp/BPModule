#include <map>

#include "bpmodule/molecule/Atoms.hpp"


namespace {
    std::map<std::string, int> symboltoz_{
                                            {"H"  ,   1},
                                            {"He" ,   2},
                                            {"Li" ,   3},
                                            {"Be" ,   4},
                                            {"B"  ,   5},
                                            {"C"  ,   6},
                                            {"N"  ,   7},
                                            {"O"  ,   8},
                                            {"F"  ,   9},
                                            {"Ne" ,  10},
                                            {"Na" ,  11},
                                            {"Mg" ,  12},
                                            {"Al" ,  13},
                                            {"Si" ,  14},
                                            {"P"  ,  15},
                                            {"S"  ,  16},
                                            {"Cl" ,  17},
                                            {"Ar" ,  18},
                                            {"K"  ,  19},
                                            {"Ca" ,  20},
                                         };

    std::map<int, std::string> ztosymbol_{
                                            {   1, "H"  },
                                            {   2, "He" },
                                            {   3, "Li" },
                                            {   4, "Be" },
                                            {   5, "B"  },
                                            {   6, "C"  },
                                            {   7, "N"  },
                                            {   8, "O"  },
                                            {   9, "F"  },
                                            {  10, "Ne" },
                                            {  11, "Na" },
                                            {  12, "Mg" },
                                            {  13, "Al" },
                                            {  14, "Si" },
                                            {  15, "P"  },
                                            {  16, "S"  },
                                            {  17, "Cl" },
                                            {  18, "Ar" },
                                            {  19, "K"  },
                                            {  20, "Ca" },
                                         };
} // close anonymous namespace



namespace bpmodule {
namespace molecule {

int AtomSymbolToZ(const std::string & sym)
{
    return symboltoz_.at(sym);
}

std::string AtomZToSymbol(int Z)
{
    return ztosymbol_.at(Z);
}


} // close namespace molecule
} // close namespace bpmodule

