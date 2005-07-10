// =================================================================== //
// Time-stamp: <20 Jun 03 18:42:27 Jean-Guillaume.Dumas@imag.fr>
// =================================================================== //
#ifndef __CONTAINER_STREAM__
#define __CONTAINER_STREAM__

#include <iostream>
template<class T, template <class T> class Container >
std::ostream& operator<< (std::ostream& o, const Container<T>& v) {
    o << "[";
    typename Container<T>::const_iterator vi = v.begin();
    for( ; vi != v.end(); ++vi)
        o << *vi << " ";
    return o << "]";
}

template<int I, class T, template <int I, class T> class Container >
std::ostream& operator<< (std::ostream& o, const Container<I,T>& v) {
    o << "[";
    typename Container<I,T>::const_iterator vi = v.begin();
    for( ; vi != v.end(); ++vi)
        o << *vi << " ";
    return o << "]";
}

#endif
