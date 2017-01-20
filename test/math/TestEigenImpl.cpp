#include <pulsar/testing/CppTester.hpp>
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
    tester.test_member_return("Matrix sizes works",true,twofour,&Matrix_t::sizes,&M1);
    tester.test_member_return("Matrix get_value works",true,3.0,
                              &Matrix_t::get_value,&M2,zerotwo);
    tester.test_member_call("Matrix set_value works",true,&Matrix_t::set_value,
                            &M3,zerotwo,55.0);
    tester.test_member_return("Matrix set_value is correct",true,55.0,
                              &Matrix_t::get_value,&M3,zerotwo);
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
    tester.test_member_return("Vector sizes works",true,four,
                              &Vector_t::sizes,&V1);
    tester.test_member_return("Vector get_value works",true,3.0,
                              &Vector_t::get_value,&V2,two);
    tester.test_member_call("Vector set_value works",true,&Vector_t::set_value,
                            &V3,two,55.0);
    tester.test_member_return("Vector set_value is correct",true,55.0,
                              &Vector_t::get_value,&V3,two);
    tester.test_equal("Vector != works",true,V3!=V1);
    auto Vec4=pulsar::convert_to_eigen(V1);
    tester.test_equal("Vector convert to eigen works",*Vec1,*Vec4);
    tester.test_equal("Vector hash",V1.my_hash(),V2.my_hash());

    tester.print_results();
    return tester.nfailed();
}
