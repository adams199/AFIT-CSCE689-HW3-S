#include "PCalc_SP.h"


PCalc_SP::PCalc_SP(unsigned int array_size) : PCalc(array_size){};

PCalc_SP::~PCalc_SP() {
   cleanup();
}

void PCalc_SP::markNonPrimes()
{
    this->at(0) = false; this->at(1) = false;
    for (unsigned int i = 2; i*i <= this->array_size(); i++)
        if (this->at(i) == true)
            for (unsigned int p = i*i; p <= this->array_size(); p += i)
                this->at(p) = false;    
}