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

void ModuleInfo::print(std::ostream & os) const
{
    

    print_output(os, "\n");
    print_output(os, "  ++ Module: %?\n", name);
    print_output(os, "         Version: %?\n", version);
    print_output(os, "            Type: %?\n", type);
    print_output(os, "            Base: %?\n", base);
    print_output(os, "            Path: %?\n", path);

    print_output(os, "     Description: %?\n", description);

    print_output(os, "         Authors: %?\n", (authors.size() ? authors[0] : ""));
    for(size_t i = 1; i < authors.size(); i++)
        print_output(os, "                  %?\n", authors[i]);

    print_output(os, "      References: %?\n", (refs.size() ? refs[0] : ""));
    for(size_t i = 1; i < refs.size(); i++)
        print_output(os, "                  %?\n", refs[i]);

    print_output(os, "         Options: %?\n", options.size());
    options.print(os);
}


bphash::HashValue ModuleInfo::my_hash(void) const
{
    return bphash::make_hash(bphash::HashType::Hash128, *this);
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

} // close namespace pulsar

