#include <iostream>

#include "BPModule/store/ModuleStore.h"
#include "BPModule/base/ModuleBase.h"


namespace bpmodule {


bool ModuleStore::Merge(const std::string & filepath, const StoreType & st)
{
    for(auto & it : st)
    {
        if(store_.count(it.first) > 0)
        {
            std::cout << "Duplicate module: " << it.first << "\n";
            return false;
        }

        store_[it.first] = std::bind(it.second, filepath); 
    }

    return true;
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
                  << " Version: " << mbptr->Version() << "\n"
                  << "    Path: " << mbptr->Filepath() << "\n"
                  << "   Class: " << MClassToString(mbptr->MClass()) << "\n"
                  << "    Type: " << MTypeToString(mbptr->MType()) << "\n"
                  << " Authors: " << mbptr->Authors() << "\n"
                  << "    Desc: " << mbptr->Description() << "\n"
                  << "\n\n";
    }
}

} // close namespace bpmodule
