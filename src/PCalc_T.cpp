#include "PCalc_T.h"
#include <thread>
#include <vector>
#include <iostream>
#include <mutex>


PCalc_T::PCalc_T(unsigned int array_size, unsigned int num_threads) : PCalc(array_size), n_threads(num_threads), workInt(0){
};

PCalc_T::~PCalc_T() {
   cleanup();
}

void PCalc_T::markNonPrimes()
{
    this->spawnThreads();
    this->at(0) = false; this->at(1) = false;

    for (unsigned int i = 2; i*i <= this->array_size(); i++)
    {
        if (this->at(i) == true)
        {
            bool notAssigned = true;
            while(notAssigned)
            {   
                if (workInt == 0)
                {
                    while(true)
                        if(this->workIntMutex.try_lock())
                            break;
                    workInt = i; // give it to a thread
                    this->workIntMutex.unlock();
                    notAssigned = false;
                    break;
                }
            }
        }
    }
    
    this->doneWork = -1;
    
    for (auto it = this->threadList.begin(); it != this->threadList.end(); it++)
        it->join(); // tell them to die, hangs here
    

}

void PCalc_T::spawnThreads()
{
    auto primeLambda = [this](int i)
    {
        while(this->doneWork != -1)
        {
            if(this->workInt != 0)
            {   
                int workNow = this->workInt;
                while(true)
                    if(this->workIntMutex.try_lock())
                        break;
                this->workInt = 0;
                this->workIntMutex.unlock();
                for (int p = workNow*workNow; p <= this->array_size(); p += workNow)
                    this->at(p) = false;
            }
        }
    };

    for(int j = 0; j < this->n_threads-1; j++)
        this->threadList.push_back(std::thread(primeLambda, j));
}


