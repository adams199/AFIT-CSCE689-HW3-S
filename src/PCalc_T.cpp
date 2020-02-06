#include "PCalc_T.h"
#include <thread>
#include <vector>
#include <iostream>
#include <mutex>


PCalc_T::PCalc_T(unsigned int array_size, unsigned int num_threads) : PCalc(array_size), n_threads(num_threads){
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
            while(true) // make sure its safe
                if(this->workIntMutex.try_lock())
                    break;
            this->workQ.push(i); // put the work on the queue
            this->workIntMutex.unlock();
        }
    }
      
    this->doneWork = -1;
    
    for (auto it = this->threadList.begin(); it != this->threadList.end(); it++)
        it->join(); // tell them to die
    

}

void PCalc_T::spawnThreads()
{
    auto primeLambda = [this]()
    {
        while(this->doneWork != -1 || !this->workQ.empty())
        {
            if(!this->workQ.empty())
            {
                while(true) // make sure we're the only ones grabbing it
                    if(this->workIntMutex.try_lock())
                        break;
                int workNow = this->workQ.front(); // grab the front of the queue
                this->workQ.pop(); // pop off the value
                this->workIntMutex.unlock();

                for (int p = workNow*workNow; p <= this->array_size(); p += workNow)
                    this->at(p) = false;  // do the work
            }
        }
    };

    for(int j = 0; j < this->n_threads-1; j++)
        this->threadList.push_back(std::thread(primeLambda));
}