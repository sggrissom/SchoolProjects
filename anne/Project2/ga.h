#if !defined(GA_H)

#define _CRT_RAND_S
#include "stdlib.h"
#include <math.h>

#define GenerationCount
#define PopulationSize 100
#define ChromosomeSize 2

struct individual
{
    float Chromosome[ChromosomeSize];
};

struct population
{
    individual Individuals[PopulationSize];
};

struct genetic_algorithm
{
    population *Population;
    float MutationRate;
    u32 Generations;
};




#define GA_H
#endif
