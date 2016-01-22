/* 
 * File:   Point.hpp
 * Original Author: Ryan Richard <ryanmrichard1@gmail.com>
 *
 * Created on January 22, 2016, 11:46 AM
 */

#ifndef BPMODULE_GUARD_POINT_HPP_
#define BPMODULE_GUARD_POINT_HPP_

#include <vector>
#include <map>
#include <memory>

namespace bpmodule{
namespace math{

template<typename Key,typename T>
class Point;

template<typename Key,typename T>
class PointStorage;

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

    Point<Key,T>&& operator*()const{
        return Point(Idx_,Parent_);
    }

    bool operator==(const PSItr& other)const{
        return Idx_==other.Idx_;
    }
};

/** \brief The class that holds the memory for a point's 
 *   coordinates and weights.
 * 
 * \param Key The type of the weight lookup key
 * \param T The type of the weights
 */
template<typename Key,typename T>
class PointStorage:public std::enable_shared_from_this<PointStorage<Key,T> >{
private:
    ///A 3 by NPoints array of coordinates
    std::vector<double> Carts_;

    ///A nkeys by NPoints array of weights
    std::map<Key,std::vector<T> > Weights_;

public:
    typedef PSItr iterator;
    typedef PSItr const_iterator;
    
    iterator begin(){
        return PSItr(0,std::shared_from_this());
    }
    const_iterator begin()const{
        return PSItr(0,std::shared_from_this());
    }
    iterator end(){
        return PSItr(size(),std::shared_from_this());
    }
    const_iterator end()const{
        return PSItr(size(),std::shared_from_this());
    }
    
    ///Inserts the new value at the end (ignores iterator), returns iterator
    ///to new point
    iterator insert(iterator,const Point<Key,T>& Value){
        Carts_.push_back(Value[0]);
        Carts_.push_back(Value[1]);
        Carts_.push_back(Value[2]);
    }

    PointStorage()=default;
    ///The number of points
    size_t size()const{return Carts_.size()/3;}
    ///The number of weights
    size_t NWeights()const{return Weights_.size();}

};

template<typename Key,typename T>
class Point{
private:
    ///The index of this point
    size_t Idx_;

    ///The actual data
    std::shared_ptr<PointStorage<Key,T> > Storage_;
    
    friend PSItr<Key,T>;
    Point(size_t Index,std::shared_ptr<PointStorage<Key,T> > Storage):
        Idx_(Index),Storage_(Storage){}
public:
    Point()=default;
    Point(double x,double y,double z):
        Idx_(0),Storage_(new PointStorage<Key,T>){
        Storage_->insert(Storage_->begin(),*this);
    }
    size_t NWeights()const{return Storage_->NWeights();}
    double operator[](size_t i)const{return (*Storage_)[Idx_][i];}
    double& operator[](size_t i){return (*Storage_)[Idx_][i];}


};

}
}//End namespaces

#endif /* BPMODULE_GUARD_POINT_HPP_ */

