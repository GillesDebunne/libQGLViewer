// =================================================================== //
// Time-stamp: <24 Jun 03 14:51:09 Jean-Guillaume.Dumas@imag.fr>
// =================================================================== //
#ifndef __Agora_IO_H__
#define __Agora_IO_H__

#include "agora_class.h"

template<class Ints>
std::ostream& operator<< (std::ostream& out, const Agora<Ints>& p) ;

template<class Ints>
std::ostream& details (std::ostream& out, const Agora<Ints>& p) ;

template<class Ints>
std::ostream& affiche (std::ostream& out, const Agora<Ints>& p) ;

template<class Ints>
std::istream& operator>> (std::istream& in, Agora<Ints>& p) ;

#endif
