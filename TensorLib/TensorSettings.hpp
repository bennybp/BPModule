#ifndef TENSORSETTINGS_HPP_
#define TENSORSETTINGS_HPP_

/*! \file This file contains settings that depend on
 *  the tensor library that is the back-end
 *
 *
 *  Regardless of which back-end we use the
 *  primary tensor will be of type TensorBase.
 *
 *  Syntactically we will then use the index
 *  operator (i.e. operator[]) to return an
 *  IndexedTensor.
 *
 *
 */


#define TILED_ARRAY 1
#if TILED_ARRAY

#include "tiledarray.h"

///Our wrapper to TA
#include "TAImpl.hpp"

namespace TensorWrap{

template<typename Data_t,size_t Rank>
using TImpl=TAImpl<Data_t,Rank>;

}
#endif /*TILED_ARRAY*/




#endif /* TENSORSETTINGS_HPP_ */
