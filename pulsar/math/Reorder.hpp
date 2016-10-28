/*! \file
 *
 * \brief Functions related to basis function ordering (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#ifndef PULSAR_GUARD_MATH__REORDER_HPP_
#define PULSAR_GUARD_MATH__REORDER_HPP_

#include <algorithm>
#include <vector>
#include "pulsar/exception/Exceptions.hpp"

namespace pulsar{

/*! \brief Reorders a block of data
 *
 * The array \p neworder (of length \p size) contains indices of
 * the elements in \p src in the order that they should appear in the
 * destination buffer.
 *
 * Ie, src = ABCDEFG, neworder = { 3, 5, 1, 2, 4, 6, 0 }
 * results in DFBCEGA
 *
 * The \p width represents the number of elements for each entry. For example,
 * rows of a matrix can be reordered by setting width to the number of columns.
 *
 * \p niter is the number of times to do the reordering, advancing the pointers
 * by \p size elements each time. This allows for interchanging the columns of a matrix
 * by setting width to 1 and \p niter to the number of rows.
 *
 * \param [in] neworder Ordering to change to
 * \param [in] size Number of elements to reorder (ie, the length of neworder)
 * \param [in] src A buffer containing the source data
 * \param [in] dest Where to put the reordered data
 * \param [in] width The number of elements each entry contains.
 * \param [in] niter Number of times to perform the ordering
 *
 * \warning \p src should not share any memory with \p buf. That is,
 *                 they should not be aliased.
 *
 * \note \p src must be at least \p width * \p size * \p niter elements.
 *       \p dest is expected to be that size as well
 */
template<typename T>
void reorder_block(size_t const * neworder, size_t size,
                   T const * RESTRICT src,
                   T * RESTRICT dest,
                   size_t width, size_t niter) noexcept
{
    for(size_t n = 0; n < niter; n++)
    {
        for(size_t i = 0; i < size; i++)
        {
            size_t idx = neworder[i];
            std::copy(src + idx*width,
                      src + idx*width + width,
                      dest + i*width);
        }

        dest += size*width;
        src += size*width;
    }
}



/*! \brief Reorders a block of data
 *
 * The vector \p neworder contains indices of
 * the elements in \p src in the order that they should appear in the
 * destination buffer.
 *
 * Ie, src = ABCDEFG, neworder = { 3, 5, 1, 2, 4, 6, 0 }
 * results in DFBCEGA
 *
 * The \p width represents the number of elements for each entry. For example,
 * rows of a matrix can be reordered by setting width to the number of columns.
 *
 * \p niter is the number of times to do the reordering, advancing the pointers
 * by \p size elements each time. This allows for interchanging the columns of a matrix
 * by setting width to 1 and \p niter to the number of rows.
 *
 * \param [in] neworder Ordering to change to
 * \param [in] src A buffer containing the source data
 * \param [in] dest Where to put the reordered data
 * \param [in] width The number of elements each entry contains.
 * \param [in] niter Number of times to perform the ordering
 *
 * \warning \p src should not share any memory with \p buf. That is,
 *                 they should not be aliased.
 *
 * \note \p src must be at least \p width * \p neworder.size() * \p niter
 *       elements. \p dest is expected to be that size as well
 */
template<typename T>
void reorder_block(const std::vector<size_t> & neworder,
                   T const * RESTRICT src,
                   T * RESTRICT dest,
                   size_t width, size_t niter) noexcept
{
    reorder_block(neworder.data(), neworder.size(),
                  src, dest, width, niter);
}



/*! \brief Makes a vector containing the order of a container
 *         with respect to another
 *
 * For every element in \p dest, stores the index of the first location
 * where that element exists in \p src.
 * 
 * For example, src = ABCDEFG, dest = DFBCEGA, will
 * return a vector 3512460. Passing this vector
 * to reorder_block will transform \p src into \p dest 
 *
 * \throw pulsar::MathException if the element of \p dest
 *        is not found in \p src
 */
template<typename T>
std::vector<size_t> make_ordering(const std::vector<T> & src,
                                  const std::vector<T> & dest)
{
    std::vector<size_t> ret;
    ret.reserve(dest.size());

    for(const auto & it : dest)
    {
        auto to_pos = std::find(src.begin(), src.end(), it);
        if(to_pos == src.end())
            throw MathException("Cannot find element in source vector");

        ret.push_back(std::distance(src.begin(), to_pos));
    }

    return ret;
}

} // close namespace pulsar


#endif
