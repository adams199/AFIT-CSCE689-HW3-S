#include "PCalc_T.h"
#include <thread>
#include <vector>
#include <iostream>
#include <mutex>


PCalc_T::PCalc_T(unsigned int array_size, unsigned int num_threads) : PCalc(array_size), n_threads(num_threads), currentnThreads(0), currentMax(2){
};

PCalc_T::~PCalc_T() {
   cleanup();
}

void PCalc_T::markNonPrimes()
{
    this->at(0) = false; this->at(1) = false;

    for (unsigned int i = 2; i*i <= this->array_size(); i++)
    {
        while(true) // make sure its safe
            if(this->workMutex.try_lock())
                break;
        unsigned int nowMax = this->currentMax; // need to wait till all threads passe this point
        this->workMutex.unlock();

        if (this->at(i) == true)
        {
            if(i <= nowMax && currentnThreads < n_threads) // make sure we can spawn one correctly
            {
                this->spawnThread(i);
                this->currentnThreads++;
            }
            else
                i--; //lets hold till we pass with max or a thread comes open
        }
    }
      
    
    for (auto it = this->threadList.begin(); it != this->threadList.end(); it++)
        it->join(); // tell them to die
    

}

void PCalc_T::spawnThread(unsigned int work)
{
    auto primeLambda = [this](unsigned int workNow)
    {
        for (unsigned int p = workNow*workNow; p <= this->array_size(); p += workNow)
        {
            this->at(p) = false;  // do the work
            while(true) // make sure its safe
                if(this->workMutex.try_lock())
                    break;
            this->currentMax = p; // need to wait till all threads pass this point
            this->workMutex.unlock();
        }
        this->currentnThreads--;
    };
    this->threadList.push_back(std::thread(primeLambda, work));
}