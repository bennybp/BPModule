#ifndef TENSORLIB_SHAPE_HPP_
#define TENSORLIB_SHAPE_HPP_
#include<array>

namespace TensorWrap {

/*! \brief A class to keep track of the dimensions of our tensor
 *
 *   Initially when a user builds a tensor it is of some rank.
 *   For a given dimension, say \f$i\f$, which is of length
 *   \f$N_i\f$, \f$i\f$ runs from \f$[0, N_i)\f$.  Things get more complicated
 *   when we start ascribing meanings to subspaces.  For two tensors, \f$A\f$
 *   and \f$B\f$, both of rank \f$R\f$ we can create a tensor of rank
 *   \f$R+2\f$ by performing a direct summation.  For each of the dimensions
 *   the range is now \f$[0,A_i)[A_i,B_i)\f$.  This class keeps track of
 *   that info for us.
 */
class Shape {
   public:
      Shape& operator+=(const Shape& Other) {
         size_t MySize=Shape_.size();
         //Initial direct sum
         if (MySize==Other.Shape_.size()) {
            Combine(MySize, Other);
            //Add two dimensions
            for (size_t t=0; t<2; t++)
               Shape_.push_front(Boundry_t({0, 1, 2}));
         }
         //Subsequent direct sum
         else if (MySize==Other.Shape_.size()+2) {
            for (size_t t=0; t<MySize; t++) {
               if (t<2) Shape_[t].push_back(Shape_[t].size());
               else Combine(MySize, Other, 2);
            }
         }
         return *this;
      }

      template <size_t Rank>
      void Fill(const std::array<size_t, Rank>& Idx) {
         for (size_t i=0; i<Idx.size(); ++i)
            Shape_.push_back(Boundry_t({0, Idx[i]}));
      }

      template <size_t Rank>
      Shape(const std::array<size_t, Rank>& Idx) {
         Fill(Idx);
      }

      Shape() {
      }

      size_t size()const{return Shape_.size();}

      const std::vector<size_t>& operator[](size_t i)const{return Shape_[i];}

      std::ostream& operator<<(std::ostream& os)const{
         for(size_t i=0; i<Shape_.size();i++){
            os<<"Rank: "<<i<<" [";
            for(size_t j=0;j<Shape_[i].size();j++)
               os<<Shape_[i][j]<<(j!=Shape_[i].size()-1?",":")");
            os<<std::endl;
         }
         return os;
      }

   private:
      void Combine(size_t MySize, const Shape& Other, size_t off=0) {
         for (size_t t=off; t<MySize; t++) {
            size_t Offset=Shape_[t].back();
            for (size_t s=1; s<Other.Shape_[t-off].size(); s++) //s=0 is 0...
               Shape_[t].push_back(Offset+Other.Shape_[t-off][s]);
         }
      }

      typedef std::vector<size_t> Boundry_t;
      typedef std::deque<Boundry_t> Shape_t;

      Shape_t Shape_;
};

inline std::ostream& operator<<(std::ostream& os,const Shape& sha){
   return sha<<os;
}

} //End namespace TensorWrap

#endif /* TENSORLIB_SHAPE_HPP_ */
