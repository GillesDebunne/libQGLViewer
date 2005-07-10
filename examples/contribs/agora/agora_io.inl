// =================================================================== //
// Time-stamp: <24 Jun 03 14:51:08 Jean-Guillaume.Dumas@imag.fr> 
// =================================================================== //
#ifndef __Agora_IO__ 
#define __Agora_IO__

#include <iostream>
#include <deque>
#include "agora.h"
 
template<class Ints>
inline std::ostream& operator<< (std::ostream& out, const Agora<Ints>& p) {
    Case_t r = 1;
    for(typename Agora<Ints>::Plateau_t::const_iterator i = p.begin(); i != p.end(); ++i, ++r) {
        out << Size_t(*i) << " ";
        if (r >= p._row) { r=0; out << std::endl; }
    }
    return out;
}

template<class Ints>
inline std::ostream& details (std::ostream& out, const Agora<Ints>& p) {
        out << "Plateau: " << std::endl;
        Case_t r = 1;
        for(typename Agora<Ints>::Plateau_t::const_iterator i = p.begin(); i != p.end(); ++i, ++r) {
            out << Size_t(*i) << " ";
            if (r >= p._row) { r=0; out << std::endl; }
        }
        out << "Cases: " << std::endl;
        r = 1;
        for(Case_t i = 0; i < p._size; ++i, ++r) {
            out << Size_t(i) << " ";
            if (r >= p._row) { r=0; out << std::endl; }
        }
        out << "Altitude: " << std::endl;
        r = 1;
        for(Table_t::const_iterator i = p._altitude.begin(); i != p._altitude.end(); ++i, ++r) {
            out << Size_t(*i) << " ";
            if (r >= p._row) { r=0; out << std::endl; }
        }
        out << "Voisins: " << std::endl;
        r = 1;
        for(typename Agora<Ints>::Voisin_t::const_iterator i = p._voisins.begin(); i != p._voisins.end(); ++i, ++r) {
            out << *i << " ";
            if (r >= p._row) { r=0; out << std::endl; }
        }
        return out;
}
    
template<class Ints>
inline std::ostream& affiche (std::ostream& out, const Agora<Ints>& p) {
        out << "---------------- ---------------- ---------------- ---------------- ---------------- ----------------\n";
        Case_t r = 1;
        std::deque<bool> d;

        for(typename Agora<Ints>::Plateau_t::const_iterator i = p.begin(); i != p.end(); ++i, ++r) {
            typename Agora<Ints>::Pile_t l = (*i);
            d.resize(0);
            for(int i = 8*sizeof(typename Agora<Ints>::Pile_t); i; --i,l>>=1) 
                d.push_front(l & 0x1);
            for(std::deque<bool>::const_iterator i = d.begin(); i != d.end(); ++i)
                out << Size_t(*i);
            out << " ";
            if (r >= p._row) { r=0; out << std::endl; }
        }
        return out << "---------------- ---------------- ---------------- ---------------- ---------------- ----------------\n";
    }
    
template<class Ints>
inline std::istream& operator>> (std::istream& in, Agora<Ints>& p) {
        for(typename Agora<Ints>::Plateau_t::iterator i = p.begin(); i != p.end(); ++i) 
            in >> (*i);
        return in;
}

#endif
