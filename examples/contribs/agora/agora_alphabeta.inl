// =================================================================== //
// Time-stamp: <27 Jun 03 17:07:18 Jean-Guillaume.Dumas@imag.fr> 
// =================================================================== //
#ifndef __Agora_AlphaBeta__
#define __Agora_AlphaBeta__

#include <iostream>
#include <vector>
#include <algorithm>
#include <deque>
#include "givtimer.h"
#include "agora.h"

#define START_PROFONDEUR 5

#define WINNER 400
#define LOOSER -500
#define INFTYOT 300
#define MINFTYOT -INFTYOT
#define INFTY 800
#define MINFTY -INFTY
#define DINFTY 1600
#define MDINFTY -DINFTY
      
 
template<class Ints>
inline double Agora<Ints>::ami_ab(int depth, double alpha, double beta, bool ordi_color) {
          // always : A < B
        if ( depth <= 0 ) return (ordi_color?-eval():eval());
        int d  = depth -1 ;
        CoupsPossibles( _PM[d], ordi_color);
    
        if ( _PM[d].size() <= 0 )  return (LOOSER-depth+(ordi_color?-eval():eval()));

        Pile_t dep, arr;
        for(typename Possible_t::const_iterator pi = _PM[d].begin(); 
            pi != _PM[d].end() ; 
            ++pi) {
            jouer( (*pi).depart(), dep, (*pi).arrivee(), arr, (*pi).dessus());
            double ab = ennemi_ab(d, alpha, beta, ordi_color);
            remettre( (*pi).depart(), dep, (*pi).arrivee(), arr );
            if (ab > alpha) alpha = ab;
            if (alpha >= beta) 
                return beta;   
        }
        return alpha;
    }

template<class Ints>
inline double Agora<Ints>::ennemi_ab(int depth, double alpha, double beta, bool ordi_color) {
            // always : A < B
        if ( depth <= 0 ) return (ordi_color?-eval():eval());
        int d  = depth -1 ;
        bool cenemy = !ordi_color;
        CoupsPossibles( _PM[d], cenemy);
        if ( _PM[d].size() <= 0 )  return (WINNER+depth+(ordi_color?-eval():eval()));
        
        Pile_t dep, arr, ndep, narr;
        for(typename Possible_t::const_iterator pi = _PM[d].begin(); 
            pi != _PM[d].end() ; 
            ++pi) {
            jouer( (*pi).depart(), dep, ndep, (*pi).arrivee(), arr, narr, (*pi).dessus());
            double ab = ami_ab(d, alpha, beta, ordi_color, eval(), dep, ndep, arr, narr);
            remettre( (*pi).depart(), dep, (*pi).arrivee(), arr );
            if (ab < beta) beta = ab;
            if (alpha >= beta) 
                return alpha ;   
        }
        return beta;
    }



template<class Ints>
inline double Agora<Ints>::ami_ab(int depth, double alpha, double beta, bool ordi_color, double currev, const Pile_t ad, const Pile_t nd, const Pile_t aa, const Pile_t na) {
    eval( currev, ad, nd, aa, na );

          // always : A < B
        if ( depth <= 0 ) return (ordi_color?-currev:currev);
        int d  = depth -1 ;
        CoupsPossibles( _PM[d], ordi_color);
    
        if ( _PM[d].size() <= 0 )  return (LOOSER-depth+(ordi_color?-currev:currev));

        Pile_t dep, arr, ndep, narr;
        for(typename Possible_t::const_iterator pi = _PM[d].begin(); 
            pi != _PM[d].end() ; 
            ++pi) {
            jouer( (*pi).depart(), dep, ndep, (*pi).arrivee(), arr, narr, (*pi).dessus());
            double ab = ennemi_ab(d, alpha, beta, ordi_color, currev, dep, ndep, arr, narr);
            remettre( (*pi).depart(), dep, (*pi).arrivee(), arr );
            if (ab > alpha) alpha = ab;
            if (alpha >= beta) 
                return beta;   
        }
        return alpha;
    }

template<class Ints>
inline double Agora<Ints>::ennemi_ab(int depth, double alpha, double beta, bool ordi_color, double currev, const Pile_t ad, const Pile_t nd, const Pile_t aa, const Pile_t na) {
            // always : A < B
    eval( currev, ad, nd, aa, na );

        if ( depth <= 0 ) return (ordi_color?-currev:currev );
        int d  = depth -1 ;
        bool cenemy = !ordi_color;
        CoupsPossibles( _PM[d], cenemy);
        if ( _PM[d].size() <= 0 )  return (WINNER+depth+(ordi_color?-currev:currev));
        
        Pile_t dep, arr, ndep, narr;
        for(typename Possible_t::const_iterator pi = _PM[d].begin(); 
            pi != _PM[d].end() ; 
            ++pi) {
            jouer( (*pi).depart(), dep, ndep, (*pi).arrivee(), arr, narr, (*pi).dessus());
            double ab = ami_ab(d, alpha, beta, ordi_color, currev, dep, ndep, arr, narr);
            remettre( (*pi).depart(), dep, (*pi).arrivee(), arr );
            if (ab < beta) beta = ab;
            if (alpha >= beta) 
                return alpha ;   
        }
        return beta;
    }


template<class Ints>
inline double Agora<Ints>::MTDFdouble(int depth, double f, bool ordi_color) {
        double g = (f>LOOSER)?f:0;
        double lowerbound = MINFTY;
        double upperbound = INFTY;
        double beta; int count = 0;
        for(double diffbound = DINFTY;
            diffbound > 1 ;
            ++count, diffbound = upperbound-lowerbound)  {
            if (count > 3) {
                g = ennemi_ab(depth, lowerbound, upperbound, ordi_color);
                break;
            }
            if (g == lowerbound)
                beta = g+1;
            else if (g == upperbound)
                beta = g-1;
            else
                beta = g;
            g = ennemi_ab(depth, beta - 1, beta + 1, ordi_color);
            if (g < beta)
                upperbound = g;
            else if (g > beta)
                lowerbound = g;
            else
                break;
        }
        return g;
    }









template<class Ints>
inline double Agora<Ints>::BestPlay(Coup& bp, bool ordi_color, int depth) {
#ifdef __ERRORS__
        _count = 0;
#endif
#ifdef __DEPTHS__
        std::cerr << "d" << depth << " ";
#endif
        Size_t ls = TopPM.size();
        
        typename Possible_t::iterator beg_pi = TopPM.begin(), end_pi = TopPM.end();
        double e = MINFTY;
        bp = *beg_pi;

        if (ls > 1) {
            std::vector<double> e_choix; e_choix.resize( ls );
            std::vector<double>::iterator eci = e_choix.begin();
#ifdef DEGRESSIVE_DEPTH
            int _mmod = -1;
            int _mdiv = (depth<7)?(depth+10):7;
            int _mmid = depth /2;
#endif
            Pile_t dep, arr;
            for(typename Possible_t::iterator pi = beg_pi ; pi != end_pi; ++pi, ++eci) {
#ifdef DEGRESSIVE_DEPTH
                int d = (++_mmod / _mdiv)*2;
                d = (d<_mmid)?depth - 1 - d: _mmid;
#else
                int d = depth - 1;
#endif
#ifdef __DEPTHS__
                std::cerr << "[d: " << d << "] ";
#endif
                jouer( (*pi).depart(), dep, (*pi).arrivee(), arr, (*pi).dessus());
		// Prevent arr_coul unused variable warning message 
                //bool arr_coul = jouer( (*pi).depart(), dep, (*pi).arrivee(), arr, (*pi).dessus());
                double em = MTDFdouble(d, e, ordi_color);
                remettre( (*pi).depart(), dep, (*pi).arrivee(), arr);
#ifdef __DEPTHS__
#ifdef __ERRORS__
                std::cerr << *pi << ":" << em << " (" << _count << ")  " << std::endl;
#else
                std::cerr << *pi << ":" << em << "  ";
#endif
#endif

                if ( (em > e) 
                    // A jeu égal, on préfère un prisonnier
		    // || ((em == e) && (couleur(dep) != arr_coul)) 
                     )     
                {
                    e = em;
                    bp = *pi;
                }
                Coup temp(*pi);
                typename Possible_t::iterator sorti = pi, lpi = pi;
                std::vector<double>::iterator preci = eci, lci = eci;
                for(--preci, --sorti ; (lpi != beg_pi) && (em > *preci); --preci, --sorti, --lci, --lpi ) {
                    *lci = *preci;
                    *lpi = *sorti;
                }
                *lci = em;
                *lpi = temp;
           }
#ifdef __DEPTHS__
            std::cerr << std::endl;
#endif
        }
#ifdef __ERRORS__
        std::cerr << "Evals: " << _count << std::endl;
#endif

        return e;
    }



    
template<class Ints>
inline double Agora<Ints>::OnebyOneBP(Coup& bp, bool ordi_color, int depth, double temps_max, Timer& global) {
        Timer tim; tim.clear(); tim.start();

        int d = depth-1;
        Size_t ls = TopPM.size();
        
        typename Possible_t::iterator beg_pi = TopPM.begin(), end_pi = TopPM.end();
        double e = MINFTY;
        bp = *beg_pi;
        if (ls > 1) {
#ifdef __DEPTHS__
            std::cerr << "d" << depth << " ";
#endif

            typename Possible_t::iterator pi = beg_pi ;
            Pile_t dep, arr;
        
            jouer( (*pi).depart(), dep, (*pi).arrivee(), arr, (*pi).dessus());
	    // Prevent unused variable `bool arr_coul' warning message
            // bool arr_coul = jouer( (*pi).depart(), dep, (*pi).arrivee(), arr, (*pi).dessus());
            double em = MTDFdouble(d, e, ordi_color);
            remettre( (*pi).depart(), dep, (*pi).arrivee(), arr);
#ifdef __DEPTHS__
            std::cerr << *pi << ":" << em << "  ";
#endif
            if ( (em > e)
                 // A jeu égal, on préfère un prisonnier
	         // || ((em == e) && (couleur(dep) != arr_coul)) 
                 )     
            {
                e = em;
                bp = *pi;
            }
            tim.stop();
            global += tim;
            double devia = tim.realtime();

            for(++pi ; ((global.realtime()+devia) < temps_max) && (pi != end_pi) ; ++pi, global += tim, devia = (devia+tim.realtime())/2 ) {
                tim.start();
                jouer( (*pi).depart(), dep, (*pi).arrivee(), arr, (*pi).dessus());
		// Prevent unused variable `bool arr_coul' warning message
                // bool arr_coul = jouer( (*pi).depart(), dep, (*pi).arrivee(), arr, (*pi).dessus());
                em = MTDFdouble(d, e, ordi_color);
                remettre( (*pi).depart(), dep, (*pi).arrivee(), arr);
#ifdef __DEPTHS__
                std::cerr << *pi << ":" << em << "  ";
#endif
                if ( (em > e)
                     // A jeu égal, on préfère un prisonnier
		     // || ((em == e) && (couleur(dep) != arr_coul)) 
                     )     
                {
                    e = em;
                    bp = *pi;
                }
                tim.stop();
            }
#ifdef __DEPTHS__
            std::cerr << std::endl;
#endif
        }
        return e;
    }


template<class Ints>
inline typename Agora<Ints>::Coup& Agora<Ints>::Suggest(Coup& bp, bool ordi_color, double temps_max, int depth) {
    if (depth > 0) {
        Timer tim; tim.clear();tim.start();
        CoupsPossibles( TopPM, ordi_color );
        std::random_shuffle(TopPM.begin(), TopPM.end() );
        BestPlay(bp, ordi_color, 1);
        _PM.resize(depth);
        BestPlay(bp, ordi_color, depth);
        tim.stop();
        if (tim.realtime() < temps_max)
            Iteratif(bp, ordi_color, temps_max-tim.realtime(), depth+1);
        return bp;
    } else {
        return Iteratif(bp, ordi_color, temps_max);
    }
}



template<class Ints>
inline typename Agora<Ints>::Coup& Agora<Ints>::Iteratif(Coup& bp, bool ordi_color, double temps_max, int StartProf = START_PROFONDEUR) {
        Timer tim, global; tim.clear(); global.clear(); global.start();
        double SEUIL =  22.0 / 4.0;
        SEUIL = (SEUIL < 3)? 3 : SEUIL ;
        CoupsPossibles( TopPM, ordi_color );
        double ev = BestPlay(bp, ordi_color, 1);
        global.stop();
        
        
        int profondeur = StartProf ;
        for(;   ( global.realtime()*SEUIL < temps_max ) 
                && (ev > MINFTYOT) 
                && (ev < INFTYOT)
                ;  ++profondeur, global += tim ) {
                
            tim.start();
            _PM.resize(profondeur);
            ev = BestPlay(bp, ordi_color, profondeur);
            tim.stop();
        }

        if ( ((global.realtime()*5) < (temps_max*4)) && (ev > MINFTYOT) && (ev < INFTYOT) ) {
            _PM.resize(profondeur);
            ev = OnebyOneBP(bp, ordi_color, profondeur, temps_max, global);
        }
#ifdef __DEPTHS__
        std::cerr << "time: " << global << std::endl;
#endif
        return  bp;
    }
    

#endif
