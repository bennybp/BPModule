
#include "BPModule/core/ModuleInfo.h"


namespace bpmodule {


void ModuleInfo::Help(std::ostream & out) const
{
        out << "    Name: " << name << "\n"
            << "    Desc: " << description << "\n"
            << " OPTIONS: " << options.Size() << "\n";
        auto opmap = options.Dump();
        for(auto & it2 : opmap)
            out << "    " << it2.first << "   :   " << it2.second.second << "\n";
        out << "\n\n";
}

void ModuleInfo::Dump(std::ostream & out) const
{
        out << "    Name: " << name << "\n"
            << " Version: " << version << "\n"
            << "    Path: " << sopath << "\n"
            << "    Desc: " << description << "\n";
        if(authors.size() > 0)
        {
            out << " Authors: " << authors[0] << "\n";
            for(size_t i = 1; i < authors.size(); i++)
                out << "          " << authors[i] << "\n";
        }
        if(refs.size() > 0)
        {
            out << "    Refs: " << refs[0] << "\n";
            for(size_t i = 1; i < refs.size(); i++)
                out << "          " << refs[i] << "\n";
        }
        out << " OPTIONS: " << options.Size() << "\n";
        auto opmap = options.Dump();
        for(auto & it2 : opmap)
            out << "    " << it2.first << "   =   " << it2.second.first << "\n";
        out << "\n\n";
}

} // close namespace bpmodule
