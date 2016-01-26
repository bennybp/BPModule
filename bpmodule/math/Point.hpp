/* 
 * File:   Point.hpp
 * Original Author: Ryan Richard <ryanmrichard1@gmail.com>
 *
 * Created on January 22, 2016, 11:46 AM
 */

#ifndef BPMODULE_GUARD_POINT_HPP_
#define BPMODULE_GUARD_POINT_HPP_

#include <array>
#include <vector>
#include <map>
#include <memory>

namespace bpmodule{
namespace math{

template<typename Key,typename T>
class Point;

template<typename Key,typename T>
class PointStorage;


/** \brief An iterator to a PointStorage class
 * 
 *  This iterator is only valid as long as the PointStorage class is valid.
 */
template<typename Key,typename T>
class PSItr{
private:
    size_t Idx_=0;
    friend PointStorage<Key,T>;
    typedef std::shared_ptr<PointStorage<Key,T> > Parent_t;
    Parent_t Parent_;
    PSItr(size_t start,Parent_t Parent):Idx_(start),Parent_(Parent){
    }
public:
    PSItr()=default;
    const PSItr& operator++(){
        ++Idx_;
        return *this;
    }

    Point<Key,T> operator*()const{
        return Point<Key,T>(Idx_,Parent_);
    }

    bool operator==(const PSItr& other)const{
        return Idx_==other.Idx_;
    }
};

/** \brief The class that holds the memory for a point's 
 *   coordinates and weights.
 * 
 * \param Key The type of the weight lookup key
 * \param T The type of the weights must be default constructable
 */
template<typename Key,typename T>
class PointStorage: public std::enable_shared_from_this<PointStorage<Key,T> >{
private:
    ///A 3 by NPoints array of coordinates
    std::vector<double> Carts_;

    ///A nkeys by NPoints array of weights
    std::map<Key,std::vector<T> > Weights_;

public:
    typedef PSItr<Key,T> iterator;
    typedef PSItr<Key,T> const_iterator;
    
    virtual ~PointStorage()=default;
    
    iterator begin(){
        return iterator(0,this->shared_from_this());
    }
    const_iterator begin()const{
        return const_iterator(0,this->shared_from_this());
    }
    iterator end(){
        return iterator(size(),this->shared_from_this());
    }
    const_iterator end()const{
        return const_iterator(size(),this->shared_from_this());
    }
    
    
    /** \brief Inserts the new value at the end (ignores iterator) of the point
     *         storage class, returns a modified copy as if it were an iterator
     *         to the added point.
     * 
     * For all this magic to work Value's type must derive from Point<Key,T>.
     * We first add value's data to the PointStorage class.  Then we default
     * consturct a new U follwed by abuse of the fact that U derives from
     * Point<Key,T>, with which we are friends, to set U's index and
     * StorageClass.  Finally we return U as a unique_ptr so that its return
     * type will be dereferencerable to U.
     * 
     * \param U must derive from Point<Key,T> and be default constructable
     */
    template<typename U>
    std::unique_ptr<U> insert(const_iterator,const U& Value){
        for(size_t i=0;i<3;i++)
            Carts_.push_back(Value[i]);
        typename std::map<Key,T>::const_iterator WI=Value.Weights_.begin(),
                WEnd=Value.Weights_.end();
        for(;WI!=WEnd;++WI){
            Weights_[WI->first].resize(size()-1);
            Weights_[WI->first].push_back(WI->second);                
        }
        std::unique_ptr<U> NewValue(new U);
        NewValue->Storage_=this->shared_from_this();
        NewValue->Idx_=size()-1;
        return NewValue;
    }

    PointStorage()=default;
    ///The number of points
    size_t size()const{return Carts_.size()/3;}
    ///The number of weights
    size_t NWeights()const{return Weights_.size();}
    ///Returns the j-th coordinate of the i-th point
    const double& operator()(size_t i,size_t j)const{
        return Carts_[i*3+j];
    }
    ///Returns the j-th coordinate of the i-th point
    double& operator()(size_t i,size_t j){
        return Carts_[i*3+j];
    }
    ///Returns the weight of the i-th point
    T& Weight(const Key& AKey,size_t i){
        return Weights_[AKey][i];
    }
    ///Returns the weight of the i-th point (const version)
    const T& Weight(const Key& AKey,size_t i)const{
        return Weights_.at(AKey)[i];
    }

};

/** \brief An interface to a point
 * 
 * Each point can be a stand alone point, which is what the user creates, or
 * they can be tied to a PointStorage class.  In the latter case you are
 * able to say grab the address of the first cart of the first atom and know
 * that you have a 3*NPoints long array of contigious memory.  Similar stories
 * hold for the weights, per key.  This is an
 * advanced feature and we highly recommend that normal users ignore the fact
 * that the return types are references.
 */
template<typename Key,typename T>
class Point{
private:
    ///The index of this point
    size_t Idx_;

    ///Local buffers for object initialization
    std::array<double,3> Carts_;
    std::map<Key,T > Weights_;
    
    ///The actual data
    std::shared_ptr<PointStorage<Key,T> > Storage_;
    
    friend PSItr<Key,T>;
    friend PointStorage<Key,T>;
    Point(size_t Index,std::shared_ptr<PointStorage<Key,T> > Storage):
        Idx_(Index),Storage_(Storage){}
public:
    ///Makes a null point
    Point():
        Idx_(0),Carts_({0.0,0.0,0.0}){}
    Point(double x,double y,double z):
        Idx_(0),Carts_({x,y,z}){
    }
        
    virtual ~Point()=default;
        
    ///Adds a weight to this point
    void AddWeight(const Key& AKey,const T& Value){
        Weights_[AKey]=Value;
    }
    ///Returns the number of weights
    size_t NWeights()const{
        return Storage_?Storage_->NWeights():Weights_.size();
    }
    ///Returns the weight with the given key
    T& Weight(const Key& AKey){
        return Storage_?Storage_->Weight(AKey,Idx_):Weights_[AKey];
    }
    ///Returns the weight with the given key (const version)
    const T& Weight(const Key& AKey)const{
        return Storage_?Storage_->Weight(AKey,Idx_):Weights_.at(AKey);
    }
    ///Returns the i-th Cartesian coordinate of the point (const version)
    const double& operator[](size_t i)const{
        return Storage_?(*Storage_)(Idx_,i):Carts_[i];
    }
    ///Returns the i-th Cartesian coordinate of the point
    double& operator[](size_t i){
        return Storage_?(*Storage_)(Idx_,i):Carts_[i];
    }
    ///A comparison operator so we can stick these in an std::set, just compares
    ///addresses
    bool operator<(const Point<Key,T>& RHS)const{
        return this<&RHS;
    }
};

}
}//End namespaces

#endif /* BPMODULE_GUARD_POINT_HPP_ */

