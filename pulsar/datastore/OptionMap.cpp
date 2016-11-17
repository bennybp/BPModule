/*! \file
 *
 * \brief Storage of options for a module (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#include "pulsar/util/PythonHelper.hpp"
#include "pulsar/datastore/OptionMap.hpp"
#include "pulsar/output/GlobalOutput.hpp"
#include "pulsar/exception/PulsarException.hpp"
#include "bphash/types/string.hpp"
#include "bphash/types/memory.hpp"
#include "bphash/types/map.hpp"

using namespace pulsar;

namespace pulsar {


////////////////////////////////////////////////
// OptionMapIssues Member functions
////////////////////////////////////////////////
bool OptionMapIssues::ok(void) const
{
    return (toplevel.size() == 0 && optissues.size() == 0);
}

void OptionMapIssues::print(std::ostream & os) const
{
    if(toplevel.size() || optissues.size())
        print_warning(os, "OptionMap has some issues\n");

    if(toplevel.size())
    {
        print_warning(os, "    OptionMap top level issues:\n");
        for(const auto & it : toplevel)
            print_warning(os, "        %?\n", it);
    }
    if(optissues.size())
    {
        print_warning(os, "    Individual option issues:\n");
        for(const auto & it : optissues)
        {
            print_warning(os, "        %?\n", it.first);
            for(const auto & it2 : it.second)
                print_warning(os, "            %?\n", it2);
        }
    }
}

std::string OptionMapIssues::to_string(void) const
{
    std::stringstream ss;
    print(ss);
    return ss.str();
}

////////////////////////////////////////////////
// Member functions
////////////////////////////////////////////////
OptionMap::OptionMap(void)
    : expert_(false)
{ }


OptionMap::OptionMap(const OptionMap & rhs)
    : expert_(rhs.expert_),
      wholevalid_(rhs.wholevalid_)
{
    for(const auto & it : rhs.opmap_)
        opmap_.emplace(it.first, std::move(it.second->clone()));
}

OptionMap & OptionMap::operator=(const OptionMap & rhs)
{
    using std::swap;
    if(this == &rhs)
        return *this;

    // copy and swap it
    OptionMap copy(rhs);
    swap(*this, copy);
    return *this;
}



bool OptionMap::has(const std::string & key) const
{
    if(opmap_.count(key) == 0)
        return false;
    return opmap_.at(key)->has_value();
}


bool OptionMap::has_key(const std::string & key) const
{
    return opmap_.count(key);
}

std::vector<std::string> OptionMap::get_keys()const
{
    std::vector<std::string> keys;
    for(const auto &key_i: opmap_)keys.push_back(key_i.first);
    return keys;
}

size_t OptionMap::size(void) const noexcept
{
    return opmap_.size();
}

bool OptionMap::is_set(const std::string & key) const
{
    return get_or_throw_(key)->is_set();
}

bool OptionMap::is_default(const std::string & key) const
{
    return get_or_throw_(key)->is_default();
}



void OptionMap::reset_to_default(const std::string & key)
{
    get_or_throw_(key)->reset_to_default();
}



bool OptionMap::all_req_set(void) const noexcept
{
    for(const auto & it : opmap_)
        if(!it.second->is_set_if_required())
            return false;
    return true;
}



void OptionMap::set_expert(bool expert) noexcept
{
    expert_ = expert;
}

bool OptionMap::is_expert(void) const noexcept
{
    return expert_;
}



KeySet OptionMap::all_missing_req(void) const
{
    KeySet req;
    for(const auto & it : opmap_)
        if(!it.second->is_set_if_required())
            req.insert(it.first);

    return req;
}


const detail::OptionBase * OptionMap::get_or_throw_(const std::string & key) const
{
    if(opmap_.count(key))
        return opmap_.at(key).get();
    else
        throw PulsarException("Option key not found", "optionkey", key);
}

detail::OptionBase * OptionMap::get_or_throw_(const std::string & key)
{
    if(opmap_.count(key))
        return opmap_.at(key).get();
    else
        throw PulsarException("Option key not found", "optionkey", key);
}


OptionMapIssues OptionMap::get_issues(void) const
{
    OptionMapIssues omi;

    if(wholevalid_)
        omi.toplevel = wholevalid_(*this);

    for(const auto & it : opmap_)
    {
        OptionIssues oi = it.second->get_issues();
        if(oi.size())
            omi.optissues.emplace(it.first, oi);
    }

    return omi;
}


bool OptionMap::has_issues(void) const
{
    return !(get_issues().ok());
}


bool OptionMap::compare(const OptionMap & rhs) const
{
    if(size() != rhs.size())
        return false;

    KeySet keys1, keys2;
    for(const auto & it : opmap_)
        keys1.insert(it.first);
    for(const auto & it : rhs.opmap_)
        keys2.insert(it.first);
    if(keys1 != keys2)
        return false;

    for(const auto & it : opmap_)
        if(!(rhs.opmap_.at(it.first)->compare(*(it.second))))
            return false;

    return true;
}

bool OptionMap::operator==(const OptionMap & rhs) const
{
    return compare(rhs);
}


void OptionMap::add_option(std::string key, OptionType opttype, bool required,
                           const pybind11::object & validator, std::string help,
                           const pybind11::object & def)
{
    if(has_key(key))
        throw PulsarException("Attempting to add duplicate option key",
                              "optionkey", key);

    detail::OptionBasePtr oph = detail::CreateOptionHolder(key, opttype, required,
                                                           validator, help, def);

    opmap_.emplace(std::move(key), std::move(oph));
}

bphash::HashValue OptionMap::my_hash(void) const
{
    return bphash::make_hash(bphash::HashType::Hash128, *this);
}

bphash::HashValue OptionMap::hash_values(const std::set<std::string> & keys) const
{
    bphash::Hasher h(bphash::HashType::Hash128);

    for(const auto & it : keys)
    {
        const detail::OptionBase * op = get_or_throw_(it);
        op->hash_value(h);
    }

    return h.finalize();
}

bphash::HashValue OptionMap::hash_all_values(void) const
{
    bphash::Hasher h(bphash::HashType::Hash128);

    for(const auto & it : opmap_)
        it.second->hash_value(h);

    return h.finalize();
}


void OptionMap::hash(bphash::Hasher & h) const
{
    h(expert_, opmap_);
}


//////////////////////
// Python
//////////////////////
pybind11::object OptionMap::get_py(const std::string & key) const
{
    return get_or_throw_(key)->get_py();
}


void OptionMap::change_py(const std::string & key, const pybind11::object & obj)
{
    detail::OptionBase * ptr = get_or_throw_(key);
    ptr->change_py(obj);
}



//////////////////////////////////////
// printing of options
/////////////////////////////////////
void OptionMap::print(std::ostream & os) const
{
    size_t nopt = size();
    if(nopt > 0)
    {
        print_output(os, "\n");
        std::string s20(20, '-');
        std::string s8(8, '-');
        print_output(os, "    %-20?   %-20?   %-8?   %-20?   %-20?   %-8?   %?\n", "Option", "Type", "is_set", "Value", "Default", "Required", "Description");
        print_output(os, "    %-20?   %-20?   %-8?   %-20?   %-20?   %-8?   %?\n", s20, s20, s8, s20, s20, s8, s20);

        for(const auto & it : opmap_)
            it.second->print(os);
    }
    print_output(os, "\n");
}







} // close namespace pulsar

