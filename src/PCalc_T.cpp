#include "PCalc_T.h"
#include <thread>
#include <vector>
#include <iostream>


PCalc_T::PCalc_T(unsigned int array_size, unsigned int num_threads) : PCalc(array_size), n_threads(num_threads)
{
    for(int i=0; i<num_threads; i++)
        this->workList.push_back(0);
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
                for (auto it = this->workList.begin(); it != this->workList.end(); it++) // loop through until it finds a thread to assign
                {
                    if (*it == 0)
                    {
                        *it = i; // give it to a thread, except this doesn't update the value
                        notAssigned = false;
                        break;
                    }
                }
            }
        }
    }

    for (int i = 0; i < n_threads; i++)
        this->workList.at(i) = -1;
    for (auto it = this->threadList.begin(); it != this->threadList.end(); it++)
        it->join(); // tell them to die
    
}

void PCalc_T::spawnThreads()
{
    auto primeLambda = [this](int i)
    {
        while(this->workList.at(i) != -1)
        {
            int workInt = this->workList.at(i);
            if(workInt != 0)
            {    
                this->workList.at(i) = 0;
                for (int p = workInt*workInt; p <= this->array_size(); p += workInt)
                    this->at(p) = false;
            }
        }
    };

    for(int j = 0; j < this->n_threads; j++)
        this->threadList.push_back(std::thread(primeLambda, j));
}


