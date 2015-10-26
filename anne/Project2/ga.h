#if !defined(GA_H)

#include <math.h>

#define MutationRate 0.01
#define GenerationCount 100
#define PopulationSize 100
#define ChromosomeSize 2

struct individual
{
    r32 Chromosome[ChromosomeSize];
};

struct population
{
    individual Individuals[PopulationSize];
};

struct individual_fitness
{
    u32 IndividualIndex;
    r32 FitnessScore;
};

#define GA_H
#endif
