#ifndef _TIMER_H_
#define _TIMER_H_
// ==========================================================================
// $Source: /home/artis/debunne/CVS/QGLViewer/examples/contribs/agora/givtimer.h,v $
// Copyright(c)'94-97 by Givaro Team
// see the copyright file.
// Authors: T. Gautier
// $Id$
// ==========================================================================
// Description:

#include <iostream>

// class RealTimer; class SysTimer; class UserTimer;

class BaseTimer {
public:
enum {
  MSPSEC = 1000000  // microsecond per second
};

   // -- Clear timer :
  inline void clear() { _t = 0; }

  // -- total amount of second spent
  inline double time() const { return _t; }

  // -- Return a value to initialize random generator
static long seed();

  // -- basic methods:
  std::ostream& print( std::ostream& ) const;

  // -- Some arithmetic operators to compute cumulative time :
  BaseTimer& operator = (const BaseTimer & T) ;
  const BaseTimer operator - (const BaseTimer & T)  const;
  const BaseTimer operator - () ;
  const BaseTimer operator +  (const BaseTimer & T)  const;
  const BaseTimer operator += (const BaseTimer & T) { return *this = *this + T; };
  const BaseTimer operator -= (const BaseTimer & T) { return *this = *this - T; };

public:
   double _t;  // time
};
inline std::ostream& operator<< (std::ostream& o, const BaseTimer& BT)
{ return BT.print(o);}


class RealTimer : public BaseTimer {
public:
  inline RealTimer( const BaseTimer& BT ): BaseTimer(BT) {};
  inline RealTimer( ){};
  void start();
  void stop();
};


class UserTimer : public BaseTimer {
public:
  inline UserTimer( const BaseTimer& BT ): BaseTimer(BT) {};
  inline UserTimer( ) {};
  void start();
  void stop();
};


class SysTimer : public BaseTimer {
public:
  inline SysTimer( const BaseTimer& BT ): BaseTimer(BT) {};
  inline SysTimer( ) {};
  void start();
  void stop();
};


class Timer {
public :

   // Clear timer :
  void clear();

   // Start timer
  void start();

  // Stop timer
  void stop();

  // total amount of second spent in user mode
  double usertime() const { return ut.time(); }

  // total amount of second spent in system mode
  double systime () const { return st.time(); }

  // real total amount of second spent.
  double realtime () const { return rt.time(); }

  // retourne une petite graine
  // long seed() const { return RealTimer::seed(); }

  // Some arithmetic operators to compute cumulative time :
  Timer& operator = (const Timer & T) ;
  const Timer operator - (const Timer & T)  const;
  const Timer operator - () ;
  const Timer operator + (const Timer & T)  const;
  const Timer operator += (const Timer & T) { return *this = *this + T; };
  const Timer operator -= (const Timer & T) { return *this = *this - T; };


  // -- methods :
  std::ostream& print( std::ostream& ) const;

public:
 RealTimer rt;
 UserTimer ut;
 SysTimer  st;
};
// inline std::ostream& operator<<( std::ostream& o, const Timer& T)
// { return T.print(o);}

inline std::ostream& operator<<( std::ostream& o, const Timer& T)
{ return o << T.realtime() << "s (" << T.usertime() << " cpu)"; }


#endif
