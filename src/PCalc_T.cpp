#include "PCalc_T.h"


PCalc_T::PCalc_T(unsigned int array_size, unsigned int num_threads) : PCalc(array_size), n_threads(num_threads){};

PCalc_T::~PCalc_T() {
   cleanup();
}

void PCalc_T::markNonPrimes()

{
    for (unsigned int i = 2; i*i <= this->array_size(); i++)
        if (this->at(i) == true)
            for (int p = i*i; p <= this->array_size(); p += i)
                this->at(p) = false;    
}