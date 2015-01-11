#include "BPModule/modules/base/Parallel_Base.h"

namespace bpmodule {

Parallel_Base::Parallel_Base(ModuleStore * mstore,
                             const std::string & filepath,
                             ModuleType mtype,
                             const std::string & name, const std::string & authors, 
                             const std::string & version, const std::string & description)
    : ModuleBase(mstore, filepath, ModuleClass::PARALLEL, mtype, name, authors, version, description)
{
}


Parallel_Base::Range Parallel_Base::GetRange_(long totalsize) const
{
    int rank = Rank();
    int size = Size();

    long nelements = (totalsize / static_cast<long>(size));
    long start = nelements * rank;
    long leftover = (totalsize % Size());

    if(rank < leftover)
    {
        start += rank;
        nelements++;
    }
    else
        start += leftover;

    return Range(start, start+nelements);
}


void Parallel_Base::ParallelFor(long totalsize, ParallelFunc pf)
{
    ParallelFor_(GetRange_(totalsize), pf);
}

} // close namespace bpmodule
