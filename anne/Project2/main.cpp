/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Steven Grissom $
   ======================================================================== */

#define _CRT_RAND_S
#include "stdlib.h"
#include "string.h"
#include "time.h"
#include "stdio.h"
#include "slib.h"
#include "ga.h"

//macro to get a random float between two values
#define RandomBetween(min,max) (min + (r32)(rand() / ((r32)(RAND_MAX/(max - min)))))
#define IsBetween(test, min, max) ((test) < (min) ? 0 : ((test) > (max) ? 0 : 1))
#define Swap(x,y) do \
    { u8 swap_temp[sizeof(x) == sizeof(y) ? (signed)sizeof(x) : -1];    \
       memcpy(swap_temp,&y,sizeof(x)); \
       memcpy(&y,&x,       sizeof(x)); \
       memcpy(&x,swap_temp,sizeof(x)); \
    } while(0)

internal u32
LotteryWinner(u32 LotteryEntrantsCount)
{
    u32 LotteryTicketCount = LotteryEntrantsCount * (LotteryEntrantsCount + 1) / 2;
    printf("%d\n", LotteryTicketCount);

    u32 ChosenTicket = (u32)(RandomBetween(0, (r32)LotteryTicketCount) + 0.5f);
    ChosenTicket = (ChosenTicket >= LotteryTicketCount) ? LotteryTicketCount-1 : ChosenTicket;

    u32 WinnerIndex = 0;
    u32 TicketIndex = ChosenTicket;

    u32 TicketCutoff = PopulationSize;
    while(TicketIndex >= TicketCutoff--)
    {
        ++WinnerIndex;
        TicketIndex -= TicketCutoff;
    }
    
    return WinnerIndex;
}

internal void
PrintPopulation(population *Population)
{
    for(u32 IndividualIndex = 0;
        IndividualIndex < PopulationSize;
        ++IndividualIndex)
    {
        printf("(%f,%f)\n", Population->Individuals[IndividualIndex].Chromosome[0],
               Population->Individuals[IndividualIndex].Chromosome[1]);
    }
}

//bubble sort for ease of implementation
internal void
SortFitnesses(individual_fitness *Fitnesses)
{
    b32 Swapped = true;
    
    for(u32 SortedIndex = 1;
        SortedIndex < PopulationSize && Swapped;
        ++SortedIndex)
    {
        Swapped = false;
        if(Fitnesses[SortedIndex-1].FitnessScore < Fitnesses[SortedIndex].FitnessScore)
        {
            Swap(Fitnesses[SortedIndex-1], Fitnesses[SortedIndex]);
            Swapped = true;
        }
    }

    printf("%f - %f\n", Fitnesses[0].FitnessScore, Fitnesses[1].FitnessScore);
}

internal float
FitnessScore(individual *Individual)
{
    float Result = 0;

    float X = Individual->Chromosome[0];
    float Y = Individual->Chromosome[1];

    if (IsBetween(X, -2, 4) && IsBetween(Y, -2, 4))
    {
        Result = atan(X) + atan(Y) + sin(X) + sin(Y);
    }
    
    return Result;
}

internal void
Mutate(individual *Individual)
{
    for(u32 ChromosomeIndex = 0;
        ChromosomeIndex < ChromosomeSize;
        ++ChromosomeIndex)
    {
        r32 Random = RandomBetween(0,1);
        if(Random < MutationRate)
        {
            r32 Offset = RandomBetween(0,1);
            Offset = pow(Offset, 5);
            Individual->Chromosome[ChromosomeIndex] += Offset;
        }
    }
}

internal population *
ReproducePopulation(population *Population)
{
    individual_fitness PopulationFitnessRanked[PopulationSize] = {};
    
    for(u32 IndividualIndex = 0;
        IndividualIndex < PopulationSize;
        ++IndividualIndex)
    {
        PopulationFitnessRanked[IndividualIndex].IndividualIndex = IndividualIndex;
         PopulationFitnessRanked[IndividualIndex].FitnessScore =
            FitnessScore(&Population->Individuals[IndividualIndex]);
    }

    SortFitnesses(PopulationFitnessRanked);

    population *NewPopulation = (population *)malloc(sizeof(population));

    for(u32 IndividualIndex = 0;
        IndividualIndex < PopulationSize;
        ++IndividualIndex)
    {
        u32 LotteryWinnerFitnessIndex = LotteryWinner(PopulationSize);
        u32 LotteryWinnerIndex = PopulationFitnessRanked[LotteryWinnerFitnessIndex].IndividualIndex;
        individual LotteryWinner = Population->Individuals[LotteryWinnerIndex];
        Mutate(&LotteryWinner);
        NewPopulation->Individuals[IndividualIndex] = LotteryWinner;
    }

    free(Population);
    
    return NewPopulation;
}

internal void
RandomizePopulation(population *Population)
{
    for(u32 IndividualIndex = 0;
        IndividualIndex < PopulationSize;
        ++IndividualIndex)
    {
        individual *Individual = &Population->Individuals[IndividualIndex];
        for(u32 ChromosomeIndex = 0;
            ChromosomeIndex < ChromosomeSize;
            ++ChromosomeIndex)
        {
            Individual->Chromosome[ChromosomeIndex] = RandomBetween(-4,4);
        }
    }
}

s32 main()
{
    srand((u32)time(0));

    population *Population = (population *)malloc(sizeof(population));

    RandomizePopulation(Population);

    for(u32 IndividualIndex = 0;
        IndividualIndex < GenerationCount;
        ++IndividualIndex)
    {
        //PrintPopulation(Population);
        printf("--------\n");
        Population = ReproducePopulation(Population);
    }

    return 1;
}
