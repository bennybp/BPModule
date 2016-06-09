/*! \file
 *
 * \brief Structure storing information about a module (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#include "pulsar/modulemanager/ModuleInfo.hpp"
#include "pulsar/output/Output.hpp"

#include "bphash/types/string.hpp"
#include "bphash/types/vector.hpp"


namespace pulsar{
namespace modulemanager {


void ModuleInfo::Print(std::ostream & os) const
{
    using namespace output;

    Output(os, "\n");
    Output(os, "  ++ Module: %?\n", name);
    Output(os, "         Version: %?\n", version);
    Output(os, "            Type: %?\n", type);
    Output(os, "            Base: %?\n", base);
    Output(os, "            Path: %?\n", path);

    Output(os, "     Description: %?\n", description);

    Output(os, "         Authors: %?\n", (authors.size() ? authors[0] : ""));
    for(size_t i = 1; i < authors.size(); i++)
        Output(os, "                  %?\n", authors[i]);

    Output(os, "      References: %?\n", (refs.size() ? refs[0] : ""));
    for(size_t i = 1; i < refs.size(); i++)
        Output(os, "                  %?\n", refs[i]);

    Output(os, "         Options: %?\n", options.Size());
    options.Print(os);
}


bphash::HashValue ModuleInfo::MyHash(void) const
{
    return bphash::MakeHash(bphash::HashType::Hash128, *this);
}


void ModuleInfo::hash(bphash::Hasher & h) const
{
    h(name, type, base, path, version, description,
      authors, refs, options); 
}

bool ModuleInfo::operator==(const ModuleInfo & rhs) const
{
    return (name == rhs.name &&
            type == rhs.type &&
            base == rhs.base &&
            path == rhs.path &&
            version == rhs.version &&
            description == rhs.description &&
            authors == rhs.authors &&
            refs == rhs.refs &&
            options == rhs.options);
}



} // close namespace modulemanager
} // close namespace pulsar

