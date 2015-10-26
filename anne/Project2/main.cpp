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

#define LINE_BREAK printf("--------\n");

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
    //printf("TicketCount: %d\n", LotteryTicketCount);

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

internal void
PrintArray(individual_fitness *Array, u32 Size)
{
    LINE_BREAK
    for(u32 ArrayIndex = 0;
        ArrayIndex < Size;
        ++ArrayIndex)
    {
        printf("%f\n", Array[ArrayIndex].FitnessScore);
    }
    LINE_BREAK
}

internal void
Quicksort(individual_fitness *Array, u32 First, u32 Last)
{
    printf("start sort\n");
    
    u32 Pivot, I, J;
    individual_fitness Temp;

    if(First < Last)
    {
        Pivot = First;
        I = First;
        J = Last;

        while(I < J)
        {
            while(Array[I].FitnessScore >= Array[Pivot].FitnessScore && I < Last)
            {
                ++I;
            }
            while(Array[J].FitnessScore < Array[Pivot].FitnessScore)
            {
                --J;
            }
            if(I < J)
            {
                Temp = Array[I];
                Array[I] = Array[J];
                Array[J] = Temp;
            }
        }

        Temp = Array[Pivot];
        Array[Pivot] = Array[J];
        Array[J] = Temp;
        Quicksort(Array, First, J-1);
        Quicksort(Array, J+1, Last);
    }
    
    printf("end sort\n");
}

internal r32
FitnessScore(individual *Individual)
{
    r32 Result = 0;

    r32 X = Individual->Chromosome[0];
    r32 Y = Individual->Chromosome[1];

    if (IsBetween(X, -2, 4) && IsBetween(Y, -2, 4))
    {
        Result = atan(X) + atan(Y) + sin(X) + sin(Y);

        printf("x=%f\ny=%f\nresult=%f\n\n", X, Y, Result);
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

    PrintArray(PopulationFitnessRanked, PopulationSize);

    Quicksort(PopulationFitnessRanked, 0, PopulationSize-1);

    PrintArray(PopulationFitnessRanked, PopulationSize);

    individual Best = Population->Individuals[PopulationFitnessRanked[0].IndividualIndex];
    printf("Best Individual: (%f, %f)\n", Best.Chromosome[0], Best.Chromosome[1]);

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
        LINE_BREAK
        PrintPopulation(Population);
        Population = ReproducePopulation(Population);
    }

    return 1;
}
