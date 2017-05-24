#include "pulsar/math/EigenImpl.hpp"
#include <cereal/archives/binary.hpp>
using Eigen::MatrixXd;
using Eigen::VectorXd;

namespace pulsar {

std::shared_ptr<const MatrixXd>
convert_to_eigen(const MatrixDImpl & ten)
{
    // does the TensorImpl contain an eigen matrix?
    auto test = dynamic_cast<const EigenMatrixImpl*>(&ten);
    if(test)
        return test->get_matrix();

    // otherwise, convert elementwise
    auto sizes = ten.sizes();
    auto ret = std::make_shared<MatrixXd>(sizes[0],sizes[1]);


    for(size_t i = 0; i < sizes[0]; i++)
    for(size_t j = 0; j < sizes[1]; j++)
        (*ret)(i,j) = ten.get_value({i,j});
    return ret;
}

std::shared_ptr<const VectorXd>
convert_to_eigen(const VectorDImpl & ten)
{
    // does the TensorImpl contain an eigen matrix?
    auto test = dynamic_cast<const EigenVectorImpl*>(&ten);
    if(test)
        return test->get_matrix();

    // otherwise, convert elementwize
    auto ret = std::make_shared<VectorXd>();

    auto sizes = ten.sizes();
    for(size_t i = 0; i < sizes[0]; i++)
        (*ret)(i) = ten.get_value({i});
    return ret;
}
} // close namespace pulsar

#include <cereal/archives/portable_binary.hpp>
CEREAL_REGISTER_TYPE(pulsar::EigenMatrixImpl)
CEREAL_REGISTER_TYPE(pulsar::EigenVectorImpl)
