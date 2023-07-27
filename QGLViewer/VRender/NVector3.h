#ifndef _VRENDER_NVECTOR3_H
#define _VRENDER_NVECTOR3_H

#include <iostream>
#include <stdexcept>

#include "Vector3.h"

namespace vrender
{
  class NVector3: public Vector3
  {
  public:
    NVector3();
    NVector3(const NVector3& u);
    inline NVector3(double x,double y,double z,bool normalization=true)
    {
      setXYZ(x,y,z,normalization);
    }

    NVector3(const Vector3 &u,bool normalization=true);

    inline double norm() const {return 1.0;}
    inline double squareNorm() const {return 1.0;}

    inline double x() const {return _n[0];}
    inline double y() const {return _n[1];}
    inline double z() const {return _n[2];}
    void setXYZ(double x,double y,double z,bool normalization=true);

    // Note: all operators must be implemented in the header.

    NVector3& operator=(const NVector3& u)
    {
      if ( &u != this )
      {
        _n[0] = u[0];
        _n[1] = u[1];
        _n[2] = u[2];
      }
      return *this;
    };

    friend std::ostream& operator<<(std::ostream &out,const NVector3 &u)
    {
      out << u[0] << " " << u[1] << " " << u[2];
      return out;
    };

    double operator[](int i) const
    {
      if((i < 0)||(i > 2)) throw std::runtime_error("Out of bounds in NVector3::operator[]") ;

      return _n[i];
    }

    inline friend Vector3 operator+(const NVector3 &u,const Vector3  &v)
    {
      return Vector3(u[0]+v[0],u[1]+v[1],u[2]+v[2]);
    };

    inline friend Vector3 operator+(const Vector3  &u,const NVector3 &v)
    {
      return Vector3(u[0]+v[0],u[1]+v[1],u[2]+v[2]);
    };

    inline friend Vector3 operator+(const NVector3 &u,const NVector3 &v)
    {
      return Vector3(u[0]+v[0],u[1]+v[1],u[2]+v[2]);
    };

    inline friend Vector3 operator-(const NVector3 &u,const Vector3  &v)
    {
      return Vector3(u[0]-v[0],u[1]-v[1],u[2]-v[2]);
    };

    inline friend Vector3 operator-(const Vector3  &u,const NVector3 &v)
    {
      return Vector3(u[0]-v[0],u[1]-v[1],u[2]-v[2]);
    };

    inline friend Vector3 operator-(const NVector3 &u,const NVector3 &v)
    {
      return Vector3(u[0]-v[0],u[1]-v[1],u[2]-v[2]);
    };

    inline friend double operator*(const NVector3 &u,const Vector3  &v)
    {
      return u[0]*v[0] + u[1]*v[1] + u[2]*v[2];
    };

    inline friend double operator*(const Vector3  &u,const NVector3 &v)
    {
      return u[0]*v[0] + u[1]*v[1] + u[2]*v[2];
    };

    inline friend double operator*(const NVector3 &u,const NVector3 &v)
    {
      return u[0]*v[0] + u[1]*v[1] + u[2]*v[2];
    };

    inline friend Vector3 operator*(double r,const NVector3 &u)
    {
      return Vector3(r*u[0],r*u[1],r*u[2]);
    };

    inline friend Vector3 operator/(const NVector3 &u,double r)
    {
      return Vector3(u[0]/r,u[1]/r,u[2]/r);
    };

    inline friend Vector3 operator^(const NVector3 &u,const Vector3  &v)
    {
      return Vector3( u[1]*v[2]-u[2]*v[1],
          u[2]*v[0]-u[0]*v[2],
          u[0]*v[1]-u[1]*v[0]);
    };

    inline friend Vector3 operator^(const Vector3  &u,const NVector3 &v)
    {
      return Vector3( u[1]*v[2]-u[2]*v[1],
          u[2]*v[0]-u[0]*v[2],
          u[0]*v[1]-u[1]*v[0]);
    };

    inline friend Vector3 operator^(const NVector3 &u,const NVector3 &v)
    {
      return Vector3( u[1]*v[2]-u[2]*v[1],
          u[2]*v[0]-u[0]*v[2],
          u[0]*v[1]-u[1]*v[0]);
    };

  private:
    void normalize();

    double _n[3];  //!< normalized vector

  }; // interface of NVector3

}

#endif // _NVECTOR3_H
