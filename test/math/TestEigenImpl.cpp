#include "TestCXX.hpp"
#include <pulsar/math/EigenImpl.hpp>

using Matrix_t=pulsar::math::EigenMatrixImpl;
using Vector_t=pulsar::math::EigenVectorImpl;
using std::bind;
using namespace std::placeholders;

TEST_CLASS(TestEigenImpl){
    Tester tester("Testing Eigen Matrix and Vector Implementations.");
    Matrix_t::shared_matrix Mat1=std::make_shared<Matrix_t::matrix_type>(2,4);
    Matrix_t::matrix_type Mat2(2,4);
    (*Mat1)<<1.0,2.0,3.0,4.0,
          5.0,6.0,7.0,8.0;
    Mat2=*Mat1;
    Matrix_t M1(Mat1),M2(Mat2);
    Matrix_t M3(std::move(Mat2));
    tester.test("Matrix shared pointer constructor works",*Mat1,*M1.get_matrix());
    tester.test("Matrix copy constructor works",M1,M2);
    tester.test("Matrix move constructor works",M1,M3);
    tester.test("Matrix sizes works",std::array<size_t,2>({2,4}),M1.sizes());
    tester.test("Matrix get_value works",3.0,M2.get_value({0,2}));
    M3.set_value({0,2},55.0);
    tester.test("Matrix set_value works",55.0,M3.get_value({0,2}));
    tester.test("Matrix != works",true,M3!=M1);
    auto Mat4=pulsar::math::convert_to_eigen(M1);
    tester.test("Matrix convert to eigen works",*Mat1,*Mat4);
    const std::string m_hash="2844c428d93543305ca764952c28570e";
    tester.test("Matrix hash is right 1",m_hash,bphash::hash_to_string(M1.my_hash()));
    tester.test("Matrix hash is right 2",m_hash,bphash::hash_to_string(M2.my_hash()));
    
    Vector_t::shared_vector Vec1=std::make_shared<Vector_t::vector_type>(4);
    Vector_t::vector_type Vec2(4);
    (*Vec1)<<1.0,2.0,3.0,4.0;
    Vec2=*Vec1;
    Vector_t V1(Vec1),V2(Vec2);
    Vector_t V3(std::move(Vec2));
    tester.test("Vector shared pointer constructor works",*Vec1,*V1.get_matrix());
    tester.test("Vector copy constructor works",V1,V2);
    tester.test("Vector move constructor works",V1,V3);
    tester.test("Vector sizes works",std::array<size_t,1>({4}),V1.sizes());
    tester.test("Vector get_value works",3.0,V2.get_value({2}));
    V3.set_value({2},55.0);
    tester.test("Vector set_value works",55.0,V3.get_value({2}));
    tester.test("Vector != works",true,V3!=V1);
    auto Vec4=pulsar::math::convert_to_eigen(V1);
    tester.test("Vector convert to eigen works",*Vec1,*Vec4);
    const std::string v_hash="ad6127d08f5fe6a7504bfd1c8dfd9cdb";
    tester.test("Vector hash is correct 1",v_hash,bphash::hash_to_string(V1.my_hash()));
    tester.test("Vector hash is correct 2",v_hash,bphash::hash_to_string(V2.my_hash()));

    tester.print_results();
}
