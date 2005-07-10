// =================================================================== //
// Time-stamp: <01 Jul 03 16:17:27 Jean-Guillaume.Dumas@imag.fr> 
// =================================================================== //
#ifndef __Agora_Init__
#define __Agora_Init__

#include <vector>
#include "agora.h"

 
template<class Ints> 
Agora<Ints>::Agora(int n) : 
        _row(n), _size(n*n), 
        TopPM( 0), _PM( 0 ),
        _bits(8*sizeof(Pile_t)), 
        _bsize( 1<<_bits ), 
        _hauteurs( _bsize ), 
        _uns(_bsize), 
        _pris(_bsize), 
        _altitude( _size ),
        _eval( _bsize ),
        _enleve( _bsize),
        _ajoute_noir( _bsize),
        _ajoute_blanc( _bsize),
        _ajoute_prison( _bsize ),
        _voisins ( _size ) { 

// for random_shuffle
#ifdef _GLIBCPP_HAVE_DRAND48
    srand48(BaseTimer::seed());
#else
    srand(BaseTimer::seed());
#endif


    this->resize(_size); 
    
    Case_t j = 0;
    typename Plateau_t::iterator e = _enleve.begin(),
        rn = _ajoute_noir.begin(),
        rb = _ajoute_blanc.begin(),
        ap = _ajoute_prison.begin();

    typename Table_Eval_t::iterator ev = _eval.begin();

    for( typename Table_t::iterator 
             i = _hauteurs.begin(), 
             u = _uns.begin(), 
             p = _pris.begin();
         i != _hauteurs.end(); 
         ++i, ++u, ++p, ++ev, ++j, ++e, ++rn, ++rb, ++ap) {
        calcule_hauteur(*i, *u, *p, j );
        calcule_enleve(*e, *i, *u, j);
        calcule_ajoute(*rn, *rb, *i, *u, j);
        calcule_prison(*ap, *i, *u, j);
        calcule_eval(*ev, *i, *u, j);
    }
    
    
    for( typename Table_t::iterator a = _altitude.begin();
         a != _altitude.end();
         ++a)
        *a = 0;
    for(Case_t i = 0; i < _row; ++i) {
        _altitude[i] = 5;
        _altitude[_size-i-1] = 5;
        _altitude[i*_row] = 5;
        _altitude[(i+1)*_row-1] = 5;
    }
    for(Case_t i = 1; i < (_row-1); ++i) {
        _altitude[i+_row] = 2;
        _altitude[_size-i-1-_row] = 2;
        _altitude[i*_row+1] = 2;
        _altitude[(i+1)*_row-2] = 2;
    }
    
    


//     Case_t i = 0;
//     for( typename Voisin_t::iterator v = _voisins.begin();
//          v != _voisins.end();
//          ++v, ++i ) {
//         (*v).resize(0);
//         Case_t k = i + _row;	    // rangee du bas
//         long j = i - _row ;	    // rangee du haut

//         if ( (i % _row) < (_row>>1) ) { // Gauche reste à Gauche   
//             voisin_g( i, *v);
//             if ( i < (_size>>1) ) {     // Haut va en bas
//                 if (k < _size) voisin_g( k, *v);
//                 if ( j >= 0) voisin_g( j, *v);
//                 if (k < _size) voisin( k, *v);
//                 if ( j >= 0) voisin( j, *v);
//                 if (k < _size) voisin_d( k, *v);
//                 if ( j >= 0) voisin_d( j, *v);
//             } else {		    // Bas va en haut
//                 if ( j >= 0) voisin_g( j, *v);
//                 if (k < _size) voisin_g( k, *v);
//                 if ( j >= 0) voisin( j, *v);
//                 if (k < _size) voisin( k, *v);
//                 if ( j >= 0) voisin_d( j, *v);
//                 if (k < _size) voisin_d( k, *v);
//             }
//             voisin_d( i, *v);
//         } else {			    // Droite reste à Droite 
//             voisin_d( i, *v);
//             if ( i < (_size>>1) ) {     // Haut va en bas
//                 if (k < _size) voisin_d( k, *v);
//                 if ( j >= 0) voisin_d( j, *v);
//                 if (k < _size) voisin( k, *v);
//                 if ( j >= 0) voisin( j, *v);
//                 if (k < _size) voisin_g( k, *v);
//                 if ( j >= 0) voisin_g( j, *v);
//             } else {		    // Bas va en haut
//                 if ( j >= 0) voisin_d( j, *v);
//                 if (k < _size) voisin_d( k, *v);
//                 if ( j >= 0) voisin( j, *v);
//                 if (k < _size) voisin( k, *v);
//                 if ( j >= 0) voisin_g( j, *v);
//                 if (k < _size) voisin_g( k, *v);
//             }
//             voisin_g( i, *v);
//         }
//     }
   
    typename Voisin_t::iterator v = _voisins.begin();
    v->resize(0);
    v->push_back( 6 );
    v->push_back( 7 );
    v->push_back( 1 );

    (++v)->resize(0);
    v->push_back( 6 );
    v->push_back( 0 );
    v->push_back( 7 );
    v->push_back( 2 );
    v->push_back( 8 );

    (++v)->resize(0);
    v->push_back( 1 );
    v->push_back( 7 );
    v->push_back( 8 );
    v->push_back( 3 );
    v->push_back( 9 );
    
    (++v)->resize(0);
    v->push_back( 4 );
    v->push_back( 10 );
    v->push_back( 9 );
    v->push_back( 2 );
    v->push_back( 8 );

    (++v)->resize(0);
    v->push_back( 11 );
    v->push_back( 5 );
    v->push_back( 10 );
    v->push_back( 9 );
    v->push_back( 3 );
    
    (++v)->resize(0);
    v->push_back( 11 );
    v->push_back( 10 );
    v->push_back( 4 );

// Row 2
    (++v)->resize(0);
    v->push_back( 12 );
    v->push_back( 0 );
    v->push_back( 13 );
    v->push_back( 1 );
    v->push_back( 7 );

 (++v)->resize(0);
v->push_back( 12 );
v->push_back( 6 );
v->push_back( 13 );
v->push_back( 0 );
v->push_back( 1 );
v->push_back( 14 );
v->push_back( 2 );
v->push_back( 8 );
   
(++v)->resize(0);
v->push_back( 13 );
v->push_back( 7 );
v->push_back( 1 );
v->push_back( 14 );
v->push_back( 2 );
v->push_back( 15 );
v->push_back( 3 );
v->push_back( 9 );

(++v)->resize(0);
v->push_back( 10 );
v->push_back( 16 );
v->push_back( 4 );
v->push_back( 15 );
v->push_back( 3 );
v->push_back( 14 );
v->push_back( 2 );
v->push_back( 8 );

(++v)->resize(0);
v->push_back( 17 );
v->push_back( 11 );
v->push_back( 5 );
v->push_back( 16 );
v->push_back( 15 );
v->push_back( 9 );
v->push_back( 4 );
v->push_back( 3 );

(++v)->resize(0);
v->push_back( 17 );
v->push_back( 16 );
v->push_back( 10 );
v->push_back( 5 );
v->push_back( 4 );

// Row 3
 (++v)->resize(0);
v->push_back( 18 );
v->push_back( 19 );
v->push_back( 13 );
v->push_back( 6 );
v->push_back( 7 );

(++v)->resize(0);
v->push_back( 18 );
v->push_back( 12 );
v->push_back( 19 );
v->push_back( 6 );
v->push_back( 7 );
v->push_back( 20 );
v->push_back( 8 );
v->push_back( 14 );

(++v)->resize(0);
v->push_back( 19 );
v->push_back( 13 );
v->push_back( 7 );
v->push_back( 20 );
v->push_back( 8 );
v->push_back( 21 );
v->push_back( 9 );
v->push_back( 15 );

(++v)->resize(0);
v->push_back( 22 );
v->push_back( 16 );
v->push_back( 10 );
v->push_back( 21 );
v->push_back( 20 );
v->push_back( 9 );
v->push_back( 8 );
v->push_back( 14 );

(++v)->resize(0);
v->push_back( 22 );
v->push_back( 23 );
v->push_back( 17 );
v->push_back( 11 );
v->push_back( 10 );
v->push_back( 21 );
v->push_back( 9 );
v->push_back( 15 );

(++v)->resize(0);
v->push_back( 23 );
v->push_back( 11 );
v->push_back( 22 );
v->push_back( 16 );
v->push_back( 10 );


// Row 4

(++v)->resize(0);
v->push_back( 12 );
v->push_back( 24 );
v->push_back( 13 );
v->push_back( 25 );
v->push_back( 19 );

(++v)->resize(0);
v->push_back( 18 );
v->push_back( 12 );
v->push_back( 24 );
v->push_back( 13 );
v->push_back( 25 );
v->push_back( 14 );
v->push_back( 26 );
v->push_back( 20 );

(++v)->resize(0);
v->push_back( 19 );
v->push_back( 13 );
v->push_back( 25 );
v->push_back( 14 );
v->push_back( 26 );
v->push_back( 15 );
v->push_back( 27 );
v->push_back( 21 );

(++v)->resize(0);
v->push_back( 22 );
v->push_back( 16 );
v->push_back( 28 );
v->push_back( 15 );
v->push_back( 27 );
v->push_back( 14 );
v->push_back( 26 );
v->push_back( 20 );


(++v)->resize(0);
v->push_back( 23 );
v->push_back( 17 );
v->push_back( 29 );
v->push_back( 16 );
v->push_back( 28 );
v->push_back( 15 );
v->push_back( 27 );
v->push_back( 21 );

(++v)->resize(0);
v->push_back( 17 );
v->push_back( 29 );
v->push_back( 16 );
v->push_back( 28 );
v->push_back( 22 );

// row 5
(++v)->resize(0);
v->push_back( 18 );
v->push_back( 30 );
v->push_back( 19 );
v->push_back( 31 );
v->push_back( 25 );

(++v)->resize(0);
v->push_back( 24 );
v->push_back( 18 );
v->push_back( 30 );
v->push_back( 19 );
v->push_back( 31 );
v->push_back( 20 );
v->push_back( 32 );
v->push_back( 26 );

(++v)->resize(0);
v->push_back( 25 );
v->push_back( 19 );
v->push_back( 31 );
v->push_back( 20 );
v->push_back( 32 );
v->push_back( 21 );
v->push_back( 33 );
v->push_back( 27 );

(++v)->resize(0);
v->push_back( 28 );
v->push_back( 22 );
v->push_back( 34 );
v->push_back( 21 );
v->push_back( 33 );
v->push_back( 20 );
v->push_back( 32 );
v->push_back( 26 );

(++v)->resize(0);
v->push_back( 29 );
v->push_back( 23 );
v->push_back( 35 );
v->push_back( 22 );
v->push_back( 34 );
v->push_back( 21 );
v->push_back( 33 );
v->push_back( 27 );

(++v)->resize(0);
v->push_back( 23 );
v->push_back( 35 );
v->push_back( 22 );
v->push_back( 34 );
v->push_back( 28 );

// Row 6

(++v)->resize(0);
v->push_back( 24 );
v->push_back( 25 );
v->push_back( 31 );

(++v)->resize(0);
v->push_back( 30 );
v->push_back( 24 );
v->push_back( 25 );
v->push_back( 26 );
v->push_back( 32 );

(++v)->resize(0);
v->push_back( 31 );
v->push_back( 25 );
v->push_back( 26 );
v->push_back( 27 );
v->push_back( 33 );

(++v)->resize(0);
v->push_back( 34 );
v->push_back( 28 );
v->push_back( 27 );
v->push_back( 26 );
v->push_back( 32 );

(++v)->resize(0);
v->push_back( 35 );
v->push_back( 29 );
v->push_back( 28 );
v->push_back( 27 );
v->push_back( 33 );

(++v)->resize(0);
v->push_back( 29 );
v->push_back( 28 );
v->push_back( 34 );

}



template<class Ints>
inline void Agora<Ints>::voisin_g(const Case_t j, typename Voisin_t::value_type & loc)  const {
    if ( Case_t((j-1) % _row) < (j %_row) ) loc.push_back( j-1 );
}

template<class Ints>
inline void Agora<Ints>::voisin_d(const Case_t j, typename Voisin_t::value_type & loc)  const {
    if ( Case_t((j+1) % _row) > (j % _row) ) loc.push_back( j+1 );
}

template<class Ints>
inline void Agora<Ints>::voisin(const Case_t j, typename Voisin_t::value_type & loc)  const {
    loc.push_back( j );
}
    
   
 

template<class Ints>
inline typename Agora<Ints>::Pile_t& Agora<Ints>::calcule_enleve(Pile_t & r, const Hauteur_t t, const Hauteur_t u, const Pile_t p) const {
    if (t <= 1) return r = 0;
    r = p ^ (1<<(t-1));	 // retire
    if ( (u-1)<(t-u) ) {
        Hauteur_t rt, ru, pri;
        Pile_t rp = r;
        calcule_hauteur(rt, ru, pri, rp);
        if (rt <= 0) return calcule_revol(r,1,rp);
        return calcule_revol(r,rt,rp);
    } else 
        return r;
}


template<class Ints>
inline typename Agora<Ints>::Pile_t& Agora<Ints>::calcule_prison(Pile_t& a, const Hauteur_t t, const Hauteur_t u, const Pile_t p) const {
    a = p << 1;
    if (couleur(p)) a ^= SHORTCOULEURMASK;
    if ((t-u) >= u) {
        for (Hauteur_t i = 0; i <= (t-u); ++i)
            a |= (1<<i);
        a ^= SHORTCOULEURMASK;
    }
    return a;
}   




    
template<class Ints>
inline typename Agora<Ints>::Pile_t& Agora<Ints>::calcule_ajoute(Pile_t & r_noir, Pile_t& r_blanc, const Hauteur_t t, const Hauteur_t u, const Pile_t p) const {
    if (t > 0) {
        Pile_t r_meme = p | (1<<t);
        Pile_t r_autre = r_meme ^ SHORTCOULEURMASK; // change la couleur
        for (Hauteur_t i = 0; i < (t-u); ++i)
            r_autre |= (1<<i);
        if ( couleur(p) ) {
            r_noir = r_meme;
            r_blanc = r_autre;
        } else {
            r_noir = r_autre;
            r_blanc = r_meme;
        }
    } else {
        r_noir = SHORTCOULEURMASK | 1;
        r_blanc = 1;
    }
    return r_noir;
}        
    
template<class Ints>
inline Hauteur_t& Agora<Ints>::calcule_hauteur(Hauteur_t& h, Hauteur_t& u, Hauteur_t& pris, const Pile_t p) const {
    u=0;
    h=0;
    Pile_t pp = p & ((1<<(_bits-1))-1);
    for( ; pp != 0; pp >>=1, ++h) { 
        if (pp&01) ++u;
    }
    pris = h - u;
    return h;
}
    
template<class Ints>
inline typename Agora<Ints>::Pile_t& Agora<Ints>::calcule_revol(Pile_t& r, const Hauteur_t t, const Pile_t p) const {
    r=0;
    Pile_t y = ~0;  // 1111...1111
    y>>=(_bits-t);
    r = p | y;
    r = (Pile_t)r^(Pile_t)(1<<(_bits-1));
    return r;
}


template<class Ints>
inline typename Agora<Ints>::Pile_t Agora<Ints>::init( const Case_t i, const Pile_t p ) {
    return this->operator[]( i ) = p;
}    
       
template<class Ints>
inline void Agora<Ints>::reinit () {
        // WARNING : This is only valid for 6x6 
    
    for(typename Agora<Ints>::iterator i = this->begin(); i != this->end(); ++i)
        *i = Pile_t(0);

    for (int i = 0; i<=6; ++i) this->operator[](i) = ajoute_blanc(0);
    this->operator[](11) = ajoute_blanc(0);
    this->operator[](24) = ajoute_noir(0);
    for (int i = 29; i<=35; ++i) this->operator[](i) = ajoute_noir(0);
}


#define FACTEURPIONSPRISONNIERS 3

template<class Ints>
inline Eval_t& Agora<Ints>::calcule_eval(Eval_t& ev, const Hauteur_t t, const Hauteur_t u, const Pile_t p) const {
    if (t > 0) {
        if (couleur(p)) {
            ev = (t-2*u)*FACTEURPIONSPRISONNIERS;
	    return ev += (t-u); // prime aux prisonniers
        } else {
	    ev = (2*u-t)*FACTEURPIONSPRISONNIERS;
	    return ev -= (t-u); // prime aux prisonniers
        }
    } else
        return ev = 0;
}   

#endif
