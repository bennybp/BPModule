#include <iostream>
#include <vector>

#include "BPModule/core/ModuleStore.h"
#include "BPModule/modules/base/Test_Base.h"
#include "BPModule/modules/base/Parallel_Base.h"

using namespace bpmodule;


class TestModule1 : public Test_Base
{
public:
    TestModule1(ModuleStore * mstore, const std::string & filepath)
                   : Test_Base(
                     mstore,
                     filepath,
                     ModuleType::TEST,
                     "SIMPLETEST1",
                     "Yo-Yo Mama",
                     "1.0",
                     "Just a simple test module component")
    {
    }

    void RunTest(void)
    {
        std::cout << "I am TestModule 1. Running tests...\n";
        std::unique_ptr<Parallel_Base> ptr = mstore_.Get<Parallel_Base>("PARALLEL");
        std::cout << "Obtained parallelizer: " << ptr->Name() << " (type " << ptr->MTypeStr() << ")\n";
        std::cout << "This is process " << ptr->Rank() << " of " << ptr->Size() << "\n";
        int nthread = ptr->Threads();
        double * sum = new double[nthread];
        std::fill(sum, sum+nthread, 0.0);

        ptr->ParallelFor(1e10l, [sum](long i, long r, long t) { sum[t] += 3.14*static_cast<double>(i/(i+2)); });

        // accumulate
        double allsum = 0;
        for(int i = 0; i < nthread; i++)
            allsum += sum[i];
        std::cout << "Sum is: " << allsum << "\n";
        delete [] sum;
    }

};

class TestModule2 : public Test_Base
{
public:
    TestModule2(ModuleStore * mstore, const std::string & filepath)
                   : Test_Base(
                     mstore,
                     filepath,
                     ModuleType::TEST,
                     "SIMPLETEST2",
                     "Yo-Yo Mama",
                     "2.0",
                     "Just a simple test module component")
    {
    }

    void RunTest(void)
    {
        std::cout << "I am TestModule 2. Running tests...\n";
    }

};


ModuleBase * NewTestModule1(ModuleStore * mstore, const std::string & filepath) { return new TestModule1(mstore, filepath); }
ModuleBase * NewTestModule2(ModuleStore * mstore, const std::string & filepath) { return new TestModule2(mstore, filepath); }


extern "C" {

StoreType GetComponents(void)
{
    StoreType st;
    st["TEST1"] = NewTestModule1;
    st["TEST2"] = NewTestModule2;
    return st;
}

}

