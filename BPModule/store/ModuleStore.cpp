#include <iostream>

#include "BPModule/store/ModuleStore.h"
#include "BPModule/base/ModuleBase.h"


namespace bpmodule {


void ModuleStore::Add(StorePair sp)
{
    store_.insert(std::move(sp));
}


void ModuleStore::Add(std::string id, ModuleGeneratorFunc mgen)
{
    store_.insert(StorePair(std::move(id), std::move(mgen)));
}


void ModuleStore::Delete(const std::string & id)
{
    store_.erase(id);
}


ModuleBaseUPtr ModuleStore::Get(const std::string & id)
{
    //! \todo Change this of course
    return ModuleBaseUPtr(store_.at(id)());
}

size_t ModuleStore::Count(void) const
{
    return store_.size();
}

void ModuleStore::DumpInfo(void) const
{
    std::cout << "Count: " << store_.size() << "\n";
    for(auto & it : store_)
    {
        ModuleBaseUPtr mbptr(it.second());

        std::cout << "---------------------------------\n"
                  << it.first << "\n"
                  << "---------------------------------\n"
                  << "    Name: " << mbptr->Name() << "\n"
                  << "   Class: " << MClassToString(mbptr->MClass()) << "\n"
                  << "    Type: " << MTypeToString(mbptr->MType()) << "\n"
                  << " Authors: " << mbptr->Authors() << "\n"
                  << "    Desc: " << mbptr->Description() << "\n"
                  << "\n\n";
    }
}

} // close namespace bpmodule
