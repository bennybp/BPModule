
#include "BPModule/core/ModuleInfo.h"
#include "BPModule/core/Output.h"

namespace bpmodule {


void ModuleInfo::PrintHelp(void) const
{
        Output("%|12| : %|-2|\n", "Name", name);
        Output("%|12| : %|-2|\n", "Description", description);
        Output("%|12| : %|-2|\n", "Options", options.Size());
        Output(" OPTIONS: %1%\n", options.Size());
        options.PrintHelp();
        Output("\n\n");
}

void ModuleInfo::PrintInfo(void) const
{
        Output("%|12| : %|-2|\n", "Name", name);
        Output("%|12| : %|-2|\n", "Version", version);
        Output("%|12| : %|-2|\n", "Path", sopath);
        Output("%|12| : %|-2|\n", "Description", description);
        Output("    Desc: %1%\n", description);
        if(authors.size() > 0)
        {
            Output("%|12| : %|-2|\n", "Authors", authors[0]);
            for(size_t i = 1; i < authors.size(); i++)
                Output("%|12|   %|-2|\n", "", authors[0]);
        }
        if(refs.size() > 0)
        {
            Output("%|12| : %|-2|\n", "Refs", refs[0]);
            for(size_t i = 1; i < refs.size(); i++)
                Output("%|12|   %|-2|\n", "", refs[i]);
        }
        Output("%|12| : %|-2|\n", "Options", options.Size());
        options.PrintInfo();
        Output("\n\n");
}

} // close namespace bpmodule
