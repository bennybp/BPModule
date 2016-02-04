/*! \file
 *
 * \brief Structure storing information about a module (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#include "bpmodule/modulemanager/ModuleInfo.hpp"
#include "bpmodule/output/Output.hpp"


namespace bpmodule {
namespace modulemanager {


void ModuleInfo::Print(void) const
{
    output::Output("\n");
    output::Output("  ++ Module: %1%\n", name);
    output::Output("         Version: %1%\n", version);
    output::Output("            Type: %1%\n", type);
    output::Output("            Path: %1%\n", path);

    output::Output("     Description: %1%\n", description);

    output::Output("         Authors: %1%\n", (authors.size() ? authors[0] : ""));
    for(size_t i = 1; i < authors.size(); i++)
        output::Output("                  %1%\n", authors[i]);

    output::Output("      References: %1%\n", (refs.size() ? refs[0] : ""));
    for(size_t i = 1; i < refs.size(); i++)
        output::Output("                  %1%\n", refs[i]);

    output::Output("         Options: %1%\n", options.Size());
    options.Print();
}




} // close namespace modulemanager
} // close namespace bpmodule

