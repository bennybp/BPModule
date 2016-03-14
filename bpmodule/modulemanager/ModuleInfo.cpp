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
    output::GlobalOutput("\n");
    output::GlobalOutput("  ++ Module: %1%\n", name);
    output::GlobalOutput("         Version: %1%\n", version);
    output::GlobalOutput("            Type: %1%\n", type);
    output::GlobalOutput("            Path: %1%\n", path);

    output::GlobalOutput("     Description: %1%\n", description);

    output::GlobalOutput("         Authors: %1%\n", (authors.size() ? authors[0] : ""));
    for(size_t i = 1; i < authors.size(); i++)
        output::GlobalOutput("                  %1%\n", authors[i]);

    output::GlobalOutput("      References: %1%\n", (refs.size() ? refs[0] : ""));
    for(size_t i = 1; i < refs.size(); i++)
        output::GlobalOutput("                  %1%\n", refs[i]);

    output::GlobalOutput("         Options: %1%\n", options.Size());
    options.Print();
}




} // close namespace modulemanager
} // close namespace bpmodule

