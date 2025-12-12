#ifndef RANDOMSUPPORT_H
#define RANDOMSUPPORT_H

#include <cstdlib>
#include <ctime>

struct RNG{
private:
int lower;
int upper;



public:

RNG() {
    srand(static_cast<unsigned int>(time(0)));
    lower=0;
    upper=9;
}

RNG(int lower, int upper){
    srand(static_cast<unsigned int>(time(0)));
    this->lower = lower; 
    this->upper = upper;
}

int get() {
    return lower + (rand() % (upper - lower + 1));
}

void setLimits(int lower, int upper) {
    this->lower = lower; 
    this->upper = upper;
}
};

#endif
