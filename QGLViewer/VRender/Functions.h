#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#include <iostream>

#define DumpCore() { std::cerr << "Dumping core." << std::endl ; ((int *)0x0)[100000] = 10 ; }

#ifndef FLT_MAX
#define FLT_MAX 3.40282347e+38F
#endif
#ifndef FLT_MIN
#define FLT_MIN 1.17549435E-38F
#endif

template<class T> inline void Exchange(T& a,T& b) { T tmp = a ; a = b ; b = tmp ; }

#endif


