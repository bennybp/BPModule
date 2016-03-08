#include "FiniteDiff.hpp"
namespace bpmodule {
namespace math {

std::vector<double> Coefs(const std::vector<double>& Stencil, size_t Deriv) {
   size_t M=Deriv,M1=M+1,N=Stencil.size();
   std::vector<double> c(M1*N);
   c[0]=1.0;
   double c1=1.0,c4=Stencil[0];
   for (size_t i=1; i<N; i++) {
      size_t mn=(i<M ? i : M);
      double c2=1.0,c5=c4;
      c4=Stencil[i];
      for (size_t j=0; j<i; j++) {
         double c3=Stencil[i]-Stencil[j];
         c2*=c3;
         if (j==i-1) {
            for (size_t k=mn; k>0; k--)
               c[i*M1+k]=c1*((double)k*c[(i-1)*M1+k-1]-c5*c[(i-1)*M1+k])/c2;
            c[i*M1]=-c1*c5*c[(i-1)*M1]/c2;
         }
         for (size_t k=mn; k>0; k--)
            c[j*M1+k]=(c4*c[j*M1+k]-(double)k*c[j*M1+k-1])/c3;
         c[j*M1]=c4*c[j*M1]/c3;
      }
      c1=c2;
   }
   std::vector<double> Cs(N);
   for (size_t i=0; i<N; i++)
      Cs[i]=c[i*M1+M];
   return Cs;
}

}}//End namespaces

