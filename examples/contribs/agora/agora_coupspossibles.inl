// =================================================================== //
// Time-stamp: <01 Jul 03 16:41:35 Jean-Guillaume.Dumas@imag.fr> 
// =================================================================== //
#ifndef __Agora_CoupsPossibles__
#define __Agora_CoupsPossibles__
#include "agora.h"

template<class Ints>
inline bool Agora<Ints>::gameIsOver() {
    return (gameIsOver(true) || gameIsOver(false) );
}

template<class Ints>
inline bool Agora<Ints>::gameIsOver(bool color) {
    Possible_t Liste;
    CoupsPossibles( Liste, color );
    if (Liste.size())
        return false;
    else
        return true;
}
  
    



template<class Ints>
inline Hauteur_t Agora<Ints>::calcule_poss_blancs(Possible_t& Liste, const Case_t depart, const Hauteur_t hdepalt, const Case_t arrivee)  const {
    Pile_t arr = this->operator[](arrivee);
    bool carr = couleur(arr);
    Hauteur_t harr = taille(arr);
    if (harr) {
            // Il y a quelqu'un
        Hauteur_t charr = harr + _altitude[arrivee];            
        if ( (hdepalt >= charr) && calcule_poss_dessus_blanc(carr,harr) )
            Liste.push_back( Coup(depart, arrivee, 1) );
        if ( (hdepalt <= charr) && calcule_poss_dessous_blanc(carr,harr) )
            Liste.push_back( Coup(depart, arrivee, 0) );
    } else
            // Il n'y a personne
        Liste.push_back( Coup(depart, arrivee, 1) );
    return (carr?pris(arr):uns(arr));
}

template<class Ints>
inline Hauteur_t Agora<Ints>::calcule_poss_noirs(Possible_t& Liste, const Case_t depart, const Hauteur_t hdepalt, const Case_t arrivee)  const {
    Pile_t arr = this->operator[](arrivee);
    bool carr = couleur(arr);
    Hauteur_t harr = taille(arr);
    if (harr) {
            // Il y a quelqu'un
        Hauteur_t charr = harr + _altitude[arrivee];
            
        if ( (hdepalt >= charr) && calcule_poss_dessus_noir(carr,harr))
            Liste.push_back( Coup(depart, arrivee, 1) );
        if ( (hdepalt <= charr)  && calcule_poss_dessous_noir(carr,harr) )
            Liste.push_back( Coup(depart, arrivee, 0) );
    } else
            // Il n'y a personne
        Liste.push_back( Coup(depart, arrivee, 1) );
    return (carr?uns(arr):pris(arr));
}

    // Pour l'instant isolé interdit
template<class Ints>
inline bool Agora<Ints>::calcule_poss_dessus_noir(const bool c, const Hauteur_t t)  const { 
    return ( !c || !t );
}

template<class Ints>
inline bool Agora<Ints>::calcule_poss_dessous_noir(const bool c, const Hauteur_t)  const {
//     return ( !c && t );
    return ( !c );
}
    
template<class Ints>
inline bool Agora<Ints>::calcule_poss_dessus_blanc(const bool c, const Hauteur_t t)  const {
    return ( c || !t );
}

template<class Ints>
inline bool Agora<Ints>::calcule_poss_dessous_blanc(const bool c, const Hauteur_t)  const {
//     return ( c && t);
    return c;
}
        
template<class Ints>
inline typename Agora<Ints>::Possible_t& Agora<Ints>::CoupsVoisinsBlancs(Possible_t& Liste, const Case_t i)  const {
    Pile_t dep = this->operator[](i);
    bool cdep = couleur(dep);
#ifdef ADVANCEDRULES
    Hauteur_t voisinsblancs = 0;
#endif
    if (cdep == WHITE)
        if (Hauteur_t hdep = taille(dep) ) {
            Hauteur_t hdepalt = hdep + _altitude[i];
            for(Voisin_t::value_type::const_iterator n = _voisins[i].begin();
                n != _voisins[i].end(); ++n)
#ifdef ADVANCEDRULES
                voisinsblancs +=
#endif
                calcule_poss_blancs( Liste, i, hdepalt, *n);   

#ifdef ADVANCEDRULES
            if ((voisinsblancs == 1) && (uns(dep) == 1))
                for(Voisin_t::value_type::const_iterator n = _voisins[i].begin();
                    n != _voisins[i].end(); ++n)
                    if (! couleur(this->operator[](*n)) && taille(this->operator[](*n))) {
                        Liste.push_back( Coup( i, *n, 1) );
                        break;
                    }
#endif
        }
    return Liste;
}       

template<class Ints>
inline typename Agora<Ints>::Possible_t& Agora<Ints>::CoupsVoisinsNoirs(Possible_t& Liste, const Case_t i)  const {
    Pile_t dep = this->operator[](i);
    bool cdep = couleur(dep);
#ifdef ADVANCEDRULES
    Hauteur_t voisinsnoirs = 0;
#endif
    if (cdep == BLACK)
        if (Hauteur_t hdep = taille(dep) ) {
            Hauteur_t hdepalt = hdep + _altitude[i];
            for(Voisin_t::value_type::const_iterator n = _voisins[i].begin();
                n != _voisins[i].end(); ++n)
#ifdef ADVANCEDRULES
                voisinsnoirs +=
#endif
                calcule_poss_noirs( Liste, i, hdepalt, *n);   
#ifdef ADVANCEDRULES
            if ((voisinsnoirs == 1) && (uns(dep) == 1) )
                for(Voisin_t::value_type::const_iterator n = _voisins[i].begin();
                    n != _voisins[i].end(); ++n)
                    if (couleur(this->operator[](*n)) ) {
                        Liste.push_back( Coup( i, *n, 1) );
                        break;
                    }
#endif
        }
    return Liste;
}       

#include "container_stream.h"

template<class Ints>
inline typename Agora<Ints>::Possible_t& Agora<Ints>::CoupsVoisins(Possible_t& Liste, const Case_t i)  const {
    Liste.resize(0);
    if ( couleur( this->operator[](i) ) )
        return CoupsVoisinsNoirs( Liste, i);
    else
        return CoupsVoisinsBlancs( Liste, i);
//     if ( couleur( this->operator[](i) ) )
//         CoupsVoisinsNoirs( Liste, i);
//     else
//         CoupsVoisinsBlancs( Liste, i);
//     std::cerr << Liste << std::endl;
//     return Liste;
}       

template<class Ints>
inline typename Agora<Ints>::Possible_t& Agora<Ints>::CoupsPossibles(Possible_t& Liste, const bool c)  const {
    Liste.resize(0);
    if (c) {
    CoupsVoisinsNoirs(Liste, 26);
    CoupsVoisinsNoirs(Liste, 27);
    CoupsVoisinsNoirs(Liste, 9);
    CoupsVoisinsNoirs(Liste, 8);
    CoupsVoisinsNoirs(Liste, 13);
    CoupsVoisinsNoirs(Liste, 22);
    CoupsVoisinsNoirs(Liste, 19);
    CoupsVoisinsNoirs(Liste, 16);
    CoupsVoisinsNoirs(Liste, 20);
    CoupsVoisinsNoirs(Liste, 15);
    CoupsVoisinsNoirs(Liste, 21);
    CoupsVoisinsNoirs(Liste, 14);
    CoupsVoisinsNoirs(Liste, 28);
    CoupsVoisinsNoirs(Liste, 7);
    CoupsVoisinsNoirs(Liste, 10);
    CoupsVoisinsNoirs(Liste, 25);
    CoupsVoisinsNoirs(Liste, 32);
    CoupsVoisinsNoirs(Liste, 3);
    CoupsVoisinsNoirs(Liste, 33);
    CoupsVoisinsNoirs(Liste, 2);
    CoupsVoisinsNoirs(Liste, 12);
    CoupsVoisinsNoirs(Liste, 23);
    CoupsVoisinsNoirs(Liste, 18);
    CoupsVoisinsNoirs(Liste, 17);
    CoupsVoisinsNoirs(Liste, 6);
    CoupsVoisinsNoirs(Liste, 29);
    CoupsVoisinsNoirs(Liste, 24);
    CoupsVoisinsNoirs(Liste, 11);
    CoupsVoisinsNoirs(Liste, 34);
    CoupsVoisinsNoirs(Liste, 1);
    CoupsVoisinsNoirs(Liste, 31);
    CoupsVoisinsNoirs(Liste, 4);
    CoupsVoisinsNoirs(Liste, 5);
    CoupsVoisinsNoirs(Liste, 30);
    CoupsVoisinsNoirs(Liste, 35);
    CoupsVoisinsNoirs(Liste, 0);
    } else {
    CoupsVoisinsBlancs(Liste, 26);
    CoupsVoisinsBlancs(Liste, 27);
    CoupsVoisinsBlancs(Liste, 9);
    CoupsVoisinsBlancs(Liste, 8);
    CoupsVoisinsBlancs(Liste, 13);
    CoupsVoisinsBlancs(Liste, 22);
    CoupsVoisinsBlancs(Liste, 19);
    CoupsVoisinsBlancs(Liste, 16);
    CoupsVoisinsBlancs(Liste, 20);
    CoupsVoisinsBlancs(Liste, 15);
    CoupsVoisinsBlancs(Liste, 21);
    CoupsVoisinsBlancs(Liste, 14);
    CoupsVoisinsBlancs(Liste, 28);
    CoupsVoisinsBlancs(Liste, 7);
    CoupsVoisinsBlancs(Liste, 10);
    CoupsVoisinsBlancs(Liste, 25);
    CoupsVoisinsBlancs(Liste, 32);
    CoupsVoisinsBlancs(Liste, 3);
    CoupsVoisinsBlancs(Liste, 33);
    CoupsVoisinsBlancs(Liste, 2);
    CoupsVoisinsBlancs(Liste, 12);
    CoupsVoisinsBlancs(Liste, 23);
    CoupsVoisinsBlancs(Liste, 18);
    CoupsVoisinsBlancs(Liste, 17);
    CoupsVoisinsBlancs(Liste, 6);
    CoupsVoisinsBlancs(Liste, 29);
    CoupsVoisinsBlancs(Liste, 24);
    CoupsVoisinsBlancs(Liste, 11);
    CoupsVoisinsBlancs(Liste, 34);
    CoupsVoisinsBlancs(Liste, 1);
    CoupsVoisinsBlancs(Liste, 31);
    CoupsVoisinsBlancs(Liste, 4);
    CoupsVoisinsBlancs(Liste, 5);
    CoupsVoisinsBlancs(Liste, 30);
    CoupsVoisinsBlancs(Liste, 35);
    CoupsVoisinsBlancs(Liste, 0);        
        
    }
    
    
    return Liste;
}

#endif
