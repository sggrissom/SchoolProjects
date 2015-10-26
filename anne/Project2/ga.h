#if !defined(GA_H)

#include <math.h>

#define MutationRate 0.3
#define MutationMagnitude 10
#define GenerationCount 1000
#define PopulationSize 1000
#define ChromosomeSize 2

#define LowerBound -10
#define UpperBound 10

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
