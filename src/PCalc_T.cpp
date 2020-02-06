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
    int optoList[30] = {0,1,2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,101,103,107};
    for(int z = 0; z < 30; z++)
        this->at(optoList[z]) == false;


    for (unsigned int i = 2; i*i <= this->array_size(); i++)
    {
        if (this->at(i) == true)
        {
            if(currentnThreads < n_threads)
            {
                this->spawnThread(i);
                this->currentnThreads++;
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
        this->currentnThreads--;
    };
    this->threadList.push_back(std::thread(primeLambda, work));
}