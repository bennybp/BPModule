#include "TestCXX.hpp"
#include <pulsar/math/EigenImpl.hpp>

using namespace pulsar::math;
using EigenMat=Eigen::MatrixXd;
using shared_eigen=std::shared_ptr<EigenMat>;
using shared_matrix=std::shared_ptr<EigenMatrixImpl>;
using Matrix_t=BlockedEigenMatrix;
using std::bind;
using namespace std::placeholders;

TEST_CLASS(TestBlockByIrrepSpin){
    Tester tester("Testing BlockByIrrepSpin with BlockedEigenMatrix.");
    
    shared_eigen Mat1_=std::make_shared<EigenMat>(3,3),
                 Mat2_=std::make_shared<EigenMat>(3,3);
    (*Mat1_)<<1.0,2.0,3.0,
            4.0,5.0,6.0,
            7.0,8.0,9.0;
    (*Mat2_)<<2.0,4.0,6.0,
             8.0,10.0,12.0,
             14.0,16.0,18.0;
    shared_matrix Mat1=std::make_shared<EigenMatrixImpl>(Mat1_),
                  Mat2=std::make_shared<EigenMatrixImpl>(Mat2_);
    shared_matrix Mat3=std::make_shared<EigenMatrixImpl>(*Mat2);
    Matrix_t M1,M4,M5,M6;
    Irrep iA=Irrep::A,iB=Irrep::B;
    Spin sa=Spin::alpha,sb=Spin::beta;
    M1.set(iA,sa,Mat1);
    M4.set(iB,sb,Mat2);
    tester.test("Set/Get elements works",Mat1,M1.get(iA,sa));
    Matrix_t M2(M1);
    tester.test("Copy constructor works",M1,M2);
    Matrix_t M3(std::move(M1));
    tester.test("Move constructor works",M2,M3);
    M5=M4;
    tester.test("Copy assignment works",M4,M5);
    M6=std::move(M4);
    tester.test("Move assignment works",M5,M6);
    tester.test("Not equal works",true,M5!=M1);
    tester.test("Has works true",true,M5.has(iB,sb));
    tester.test("Has works false",false,M5.has(iA,sb));
    tester.test("Get valid irreps works",std::set<Irrep>({iA}),M2.get_irreps());
    tester.test("Get valid spin works",std::set<int>({sb}),M5.get_spins(iB));
    tester.test("Get invalid spin works",std::set<int>({}),M5.get_spins(iA));
    using const_get=const shared_matrix&(Matrix_t::*)(Irrep,int)const;
    using get=shared_matrix&(Matrix_t::*)(Irrep,int);
    auto get1=bind(static_cast<const_get>(&Matrix_t::get),_1,_2,_3);
    auto get2=bind(static_cast<get>(&Matrix_t::get),_1,_2,_3);
    tester.test("Get invalid element throws",false,Mat1,get2,M5,iA,sb);
    tester.test("Const get works",true,Mat1,get1,M2,iA,sa);
    tester.test("Const get invalid throws",false,Mat1,get1,M2,iB,sa);
    M5.set(iB,sa,std::move(Mat2));
    tester.test("Move set works",*Mat3,*M5.get(iB,sa));
    tester.test("Same structure different",false,M5.same_structure(M6));
    tester.test("Same structure same",true,M2.same_structure(M3));
    const std::string hash="8d2ea346f055c3726ff70b565864d38c";
    tester.test("Hash works 1",hash,bphash::hash_to_string(M2.my_hash()));
    tester.test("Hash works 2",hash,bphash::hash_to_string(M3.my_hash()));
    
    tester.print_results();
}
    
    
    
    
