#include "TestCXX.hpp"
#include <pulsar/math/EigenImpl.hpp>

using namespace pulsar::math;
using Matrix_t=BlockedEigenMatrix;
using std::bind;
using namespace std::placeholders;

TEST_CLASS(TestBlockByIrrepSpin){
    Tester tester("Testing BlockByIrrepSpin with BlockedEigenMatrix.");
    
    std::shared_ptr<Eigen::MatrixXd> Mat1_=std::make_shared<Eigen::MatrixXd>(3,3),
                                     Mat2_=std::make_shared<Eigen::MatrixXd>(3,3);
    (*Mat1_)<<1.0,2.0,3.0,
            4.0,5.0,6.0,
            7.0,8.0,9.0;
    (*Mat2_)<<2.0,4.0,6.0,
             8.0,10.0,12.0,
             14.0,16.0,18.0;
    EigenMatrixImpl Mat1(Mat1_),Mat2(Mat2_);
    EigenMatrixImpl Mat3(Mat2);
    Matrix_t M1,M4,M5,M6;
    M1.set(Irrep::A,Spin::alpha,Mat1);
    M4.set(Irrep::B,Spin::beta,Mat2);
    tester.test("Set/Get elements works",Mat1,M1.get(Irrep::A,Spin::alpha));
    Matrix_t M2(M1);
    tester.test("Copy constructor works",M1,M2);
    Matrix_t M3(std::move(M1));
    tester.test("Move constructor works",M2,M3);
    M5=M4;
    tester.test("Copy assignment works",M4,M5);
    M6=std::move(M4);
    tester.test("Move assignment works",M5,M6);
    tester.test("Not equal works",true,M5!=M1);
    tester.test("Has works true",true,M5.has(Irrep::B,Spin::beta));
    tester.test("Has works false",false,M5.has(Irrep::A,Spin::beta));
    tester.test("Get valid irreps works",std::set<Irrep>({Irrep::A}),
            M2.get_irreps());
    tester.test("Get valid spin works",std::set<int>({Spin::beta}),
            M5.get_spins(Irrep::B));
    tester.test("Get invalid spin works",std::set<int>({}),
            M5.get_spins(Irrep::A));
    using const_get=const EigenMatrixImpl&(Matrix_t::*)(Irrep,int)const;
    using get=EigenMatrixImpl&(Matrix_t::*)(Irrep,int);
    auto get1=bind(static_cast<const_get>(&Matrix_t::get),_1,_2,_3);
    auto get2=bind(static_cast<get>(&Matrix_t::get),_1,_2,_3);
    tester.test("Get invalid element throws",false,Mat1,get2,M5,Irrep::A,Spin::beta);
    tester.test("Const get works",true,Mat1,get1,M2,Irrep::A,Spin::alpha);
    tester.test("Const get invalid throws",false,Mat1,get1,M2,Irrep::B,Spin::alpha);
    M5.set(Irrep::B,Spin::alpha,std::move(Mat2));
    tester.test("Move set works",Mat3,M5.get(Irrep::B,Spin::alpha));
    tester.test("Same structure different",false,M5.same_structure(M6));
    tester.test("Same structure same",true,M2.same_structure(M3));
    std::cout<<bphash::hash_to_string(M2.my_hash())<<std::endl;
    
        
    tester.print_results();
}
    
    
    
    
