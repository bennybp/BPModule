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
    output::GlobalOutput("  ++ Module: %?\n", name);
    output::GlobalOutput("         Version: %?\n", version);
    output::GlobalOutput("            Type: %?\n", type);
    output::GlobalOutput("            Path: %?\n", path);

    output::GlobalOutput("     Description: %?\n", description);

    output::GlobalOutput("         Authors: %?\n", (authors.size() ? authors[0] : ""));
    for(size_t i = 1; i < authors.size(); i++)
        output::GlobalOutput("                  %?\n", authors[i]);

    output::GlobalOutput("      References: %?\n", (refs.size() ? refs[0] : ""));
    for(size_t i = 1; i < refs.size(); i++)
        output::GlobalOutput("                  %?\n", refs[i]);

    output::GlobalOutput("         Options: %?\n", options.Size());
    options.Print();
}




} // close namespace modulemanager
} // close namespace bpmodule

