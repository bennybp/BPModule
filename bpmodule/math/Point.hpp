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

    template<typename T>
    class PointStorage;
    
/** \brief An interface to a point
 * 
 * Each point can be a stand alone point, which is what the user creates, or
 * they can be tied to a PointStorage class.  They have identical interfaces.
 * The only difference is whether or not it is registered to a PointStorage
 * container.  Even if it is registered, there is no way for the user to
 * modify the PointStorage container from this class.  When points are
 * inserted into a PointStorage class they become registered to it
 * 
 */
template<typename Key,typename T>
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
    ///Type of my storage
    typedef PointStorage<My_t> Storage_t;
    
    ///The index of this point
    size_t Idx_;

    ///Local buffers for object initialization
    std::array<double,3> Carts_;
    std::map<Key_t,Data_t > Weights_;
    
    ///The actual data storage
    std::shared_ptr<Storage_t> Storage_;
    
    friend  PointStorage<My_t>;
    ///Constructor for linking this point to a PointStorage, only callable by a
    ///PointStorage instance
    Point(size_t Index,std::shared_ptr<Storage_t> Storage):
        Idx_(Index),Storage_(Storage){}
public:
    ///Makes a null point
    Point():
        Idx_(0),Carts_({0.0,0.0,0.0}){}
    ///Makes a point with the given Carts
    Point(double x,double y,double z):
        Idx_(0),Carts_({x,y,z}){
    }
    ///Shallow copies other point
    Point(const My_t& /*RHS*/)=default;
        
    ///Normal clean-up is fine
    virtual ~Point()=default;
        
    ///Adds a weight to this point
    void AddWeight(const Key_t& AKey,const Data_t& Value){
        Weights_[AKey]=Value;
    }
    ///@{
    /** Iterators to the weights (there's three Cartesian coordinates, you don't
     * need iterators to those).  Only constant cause you should add weights via
     * the AddWeight fxn
     */
    typedef typename std::map<Key_t,Data_t>::const_iterator const_iterator;
    const_iterator begin()const{return Weights_.begin();}
    const_iterator end()const{return Weights_.end();}
    ///@}
    ///Returns the number of weights
    size_t NWeights()const{
        return Storage_? Storage_->NWeights():Weights_.size();
    }
    ///Returns the weight with the given key by copy
    Data_t Weight(const Key_t& AKey)const{
        return Storage_?Storage_->Weight(AKey,Idx_):Weights_.at(AKey);
    }
    ///Returns the i-th Cartesian coordinate of the point by copy
    double operator[](size_t i)const{
        return Storage_?(*Storage_)(Idx_,i):Carts_[i];
    }
};
    
/** \brief The class that holds the memory for a point's 
 *   coordinates and weights.
 * 
 * \param Key The type of the weight lookup key
 * \param T The type of the Points that are going in this class, must derive
 *          from the point class below.
 */
template<typename Point_t>
class PointStorage:
    public std::enable_shared_from_this<PointStorage<Point_t>>{
private:
    typedef typename Point_t::Key_t Key_t;
    typedef typename Point_t::Data_t Data_t;
    
    ///A 3 by NPoints array of coordinates
    std::vector<double> Carts_;

    ///A nkeys by NPoints array of weights
    std::map<Key_t,std::vector<Data_t>> Weights_;
    
    ///A vector of the wrapper Points
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
        /*for(size_t i=0;i<3;i++) Carts_.push_back(Value[i]);
        for(const typename std::pair<Key_t,Data_t>  WI : Value){
            Weights_[WI.first].resize(size()-1);
            Weights_[WI.first].push_back(WI.second);                
        }
        std::unique_ptr<PBase_t>Point_t NewValue;
        NewValue.Idx_=size()-1;
        NewValue.Storage_=this->shared_from_this());
        return Points_.insert(Points_.end(),NewValue);*/
    }

    PointStorage()=default;
    ///The number of points
    size_t size()const{return Points_.size();}
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
    ///Returns the i-th Point
    Point_t& operator[](size_t i){
        return Points_[i];
    }
    ///Returns the i-th point (const version)
    const Point_t& operator[](size_t i)const{
        return Points_[i];
    }
    ///Returns the weight of the i-th point
    Data_t& Weight(const Key_t& AKey,size_t i){
        return Weights_[AKey][i];
    }
    ///Returns the weight of the i-th point (const version)
    const Data_t& Weight(const Key_t& AKey,size_t i)const{
        return Weights_.at(AKey)[i];
    }

};



}
}//End namespaces

#endif /* BPMODULE_GUARD_POINT_HPP_ */

