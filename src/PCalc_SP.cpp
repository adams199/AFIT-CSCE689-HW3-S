# include "PCalc_SP.h"


PCalc_SP::PCalc_SP(unsigned int array_size) : PCalc(array_size)
{
};

void PCalc_SP::markNonPrimes()

{
            for (unsigned int i = 2; i*i <= this->array_size; i++)
            {
                if (this->primelist[i] == true)
                {
                    for (int p = i*i; p <= this->array_size; p += i)
                    {
                        this->primelist[p] = false;
                    }
                    
                }
                
            }
            for (unsigned int q = 2; q < this->array_size; q++)
                if (primelist[q])
                    std::cout << q << '\n'; // just to check before file        
}