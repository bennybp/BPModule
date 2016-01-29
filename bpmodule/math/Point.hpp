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
#include "bpmodule/pragma.h"

namespace bpmodule{
namespace math{

template<typename Point_t>
class PointStorage;

    
/** \brief An interface to a point
 * 
 * The philosophy here is that we have a bunch of weighted Cartesian points,
 * that we want to be able to treat individually, but also want to be able to
 * store contigiously.  Furthermore we want the Point to be the interface
 * with which we interact not the memory manager.  To do this we store
 * shared_ptrs to both the weights and the Carts.  When we make the pont
 * we allocate these pointers, we then put the values there.  If we never 
 * register the point that's the end of the story.  If/when the point goes to
 * a PointStorage, it is added to that classes buffer, the index switched,
 * and pointers to the buffers switched.
 * 
 * Now
 * we make some assumptions about this point instance, specifically that one of
 * the following is true:
 *  1) This point will forever be by itself and will never be registered
 *  2) This point will be registered to one and only one PointStorage class
 * The above are on a per instance base, so if you copy the point you can put it
 * in another PointStorage class.
 * 
 * 
 * \param Key The type of the object that will be used to look up weights
 * \param T The type of the weight
 */
template<typename Key,typename T,typename Parent=Point<Key,T>>
class Point{
public:
    ///@{
    ///The following two types are primarily for template meta-programming
    ///Feel free to use them if you want
    
    ///The type of the key
    typedef Key Key_t;
    ///The type of the data
    typedef T Data_t;
    ///@}
private:
    ///My type
    typedef Point<Key,T> My_t;
    ///Type of the storage container for the carts
    typedef std::vector<double> Carts_t;
    ///Type of the storage container for the weights
    typedef std::map<Key_t,std::vector<T>> Weight_t;
    
    ///The index of this point in the storage containers
    size_t Idx_;
    
    friend PointStorage<Parent>;
    ///Buffers for Carts and Weights
    std::shared_ptr<Carts_t> Carts_;
    std::shared_ptr<Weight_t> Weights_;
    
public:
    ///Makes a null point
    Point():Idx_(0),Carts_(new Carts_t({0.0,0.0,0.0})){}
    ///Makes a point with the given Carts
    Point(double x,double y,double z):Idx_(0),Carts_(new Carts_t({x,y,z})){
    }
    
    ///Shallow copies other point
    Point(const My_t& /*RHS*/)=default;
        
    ///Normal clean-up is fine
    virtual ~Point()=default;

    ///@{
    ///Sets the carts of the point to the given values
    /// Iterator variant copies range (Qi,QEnd] range should be 3 elements long
    template<typename BeginItr,typename EndItr>
    void SetCarts(BeginItr Qi,EndItr QEnd){
        for(size_t i=0;Qi!=QEnd;++QEnd,++i)(*Carts_)[Idx_*3+i]=*Qi;
    }
    
    ///Explicit component version
    void SetCarts(double x, double y, double z){
        std::array<double,3> temp({x,y,z});
        SetCarts(temp.begin(),temp.end());
    }
    ///@}
    
    
    /** \brief Adds a weight to this point
     * 
     * First we check if weights has been allocated, if not we allocate it.
     * Second we check if weights has the weight type we are adding
     *       if not we add a vector of the values
     * Third we add our value.
     * 
     * If we are not linked to 
     */ 
    void AddWeight(const Key_t& AKey,const Data_t& Value){
        if(!Weights_)Weights_=std::shared_ptr<Weight_t>(new Weight_t);
        //In case we are adding a completely new weight
        size_t NP=Carts_->size()/3;
        if(Weights_->count(AKey)==0)
            (*Weights_)[AKey]=std::vector<T>(NP);
        (*Weights_)[AKey][Idx_]=Value;
    }
    
    ///@{
    /** Iterators to the weights (there's three Cartesian coordinates, you don't
     * need iterators to those).  Only constant versions because you should add
     * weights via the AddWeight fxn
     */
    typedef typename std::map<Key_t,Data_t>::const_iterator const_iterator;
    const_iterator begin()const{return Weights_.begin();}
    const_iterator end()const{return Weights_.end();}
    ///@}
    
    ///Returns the number of weights
    size_t NWeights()const{return Weights_->size();}
    
    ///Returns the weight with the given key by copy
    Data_t Weight(const Key_t& AKey)const{return Weights_->at(AKey)[Idx_];}
    
    ///Returns the i-th Cartesian coordinate of the point by copy
    double operator[](size_t i)const{return (*Carts_)[i+Idx_*3];}
};

/** \brief The class that holds the memory for a point's 
 *   coordinates and weights.
 * 
 *   Saying this class holds the memory is an over simplification.  What really
 *   happens is it synchronizes as points are passed to it.  Specifically it
 *   starts with null pointers.  When the first point comes in it adopts that
 *   point's buffers.  Then all subsequent points that come in are added to
 *   that buffer and told their index and given a pointer to that buffer.
 * 
 *   Here's the various interactions for Points a,b,c,etc. and PointStorages
 *   A,B,C,etc.
 *   
 *   1) a belongs to no PointStorage, we add it to a null A
 *      Result.  A adopts a's buffer, a remains index 0.
 *   2) a belongs to no PointStorage, we add it to a non-null A
 *      Result. a's data is added to A's buffer and a's buffers are set to those
 *              of A.
 *   3) a belongs to A, and we add it to a null B
 *      Result. B adopts a's buffer, a maintains its index
 *   4) a belongs to A, and we add it to a non-null B
 *      Result. a's data is added to B and a's buffers are set to those of B.
 *   
 */
template<typename Point_t>
class PointStorage {
private:
    //Establish the types
    ///The type of the keys
    typedef typename Point_t::Key_t Key_t;
    ///The type of the Data
    typedef typename Point_t::Data_t Data_t;
    ///The type of the Carts
    typedef std::vector<double> Carts_t;
    ///The type of the Weights
    typedef std::map<Key_t,std::vector<Data_t>> Weight_t;
    
    ///A 3 by NPoints array of coordinates
    std::shared_ptr<Carts_t> Carts_;

    ///A nkeys by NPoints array of weights
    std::shared_ptr<Weight_t> Weights_;
    
    ///Points we know of
    std::vector<Point_t> Points_;

public:
    typedef typename std::vector<Point_t>::iterator iterator;
    typedef typename std::vector<Point_t>::const_iterator const_iterator;
    
    ///Standard destructor is fine
    virtual ~PointStorage()=default;
    
    iterator begin(){return Points_.begin();}
    const_iterator begin()const{return Points_.begin();}
    iterator end(){return Points_.end();}
    const_iterator end()const{return Points_.end();}
    
    const_iterator insert(const_iterator,const Point<Key_t,Data_t>& Value){
        Point_t& NewValue=std::const_cast<Point_t&>(Value);
        if(!Carts_)Carts_=NewValue.Carts_;
        else{
            for(size_t i=0;i<3;i++)Carts_->push_back((*Value.Carts_)[i]);
            NewValue.Carts_=Carts_;
        }
        if(!Weights_)Weights_=NewValue.Weights_;
        else{
            for(const Weight_t& Wi : *Value.Weights_){
                if(Weights_->count(Wi.first)!=1)
                    (*Weights_)[Wi.first]=std::vector<Data_t>(size()+1);
                else (*Weights_)[Wi.first].resize(size()+1); 
                (*Weights_)[Wi.first][size()]=Wi.second;
            }
            NewValue.Weights_=Weights_;
        }
        NewValue.Idx_=Points_.size();
        return Points_.insert(Points_.end(),NewValue);
        
    }

    PointStorage()=default;
    
    ///The number of points
    size_t size()const{return Points_.size();}
    
    ///
    Point_t operator[](size_t i)const{return Points_[i];}
};

}
}//End namespaces

#endif /* BPMODULE_GUARD_POINT_HPP_ */

