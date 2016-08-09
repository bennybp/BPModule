#include "pulsar/math/EigenImpl.hpp"
#include <cereal/archives/binary.hpp>
using Eigen::MatrixXd;
using Eigen::VectorXd;

using namespace pulsar::math;

namespace pulsar {
namespace math {

std::shared_ptr<const MatrixXd>
convert_to_eigen(const std::shared_ptr<const MatrixDImpl> & ten)
{
    // does the TensorImpl contain an eigen matrix?
    auto test = std::dynamic_pointer_cast<const EigenMatrixImpl>(ten);
    if(test)
        return test->get_matrix();

    // otherwise, convert elementwise
    auto ret = std::make_shared<MatrixXd>();

    auto sizes = ten->sizes();
    for(size_t i = 0; i < sizes[0]; i++)
    for(size_t j = 0; j < sizes[1]; j++)
        (*ret)(i,j) = ten->get_value({i,j});
    return ret;
}

std::shared_ptr<const VectorXd>
convert_to_eigen(const std::shared_ptr<const VectorDImpl> & ten)
{
    // does the TensorImpl contain an eigen matrix?
    auto test = std::dynamic_pointer_cast<const EigenVectorImpl>(ten);
    if(test)
        return test->get_matrix();

    // otherwise, convert elementwize
    auto ret = std::make_shared<VectorXd>();

    auto sizes = ten->sizes();
    for(size_t i = 0; i < sizes[0]; i++)
        (*ret)(i) = ten->get_value({i});
    return ret;
}


} // close namespace math
} // close namespace pulsar