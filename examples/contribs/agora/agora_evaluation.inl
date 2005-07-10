// =================================================================== //
// Time-stamp: <30 Jun 03 18:59:17 Jean-Guillaume.Dumas@imag.fr> 
// =================================================================== //
#ifndef __Agora_Evaluation__
#define __Agora_Evaluation__

#include "agora.h"

template<class Ints>
inline Eval_t Agora<Ints>::eval(const Pile_t p) const {
//     std::cerr << "p: " << p << ", e: " << _eval[p] << std::endl;
    return _eval[p];
}   



template<class Ints>
inline double& Agora<Ints>::eval(double& e, const Pile_t p)  const {
#ifdef __ERRORS__
    ++_count;
#endif
    return e += eval(p);
}

template<class Ints>
inline double& Agora<Ints>::uneval(double& e, const Pile_t p)  const {
#ifdef __ERRORS__
    ++_count;
#endif
    return e -= eval(p);
}

template<class Ints>
inline double& Agora<Ints>::eval(double& e, const Pile_t dep, const Pile_t ndep, const Pile_t arr, const Pile_t narr)  const {
    uneval(e, dep);
    eval(e, ndep);
    uneval(e, arr);
    return eval(e, narr);
}

template<class Ints>
inline double Agora<Ints>::eval()  const {
#ifdef __ERRORS__
    ++_count;
#endif
    double e=0.0;
    for(typename Self_t::const_iterator it = this->begin() ; it != this->end(); ++it)
        eval(e, *it);
    return e;    
}


#endif
