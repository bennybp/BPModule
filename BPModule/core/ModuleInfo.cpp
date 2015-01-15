
#include "BPModule/core/ModuleInfo.h"
#include "BPModule/core/Output.h"

namespace bpmodule {


void ModuleInfo::Help(void) const
{
        bpmodule::Output("    Name: %1%\n", name);
        bpmodule::Output("    Desc: %1%\n", description);
        bpmodule::Output(" OPTIONS: %1%\n", options.Size());
        auto opmap = options.Info();
        for(auto & it2 : opmap)
            bpmodule::Output("    %1%  :  %2%\n", it2.first, it2.second.second);
        bpmodule::Output("\n\n");
}

void ModuleInfo::Info(void) const
{
        bpmodule::Output("    Name: %1%\n", name);
        bpmodule::Output(" Version: %1%\n", version);
        bpmodule::Output("    Path: %1%\n", sopath);
        bpmodule::Output("    Desc: %1%\n", description);
        if(authors.size() > 0)
        {
            bpmodule::Output(" Authors: %1%", authors[0]);
            for(size_t i = 1; i < authors.size(); i++)
                bpmodule::Output("          %1%\n", authors[i]);
        }
        if(refs.size() > 0)
        {
            bpmodule::Output("    Refs: %1%\n", refs[0]);
            for(size_t i = 1; i < refs.size(); i++)
                bpmodule::Output("          %1%\n", refs[i]);;
        }
        bpmodule::Output(" OPTIONS: %1%\n", options.Size());
        auto opmap = options.Info();
        for(auto & it2 : opmap)
            bpmodule::Output("    %1%  :  %2%\n", it2.first, it2.second.second);
        bpmodule::Output("\n\n");
}

} // close namespace bpmodule
