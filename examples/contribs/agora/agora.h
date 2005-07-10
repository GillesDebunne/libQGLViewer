// =================================================================== //
// Time-stamp: <01 Jul 03 16:27:25 Jean-Guillaume.Dumas@imag.fr>
// =================================================================== //
#ifndef __Agora_Plateau__
#define __Agora_Plateau__

#include <iostream>
#include <vector>
#include "givtimer.h"

#define SHORTCOULEURMASK 32768

#define WHITE false
#define BLACK true

#define ADVANCEDRULES

#include "agora_class.h"
#include "agora_io.h"
#include "agora_types.h"
#include "agora_coup.h"
#include "agora_container.h"


template<class Ints>
class Agora : public std::vector<Ints> {
public:
    typedef Agora< Ints > Self_t;
    typedef Ints Pile_t;
    typedef std::vector<Pile_t> Plateau_t;
    typedef std::vector< std::vector< Case_t > >  Voisin_t;

    typedef AgoraCoup Coup;
    typedef AgoraContainer<256, Coup> Possible_t;
    typedef std::vector< Possible_t > Table_Possible_t;
    typedef std::vector<Eval_t> Table_Eval_t;

private:
    Case_t _row, _size;
    Possible_t TopPM;
    Table_Possible_t _PM;
    Hauteur_t _bits;
    Size_t _bsize;
    Table_t _hauteurs, _uns, _pris, _altitude;
    Table_Eval_t _eval;
    Plateau_t _enleve, _ajoute_noir, _ajoute_blanc, _ajoute_prison;
    Voisin_t _voisins;

public:

    Agora(int n = 6) ;
    void reinit() ;

        // Tabulées
    bool couleur(const Pile_t p) const ;
    Hauteur_t taille( const Pile_t i ) const;
    Hauteur_t uns( const Pile_t i ) const;
    Hauteur_t pris( const Pile_t i ) const;
    Eval_t eval( const Pile_t i ) const;

	// Calculée
    bool ArriveeRevolution( const Coup& c) const;
    bool DepartRevolution( const Coup& c) const;


        // Tabulées
    Pile_t enleve( const Pile_t p ) const;
    Pile_t ajoute_noir( const Pile_t p ) const ;
    Pile_t ajoute_blanc( const Pile_t p ) const ;
    Pile_t ajoute_prison( const Pile_t p) const ;

        // Initialisation
    Pile_t init( const Case_t i, const Pile_t p ) ;

        // Jouer
    bool jouer( const Coup& c ) ;
    bool jouer( const Case_t depart, const Case_t arrivee, const bool dessus );
    bool jouer( const Case_t depart, Pile_t& ancien_d, const Case_t arrivee, Pile_t& ancien_a, const bool dessus );
    bool jouer_dessus( const Case_t depart, Pile_t& ancien_d, const Case_t arrivee, Pile_t& ancien_a ) ;
    bool jouer_dessous( const Case_t depart, Pile_t& ancien_d, const Case_t arrivee, Pile_t& ancien_a );
    void jouer( const Case_t depart, Pile_t& ancien_d, Pile_t& new_d, const Case_t arrivee, Pile_t& ancien_a, Pile_t& new_a, const bool dessus );
    void jouer_dessus( const Case_t depart, Pile_t& ancien_d, Pile_t& new_d, const Case_t arrivee, Pile_t& ancien_a, Pile_t& new_a ) ;
    void jouer_dessous( const Case_t depart, Pile_t& ancien_d, Pile_t& new_d, const Case_t arrivee, Pile_t& ancien_a, Pile_t& new_a );
    void remettre( const Case_t depart, const Pile_t ancien_d, const Case_t arrivee, const Pile_t ancien_a ) ;


        // Fonction d'évaluation
    double& eval(double& e, const Pile_t p) const;
    double& uneval(double& e, const Pile_t p) const;
    double& eval(double& e, const Pile_t dep, const Pile_t ndep, const Pile_t arr, const Pile_t narr)  const ;
    double eval() const ;


        // Meilleur coup
        // Default : temps limité, mais si depth > 0 --> profondeur fixée
    Coup& Suggest(Coup& bp, bool ordi_color, double temps_max, int depth = 0);

	// Coups possibles
    bool gameIsOver() ;
    bool gameIsOver(bool color) ;

    Possible_t& CoupsVoisinsBlancs(Possible_t& Liste, const Case_t i) const ;
    Possible_t& CoupsVoisinsNoirs(Possible_t& Liste, const Case_t i) const ;
    Possible_t& CoupsVoisins(Possible_t& Liste, const Case_t i) const ;
    Possible_t& CoupsPossibles(Possible_t& Liste, const bool c) const ;

private:
#ifdef __ERRORS__
    unsigned long long _count;
#endif

        // Construction
    void voisin_g(const Case_t j, typename Voisin_t::value_type & loc) const ;
    void voisin_d(const Case_t j, typename Voisin_t::value_type & loc) const ;
    void voisin(const Case_t j, typename Voisin_t::value_type & loc) const  ;

        // Calculs initiaux pour tabulation
    Pile_t& calcule_enleve(Pile_t & r, const Hauteur_t t, const Hauteur_t u, const Pile_t p) const ;
    Pile_t& calcule_prison(Pile_t& a, const Hauteur_t t, const Hauteur_t u, const Pile_t p) const ;
    Pile_t& calcule_ajoute(Pile_t & r_noir, Pile_t& r_blanc, const Hauteur_t t, const Hauteur_t u, const Pile_t p) const ;
    Hauteur_t& calcule_hauteur(Hauteur_t& h, Hauteur_t& u, Hauteur_t& pris, const Pile_t p) const ;
    Pile_t& calcule_revol(Pile_t& r, const Hauteur_t t, const Pile_t p) const ;
    Eval_t& calcule_eval(Eval_t& ev, const Hauteur_t t, const Hauteur_t u, const Pile_t p) const ;

        // Optimisations Branch and Cut
    double ami_ab(int depth, double alpha, double beta, bool ordi_color);
    double ennemi_ab(int depth, double alpha, double beta, bool ordi_color) ;
    double ami_ab(int depth, double alpha, double beta, bool ordi_color, double currev, const Pile_t ad, const Pile_t nd, const Pile_t aa, const Pile_t na);
    double ennemi_ab(int depth, double alpha, double beta, bool ordi_color, double currev, const Pile_t ad, const Pile_t nd, const Pile_t aa, const Pile_t na) ;
        // Meilleur coup en temps limité
    Coup& Iteratif(Coup& bp, bool ordi_color, double temps_max, int StartProf) ;
        // Meilleur coup à profondeur fixée
    double BestPlay(Coup& bp, bool ordi_color, int depth);

    inline double MTDFdouble(int depth, double f, bool ordi_color) ;
    double OnebyOneBP(Coup& bp, bool ordi_color, int depth, double temps_max, Timer& global) ;

        // Possiblité de coup
        // Pour l'instant isolé interdit
    bool calcule_poss_dessus_noir(const bool c, const Hauteur_t t) const ;
    bool calcule_poss_dessous_noir(const bool c, const Hauteur_t t)  const ;
    bool calcule_poss_dessus_blanc(const bool c, const Hauteur_t t)  const ;
    bool calcule_poss_dessous_blanc(const bool c, const Hauteur_t t)  const ;
    Hauteur_t calcule_poss_blancs(Possible_t& Liste, const Case_t depart, const Hauteur_t hdepalt, const Case_t arrivee)  const ;
    Hauteur_t calcule_poss_noirs(Possible_t& Liste, const Case_t depart, const Hauteur_t hdepalt, const Case_t arrivee)  const ;

	// Entrées/Sorties
    friend std::ostream& operator<< <>(std::ostream& out, const Agora& p) ;
    friend std::ostream& affiche <>(std::ostream& out, const Agora& p) ;
    friend std::ostream& details <>(std::ostream& out, const Agora& p) ;
    friend std::istream& operator>> <>(std::istream& in, Agora& p) ;

};

#include "agora_init.inl"
#include "agora_io.inl"
#include "agora_jouer.inl"
#include "agora_evaluation.inl"
#include "agora_alphabeta.inl"
#include "agora_coupspossibles.inl"
#endif
