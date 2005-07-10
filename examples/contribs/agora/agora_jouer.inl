// =================================================================== //
// Time-stamp: <01 Jul 03 16:18:09 Jean-Guillaume.Dumas@imag.fr> 
// =================================================================== //
#ifndef __Agora_Jouer__
#define __Agora_Jouer__
#include "agora.h"
#include "agora_types.h"


template<class Ints>
inline bool Agora<Ints>::couleur(const Pile_t p) const {
    return p & SHORTCOULEURMASK;
}
    
template<class Ints>
inline Hauteur_t Agora<Ints>::taille( const Pile_t i ) const {
    return _hauteurs[i];
} 

template<class Ints>
inline Hauteur_t Agora<Ints>::uns( const Pile_t i ) const {
    return _uns[i];
} 

template<class Ints>
inline Hauteur_t Agora<Ints>::pris( const Pile_t i ) const {
    return _pris[i];
} 

template<class Ints>
inline typename Agora<Ints>::Pile_t Agora<Ints>::enleve( const Pile_t p ) const {
    return _enleve[p];
}
    
template<class Ints>
inline typename Agora<Ints>::Pile_t Agora<Ints>::ajoute_noir( const Pile_t p ) const {
    return _ajoute_noir[p];
}
    
template<class Ints>
inline typename Agora<Ints>::Pile_t Agora<Ints>::ajoute_blanc( const Pile_t p ) const {
    return _ajoute_blanc[p];
}
    
template<class Ints>
inline typename Agora<Ints>::Pile_t Agora<Ints>::ajoute_prison( const Pile_t p) const {
    return _ajoute_prison[p];
}

template<class Ints>
inline void Agora<Ints>::jouer_dessus( const Case_t depart, Pile_t& ancien_d, Pile_t& new_d, const Case_t arrivee, Pile_t& ancien_a, Pile_t& new_a ) {
        // Pré requis : taille[dep]>=taille[arr]
    if (couleur( ancien_d = this->operator[](depart) ))
        new_a = this->operator[](arrivee) = ajoute_noir( ancien_a = this->operator[](arrivee));
    else
        new_a = this->operator[](arrivee) = ajoute_blanc( ancien_a = this->operator[](arrivee));
    new_d = this->operator[](depart) = enleve( ancien_d );
}
    
            
template<class Ints>
inline void Agora<Ints>::jouer_dessous( const Case_t depart, Pile_t& ancien_d, Pile_t& new_d, const Case_t arrivee, Pile_t& ancien_a, Pile_t& new_a ) {
    new_a = this->operator[](arrivee) = ajoute_prison( ancien_a = this->operator[](arrivee) );
    new_d = this->operator[](depart) = enleve( ancien_d = this->operator[](depart) );
}

template<class Ints>
inline void Agora<Ints>::jouer( const Case_t depart, Pile_t& ancien_d, Pile_t& new_d, const Case_t arrivee, Pile_t& ancien_a, Pile_t& new_a, const bool dessus ) {
    if (dessus) 
        jouer_dessus( depart, ancien_d, new_d, arrivee, ancien_a, new_a);
    else
        jouer_dessous( depart, ancien_d, new_d, arrivee, ancien_a, new_a);
}

       
template<class Ints>
inline bool Agora<Ints>::jouer_dessus( const Case_t depart, Pile_t& ancien_d, const Case_t arrivee, Pile_t& ancien_a ) {
        // Pré requis : taille[dep]>=taille[arr]
    if (couleur( ancien_d = this->operator[](depart) )) {
        Pile_t new_a = this->operator[](arrivee) = ajoute_noir( ancien_a = this->operator[](arrivee));
        this->operator[](depart) = enleve( ancien_d );
        return couleur( new_a );
    } else {
        Pile_t new_a = this->operator[](arrivee) = ajoute_blanc( ancien_a = this->operator[](arrivee));
        this->operator[](depart) = enleve( ancien_d );    
        return couleur( new_a );
    }
}
    
            
template<class Ints>
inline bool Agora<Ints>::jouer_dessous( const Case_t depart, Pile_t& ancien_d, const Case_t arrivee, Pile_t& ancien_a ) {
    Pile_t new_a = this->operator[](arrivee) = ajoute_prison( ancien_a = this->operator[](arrivee) );
    this->operator[](depart) = enleve( ancien_d = this->operator[](depart) );
    return couleur(new_a);
}

template<class Ints>
inline bool Agora<Ints>::jouer( const Case_t depart, Pile_t& ancien_d, const Case_t arrivee, Pile_t& ancien_a, const bool dessus ) {
    if (dessus) 
        return jouer_dessus( depart, ancien_d, arrivee, ancien_a);
    else
        return jouer_dessous( depart, ancien_d, arrivee, ancien_a);
}

template<class Ints>
inline bool Agora<Ints>::jouer( const Case_t depart, const Case_t arrivee, const bool dessus ) {
    Pile_t ad, aa;
    return jouer(depart, ad, arrivee, aa, dessus);
}

template<class Ints>
inline bool Agora<Ints>::jouer( const Coup& c ) {
    Pile_t ad, aa;
//     if (couleur( this->operator[]( c.depart() ) ) ) std::cerr << c << std::endl;
//     if ( ArriveeRevolution( c ) ) std::cerr << "Revolution à l'arrivée" << std::endl;
//     if ( DepartRevolution( c ) ) std::cerr << "Revolution au départ" << std::endl;
    return jouer(c.depart(), ad, c.arrivee(), aa, c.dessus());
}


template<class Ints>
inline void Agora<Ints>::remettre( const Case_t depart, const Pile_t ancien_d, const Case_t arrivee, const Pile_t ancien_a ) {
    this->operator[](depart) = ancien_d;
    this->operator[](arrivee) = ancien_a;
}



	// Calculée
template<class Ints>
inline bool Agora<Ints>::ArriveeRevolution( const Coup& c) const {
        // A appeler avant de jouer
    Pile_t ancien_a = this->operator[]( c.arrivee() );
    Hauteur_t t = taille( ancien_a ), u = uns( ancien_a );
    if ( (! c.dessus()) && ( (t-u)>=(u) ) )
        return true;
    else
        return false;
}

template<class Ints>
inline bool Agora<Ints>::DepartRevolution( const Coup& c) const {
        // A appeler avant de jouer
    Pile_t ancien_d = this->operator[]( c.depart() );
    Hauteur_t t = taille( ancien_d ), u = uns( ancien_d );
    if ( (t>=3) && ( (u-1)<(t-u) ) ) 
        return true;
    else
        return false;
}


#endif
