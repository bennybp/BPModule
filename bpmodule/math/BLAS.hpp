/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   BLAS.hpp
 * Author: richard
 *
 * Created on April 13, 2016, 5:20 PM
 */

#ifndef BLAS_HPP
#define BLAS_HPP

#include <string>
#include <array>
extern "C" {
    void dsyev(char*,char*,int*,double*,int*, double*,double*,int*,int*);
}
namespace bpmodule{
namespace math{


/** \brief A C++-ified call to BLAS's symmetric matrix diagonalizer
 * 
 *  This function assumes that you are using some container like
 *  an std::vector for your flattened matrix.
 * 
 *  \param[in,out] Matrix The nxn matrix we are diagonalizing.  If EVecs==true
 *                        the rows of this matrix upon return will be the 
 *                        eigenvectors.  Must support the data() function
 *  \param[in,out] EVals  A PRE-ALLOCATED container of length n, must support
 *                        the size() function as well as the data() function
 *  \param[in] Stride     The number of elements in a row.  0 is the default
 *                        and a special value that means the stride is the
 *                        same as the length of the row
 *  \param[in] EVecs      True means you want the eigenvectors, False means you
 *                        don't
 *  \param[in] Upper      In theory you only have to store the upper or the
 *                        lower half of the matrix since it's symmetric.  By
 *                        default we assume you store the upper half, but if
 *                        you have the lower set this to false.  Note if you
 *                        have both this flag is irrelevant.
 */

template<typename Mat_t,typename EVal_t>
void SymmetricDiagonalize(Mat_t& Matrix,EVal_t& EVals,
         int Stride=0,bool EVecs=true,bool Upper=true){
    int n=EVals.size(),info,lwork=-1;
    if(Stride==0)Stride=n;
    double wkopt;
    //It's Fortran ultimately so upper is actually lower...stupid Fortran
    char v=EVecs?'V':'N',u=Upper?'L':'U';
    dsyev(&v,&u,&n,Matrix.data(),&Stride,EVals.data(),&wkopt,&lwork,&info);
    lwork=(int)wkopt;
    double* work=new double[lwork];
    dsyev(&v,&u,&n,&Matrix[0],&Stride,&EVals[0],work,&lwork,&info);
    delete [] work;
}

///Returns the cross product of two vectors
///\todo write in terms of wedge product
template<typename T1,typename T2>
std::array<double,3> Cross(const T1& LHS,const T2& RHS){
    return{LHS[1]*RHS[2]-LHS[2]*RHS[1],
           LHS[2]*RHS[0]-LHS[0]*RHS[2],
           LHS[0]*RHS[1]-LHS[1]*RHS[0]
    };
}
        

///Returns the dot product of two vectors
template<typename T1,typename T2>
double Dot(const T1& LHS,const T2& RHS){
    double sum=0.0;
    for(size_t i=0;i<LHS.size();++i)sum+=LHS[i]*RHS[i];
    return sum;
}

}}//End namespaces
#endif /* BLAS_HPP */

