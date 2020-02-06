#include "PCalc_T.h"
#include <thread>
#include <vector>
#include <iostream>
#include <mutex>


PCalc_T::PCalc_T(unsigned int array_size, unsigned int num_threads) : PCalc(array_size), n_threads(num_threads), currentnThreads(0){
};

PCalc_T::~PCalc_T() {
   cleanup();
}

void PCalc_T::markNonPrimes()
{
    this->at(0) = false; this->at(1) = false; // 0 and 1 are not prime

    for (unsigned int i = 2; i*i <= this->array_size(); i++)
    {
        if (this->at(i) == true)
        {
            if(currentnThreads < n_threads)
            {
                this->workMutex.lock();
                this->currentnThreads++;
                this->workMutex.unlock(); // make sure nthreads isnt stepped on

                this->spawnThread(i);
            }
            else
                i--; //lets hold till a thread comes open
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
            this->at(p) = false;

        this->workMutex.lock();
        this->currentnThreads--;
        this->workMutex.unlock(); // make sure nthreads isnt stepped on
    };

    this->threadList.push_back(std::thread(primeLambda, work)); // add it to our thread list
}