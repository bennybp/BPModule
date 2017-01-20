#include <pulsar/testing/CppTester.hpp>
#include <pulsar/math/FiniteDiff.hpp>
#include <cmath>

using namespace pulsar;
using VectorD=std::vector<double>;
using Return_t=std::vector<VectorD>;
using cD=const double;

void compare_deriv(const std::string& msg,const Return_t& Result,
                   const VectorD& Corr,CppTester& tester){
    for(size_t i=0;i<Result.size();i++){
        const std::string test_msg=msg+" component "+std::to_string(i);
        tester.test_double(test_msg,Result[i][0],Corr[i]);
    }
}

TEST_SIMPLE(TestFiniteDiff){

    CppTester tester("Testing the finite difference class.");
    

//fxn is 1/3 r^3
struct HOVisitor:public pulsar::FDiffVisitor<double,VectorD>{
    VectorD Coords_;
    double coord(size_t i)const{return Coords_[i];}
    double shift(cD& Old,cD& H,double Shift)const{return Old+H*Shift;}

    VectorD run(size_t i,cD& NewVar)const{
        VectorD Result({0.0});
        for(size_t j=0; j<Coords_.size();++j){
            double val=std::pow((j==i?NewVar:Coords_[j]),3);
            Result[0]+=val;
        }
        Result[0]*=1.0/3.0;
        return Result;
    }

    void scale(VectorD& RV,double constant)const{
        for(double& i:RV)i*=constant;
    }

    //Set element i of RV to Element/H
    void update(VectorD& RV,VectorD& Element,size_t,double H)const{
        if(RV.size()<Element.size())RV.resize(Element.size(),0.0);
        for(size_t j=0;j<Element.size();j++)RV[j]+=Element[j]/H;
    }
};

HOVisitor MyVisitor;
MyVisitor.Coords_={1.0,2.0,3.0,4.0,5.0,6.0};

//Deriv is x^2
VectorD Deriv({1.0,4.0,9.0,16.0,25.0,36.0});

using CDiff_t=pulsar::CentralDiff<double,VectorD>;
CDiff_t CFD;
Return_t Result;

///FDiff alogrithms are not designed to be moved/copied so we can't bind it
try{
    CFD.Run(MyVisitor,6,0.01,2);
    throw pulsar::PulsarException("Central Diff should throw");
}
catch(...){tester.test_equal("C-Diff threw for even stencil size",true,true);}

try{
    CFD.Run(MyVisitor,6,0.01,1);
    throw pulsar::PulsarException("Central Diff should throw");
}
catch(...){tester.test_equal("C-Diff threw for a stencil size of 1",true,true);}


Result=CFD.Run(MyVisitor,6,0.01,3);
compare_deriv("C-Diff 3",Result,Deriv,tester);

Result=CFD.Run(MyVisitor,6,0.01,5);
compare_deriv("C-Diff 5",Result,Deriv,tester);

//For foward/backward n=2 only gets like two sig figs right

pulsar::BackwardDiff<double,VectorD> BFD;
Result=BFD.Run(MyVisitor,6,0.01,3);

compare_deriv("B-Diff 3",Result,Deriv,tester);

Result=BFD.Run(MyVisitor,6,0.01,4);
compare_deriv("B-Diff 4",Result,Deriv,tester);

pulsar::ForwardDiff<double,VectorD> FFD;
Result=FFD.Run(MyVisitor,6,0.01,3);
compare_deriv("F-Diff 3",Result,Deriv,tester);

Result=FFD.Run(MyVisitor,6,0.01,4);
compare_deriv("F-Diff 4",Result,Deriv,tester);

tester.print_results();
return tester.nfailed();

}
