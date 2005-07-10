#include <fstream>
#include <algorithm>

#include "container_stream.h"
#include "agora.h"
#include "givtimer.C"


typedef Agora<unsigned short> AGORA;
typedef AGORA::Pile_t Pile;
typedef AGORA::Coup   Coup;
typedef AGORA::Possible_t   Possibles;

int main(int argc, char * argv[]) {

    double ww = atof (argv[1]) ;

    bool color = WHITE;
    if (ww > 0)
        color = WHITE;
    else if (ww < 0) {
        color = BLACK;
        ww = -ww;
    }
    

    Timer cstr, init;
    cstr.clear();
    cstr.start();
    AGORA Jeu;
    cstr.stop();
    std::cerr << "Cstruct: " << cstr << std::endl;
    
    init.clear();
    init.start();
//    std::ifstream F_init("Agora.init6");
//     F_init >> Jeu;
    Jeu.reinit();
    init.stop();
    std::cerr << "Init   : " << init << std::endl;
    std::cerr << "Total  : " << (init+=cstr) << std::endl;
   
    details( std::cerr , Jeu ) << std::endl;

//     std::ofstream IA_Out("Agora.IA", std::ios_base::out);     
//     std::ofstream Ext_Out("Agora.ext", std::ios_base::out);     
    std::ofstream IA_Out("Agora.IA");     
    std::ofstream Ext_Out("Agora.ext");    


//     Jeu.saveToFile( "Toto1" );
//     Jeu.loadFromFile( "Toto1" );
    



    std::cerr << Jeu << std::endl;
    

   
    Coup m;   
    if (color == WHITE) {
        for( Possibles Liste(1); Liste.size()>0; ) {
            Jeu.Suggest(m, color, ww);
            std::cout << m << std::endl; IA_Out << m << std::endl; 
            Jeu.jouer( m );
            affiche(std::cerr , Jeu);
            Jeu.CoupsPossibles( Liste, !color) ;
	    if (Liste.size() == 0) {
		    std::cerr << "L'ordinateur a gagné" << std::endl;
		    return 0;
	    }
            std::cerr << Liste << " : " << Jeu.eval() << std::endl;
            std::cerr << "Entrez un coup : " << std::flush;
            std::cin >> m; 
            while( std::find(Liste.begin(), Liste.end(), m) == Liste.end() ) {
                if ( m == Coup(0,0,0) ) return 0;
                std::cerr << "Error, coup invalide : " << m << std::endl;
                std::cin >> m;
            }
            Jeu.jouer(m); Ext_Out << m << std::endl; 
            affiche(std::cerr , Jeu);
            Jeu.CoupsPossibles( Liste, color) ;
	    if (Liste.size() == 0) {
		    std::cerr << "Vous avez gagné" << std::endl;
		    return 0;
	    }
        }
    } else {
        for( Possibles Liste(1); Liste.size()>0; ) {
            affiche(std::cerr , Jeu);
            Jeu.CoupsPossibles( Liste, !color) ;
	    if (Liste.size() == 0) {
		    std::cerr << "L'ordinateur a gagné" << std::endl;
		    return 0;
	    }
            std::cerr << Liste << " : " << Jeu.eval() << std::endl;
            std::cerr << "Entrez un coup : " << std::flush;
            std::cin >> m; 
            while( std::find(Liste.begin(), Liste.end(), m) == Liste.end() ) {
                if ( m == Coup(0,0,0) ) return 0;
                std::cerr << "Error, coup invalide : " << m << std::endl;
                std::cin >> m;
            }
            Jeu.jouer(m); Ext_Out << m << std::endl; 
            affiche(std::cerr , Jeu);
            Jeu.CoupsPossibles( Liste, color) ;
	    if (Liste.size() == 0) {
		    std::cerr << "Vous avez gagné" << std::endl;
		    return 0;
	    }
            Jeu.Suggest(m, color, ww);
            std::cout << m << std::endl; IA_Out << m << std::endl; 
            Jeu.jouer( m );
        }
    }

    return 0;
    
}
