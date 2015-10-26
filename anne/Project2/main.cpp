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
#include <math.h>
#include "slib.h"

#define GA 1
#define P1 0

#define MutationRate 0.3
#define MutationMagnitude 10
#define GenerationCount 50
#define PopulationSize 1000
#define ChromosomeSize 2

#if P1
#define LowerBound -2
#define UpperBound 4
#else
#define LowerBound -10
#define UpperBound 10
#endif

//macro to get a random float between two values
#define RandomBetween(min,max) (min + (r32)(rand() / ((r32)(RAND_MAX/(max - min)))))
#define IsBetween(test, min, max) ((test) < (min) ? 0 : ((test) > (max) ? 0 : 1))

internal r32
ValueScore(r32 X)
{
    r32 Result = 0;

    if (IsBetween(X, LowerBound, UpperBound))
    {
        Result = (r32)(atan(X) + sin(X));
    }

    return Result;
}

//theoretical max from -2 to 4: 1.80738
//theoretical max from -10 to 10: 7.86987
internal r32
FitnessScore(r32 X, r32 Y)
{    
    return ValueScore(X) + ValueScore(Y);
}

#include "ga.h"

#ifndef GA
internal void
HillClimb()
{
    r32 candidate[5];
    r32 currentPoint = 0;
    r32 stepSize = 0.1;
    r32 acceleration = 1.2;
    candidate[0] = -acceleration;
    candidate[1] = -1 / acceleration;
    candidate[2] = 0;
    candidate[3] = 1 / acceleration;
    candidate[4] = acceleration;
    for(;;)
    {
        r32 before = ValueScore(currentPoint);
        for each element i in currentPoint do
                     best = -1;
        bestScore = -INF;
        for j from 0 to 4         // try each of 5 candidate locations
                  currentPoint[i] = currentPoint[i] + stepSize[i] * candidate[j];
        temp = EVAL(currentPoint);
        currentPoint[i] = currentPoint[i] - stepSize[i] * candidate[j];
        if(temp > bestScore)
            bestScore = temp;
        best = j;
        if candidate[best] is not 0
                        currentPoint[i] = currentPoint[i] + stepSize[i] * candidate[best];
        stepSize[i] = stepSize[i] * candidate[best]; // accelerate
        if (EVAL(currentPoint) - before) < epsilon 
                                           return currentPoint;
    }    
}
#endif

s32 main()
{
#if 1
    srand((u32)time(0));
#else
    srand(0);
#endif

#if GA
    population *Population = (population *)malloc(sizeof(population));
    
    RandomizePopulation(Population);

    for(u32 IndividualIndex = 0;
        IndividualIndex < GenerationCount;
        ++IndividualIndex)
    {
        Population = ReproducePopulation(Population);
    }
#else
    HillClimb();
#endif

    free(Population);

    return 1;
}
