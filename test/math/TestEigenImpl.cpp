#include <pulsar/testing/CppTest.hpp>
#include <pulsar/math/EigenImpl.hpp>

using namespace pulsar;
using Matrix_t=pulsar::EigenMatrixImpl;
using Vector_t=pulsar::EigenVectorImpl;

TEST_SIMPLE(TestEigenImpl){
    CppTester tester("Testing Eigen Matrix and Vector Implementations.");
    Matrix_t::shared_matrix Mat1=std::make_shared<Matrix_t::matrix_type>(2,4);
    Matrix_t::matrix_type Mat2(2,4);
    (*Mat1)<<1.0,2.0,3.0,4.0,
          5.0,6.0,7.0,8.0;
    Mat2=*Mat1;
    Matrix_t M1(Mat1),M2(Mat2);
    Matrix_t M3(std::move(Mat2));
    tester.test_equal("Matrix shared pointer constructor works",*Mat1,*M1.get_matrix());
    tester.test_equal("Matrix copy constructor works",M1,M2);
    tester.test_equal("Matrix move constructor works",M1,M3);
    std::array<size_t,2> twofour({2,4}),zerotwo({0,2});
    TEST_FXN("Matrix sizes works",true,twofour,M1.sizes());
    TEST_FXN("Matrix get_value works",true,3.0,M2.get_value(zerotwo));
    TEST_VOID("Matrix set_value works",true,M3.set_value(zerotwo,55.0));
    TEST_FXN("Matrix set_value is correct",true,55.0,M3.get_value(zerotwo));
    tester.test_equal("Matrix != works",true,M3!=M1);
    auto Mat4=pulsar::convert_to_eigen(M1);
    tester.test_equal("Matrix convert to eigen works",*Mat1,*Mat4);
    tester.test_equal("Matrix hash",M1.my_hash(),M2.my_hash());
    
    Vector_t::shared_vector Vec1=std::make_shared<Vector_t::vector_type>(4);
    Vector_t::vector_type Vec2(4);
    (*Vec1)<<1.0,2.0,3.0,4.0;
    Vec2=*Vec1;
    Vector_t V1(Vec1),V2(Vec2);
    Vector_t V3(std::move(Vec2));
    tester.test_equal("Vector shared pointer constructor works",*Vec1,*V1.get_matrix());
    tester.test_equal("Vector copy constructor works",V1,V2);
    tester.test_equal("Vector move constructor works",V1,V3);
    std::array<size_t,1> four({4}),two({2});
    TEST_FXN("Vector sizes works",true,four,V1.sizes());
    TEST_FXN("Vector get_value works",true,3.0,V2.get_value(two));
    TEST_VOID("Vector set_value works",true,V3.set_value(two,55.0));
    TEST_FXN("Vector set_value is correct",true,55.0,V3.get_value(two));
    tester.test_equal("Vector != works",true,V3!=V1);
    auto Vec4=pulsar::convert_to_eigen(V1);
    tester.test_equal("Vector convert to eigen works",*Vec1,*Vec4);
    tester.test_equal("Vector hash",V1.my_hash(),V2.my_hash());

    tester.print_results();
    return tester.nfailed();
}
