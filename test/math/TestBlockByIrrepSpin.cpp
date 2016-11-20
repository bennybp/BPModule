#include <pulsar/testing/CppTest.hpp>
#include <pulsar/math/EigenImpl.hpp>

using namespace pulsar;
using EigenMat=Eigen::MatrixXd;
using shared_eigen=std::shared_ptr<EigenMat>;
using shared_matrix=std::shared_ptr<EigenMatrixImpl>;
using Matrix_t=BlockedEigenMatrix;

TEST_SIMPLE(TestBlockByIrrepSpin){
    CppTester tester("Testing BlockByIrrepSpin with BlockedEigenMatrix.");
    
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
    TEST_FXN("Set/Get elements works",true,Mat1,M1.get(iA,sa));
    Matrix_t M2(M1);
    tester.test_equal("Copy constructor works",M1,M2);
    Matrix_t M3(std::move(M1));
    tester.test_equal("Move constructor works",M2,M3);
    M5=M4;
    tester.test_equal("Copy assignment works",M4,M5);
    M6=std::move(M4);
    tester.test_equal("Move assignment works",M5,M6);
    tester.test_equal("Not equal works",true,M5!=M1);
    TEST_FXN("Has works true",true,true,M5.has(iB,sb));
    TEST_FXN("Has works false",true,false,M5.has(iA,sb));
    TEST_FXN("Get valid irreps works",true,std::set<Irrep>({iA}),M2.get_irreps());
    TEST_FXN("Get valid spin works",true,std::set<int>({sb}),M5.get_spins(iB));
    tester.test_equal("Get invalid spin works",std::set<int>({}),M5.get_spins(iA));
    TEST_FXN("Get invalid element throws",false,Mat1,M5.get(iA,sb));
    const Matrix_t& M2c=M2;
    TEST_FXN("Const get works",true,Mat1,M2c.get(iA,sa));
    TEST_FXN("Const get invalid throws",false,Mat1,M2c.get(iB,sa));
    TEST_VOID("Move set works",true,M5.set(iB,sa,std::move(Mat2)));
    TEST_FXN("Move set value is correct",true,*Mat3,*M5.get(iB,sa));
    TEST_FXN("Same structure different",true,false,M5.same_structure(M6));
    TEST_FXN("Same structure same",true,true,M2.same_structure(M3));
    TEST_FXN("Hash works 1",true,M2.my_hash(),M3.my_hash());
     
    tester.print_results();
    return tester.nfailed();
}
    
    
    
    
