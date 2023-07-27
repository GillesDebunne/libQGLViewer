#include "NVector3.h"

using namespace vrender;

NVector3::NVector3(const Vector3 &u,bool normalization)
{
  setXYZ(u[0],u[1],u[2],normalization);
}

// -----------------------------------------------------------------------------
//! Default constructor (the default normalized vector is (1,0,0))
NVector3::NVector3()
{
  _n[0] = 1.0;
  _n[1] = 0.0;
  _n[2] = 0.0;
}

// -----------------------------------------------------------------------------
//! Copy constructor
NVector3::NVector3(const NVector3& u)
{
  _n[0] = u._n[0] ;
  _n[1] = u._n[1] ;
  _n[2] = u._n[2] ;
}

// -----------------------------------------------------------------------------
//! Writing X,Y and Z coordinates
void NVector3::setXYZ(double x,double y,double z,bool normalization)
{
  _n[0] = x;
  _n[1] = y;
  _n[2] = z;
  if ( normalization ) normalize();
}

// -----------------------------------------------------------------------------
//! Normalization
//! Private method to do normalization (using Norm() method of the Vector class)
//! when it is necessary (construction of a normalized vector for exemple).
void NVector3::normalize()
{
  double n = _n[0]*_n[0]+_n[1]*_n[1]+_n[2]*_n[2] ;

  if ( n > 0.0 )
  {
    _n[0] /= n;
    _n[1] /= n;
    _n[2] /= n;
  }
  else
    throw std::runtime_error("Attempt to normalize a null 3D vector.") ;
}

