/*
 *@BEGIN LICENSE
 *
 * PSI4: an ab initio quantum chemistry software package
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 *@END LICENSE
 */

#include "tiledarray.h"
#include "Tensor.hpp"

namespace TA=TiledArray;
using namespace TensorWrap;
// Construct a second-order tensor tile that is filled with v
TA::Array<double,2>::value_type
make_tile(TA::Array<double, 2>::range_type& range, const double v) {
  // Construct a tile
  TA::Array<double, 2>::value_type tile(range);
  std::fill(tile.begin(), tile.end(), v);

  return tile;
}

TA::World* world;


// Fill array x with value v
void init_array(TA::Array<double, 2>& x, const double v) {
  // Add local tiles to a
  for(TA::Array<double, 4>::iterator it = x.begin(); it != x.end(); ++it) {
    // Construct a tile using a MADNESS task.
    TA::Future<TA::Array<double, 2>::value_type> tile =
        x.get_world().taskq.add(& make_tile, x.trange().make_tile_range(it.ordinal()), v);

    // Insert the tile into the array
    *it = tile;
  }
}
int main(int argc, char** argv){
   TA::World& temp=madness::initialize(argc,argv);
   world=&temp;
   // Construct tile boundary vector
     std::vector<std::size_t> tile_boundaries;
     for(std::size_t i = 0; i <= 16; i += 16)
       tile_boundaries.push_back(i);

     // Construct a set of TiledRange1's
     std::vector<TiledArray::TiledRange1>
       ranges(3, TiledArray::TiledRange1(tile_boundaries.begin(), tile_boundaries.end()));

     TiledArray::TiledRange1 r2(0,1);
     // Construct the 2D TiledRange
     TA::TiledRange trange(ranges.begin(), ranges.end());
     TA::TiledRange trange2(ranges.begin()+1, ranges.end());
     // Construct array objects.
     TA::Array<double, 3> a(*world, trange);
     TA::Array<double, 3> b(*world, trange);
     TA::Array<double, 2> c(*world, trange2);

     // Initialize a and b.
     a.fill_local(3.0);
     b.fill_local(2.0);

     // Print the content of input tensors, a and b.
     /*if(world.rank() == 0) {
       std::cout << "a = \n" << a << "\n";
       std::cout << "b = \n" << b << "\n";
     }*/

     // Compute the contraction c(m,n) = sum_k a(m,k) * b(k,n)
     c("m,n") = a("m,k,l") * b("k,l,n");


     // Print the result tensor, c.
     // if(world->rank() == 0)
     std::cout << "c = \n" << c << "\n";

     Tensor<double,3> A(16,16,16),B(16,16,16);
     Tensor<double,2> C(16,16),D(16,16);

     Tensor<double,3>::iterator EI=A.begin(),EEnd=A.end();
     for(;EI!=EEnd;++EI)*EI=3.0;

     B.Fill(2.0);

     C["i,k"]=A["i,j,l"]*B["j,l,k"];
     std::cout<<C;

     Tensor<double,0> T;
     T.Fill(2.0);

     D["i,k"]=(double)T*C["i,k"];
     std::cout<<D<<std::endl;


     // Wait for all the computation to complete before exiting
     world->gop.fence();
     madness::finalize();
   return 0;
}


