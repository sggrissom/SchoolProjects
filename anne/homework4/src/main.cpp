/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Steven Grissom $
   ======================================================================== */

#define _CRT_RAND_S
#include "stdlib.h"
#include "stdio.h"
#include "time.h"
#include "slib.h"

#define Random1To100(number) rand_s(&number);number = ((u32) ((r32)number / ((r32) UINT_MAX + 1 ) * 100.0) + 1);

#define ChromosomeSize 100
#define PopulationSize 100
#define PossibleAlleleCount (2 * ChromosomeSize)

struct individual
{
    b32 Chromosome[ChromosomeSize];
};

struct population
{
    individual Individuals[PopulationSize];
};

internal void
PrintIndividual(individual Individual)
{
    for(u32 LocusIndex = 0;
        LocusIndex < ChromosomeSize;
        ++LocusIndex)
    {
        b32 Allele = Individual.Chromosome[LocusIndex];
        printf("a%d: %d\n", LocusIndex, Allele);
    }
}

//allele ID = LocusIndex + ChromosomeSize * LocusValue
internal r32
PercentOfPossibleAlleles(population *Population)
{
    b32 PossibleAlleles[PossibleAlleleCount] = {};
    
    for(u32 IndividualIndex = 0;
        IndividualIndex < PopulationSize;
        ++IndividualIndex)
    {
        individual Individual = Population->Individuals[IndividualIndex];
        for(u32 LocusIndex = 0;
            LocusIndex < ChromosomeSize;
            ++LocusIndex)
        {
            b32 Allele = Individual.Chromosome[LocusIndex];
            u32 AlleleID = LocusIndex + ChromosomeSize * Allele;
            PossibleAlleles[AlleleID] = 1;
        }
    }

    u32 PresentAlleles = 0;

    for(u32 PossibleAlleleIndex = 0;
        PossibleAlleleIndex < PossibleAlleleCount;
        ++PossibleAlleleIndex)
    {
        PresentAlleles += PossibleAlleles[PossibleAlleleIndex];
    }

    return ((r32)PresentAlleles)/((r32)PossibleAlleleCount);
}

internal void
MutatePopulation(population *Population)
{
    for(u32 IndividualIndex = 0;
        IndividualIndex < PopulationSize;
        ++IndividualIndex)
    {
        individual *Individual = &Population->Individuals[IndividualIndex];
        for(u32 LocusIndex = 0;
            LocusIndex < ChromosomeSize;
            ++LocusIndex)
        {
            u32 RandomValue;
            Random1To100(RandomValue);
            if(RandomValue == 100)
            {
                Individual->Chromosome[LocusIndex] = !Individual->Chromosome[LocusIndex];
            }
        }
    }
}

s32 main()
{
    srand((u32)time(0));
    
    population Population = {};

    for(u32 PopulationGeneration = 0;
        PopulationGeneration < 300;
        ++PopulationGeneration)
    {
        r32 PercentPossibleAlleles = PercentOfPossibleAlleles(&Population);
        printf("p(%d): %.2f%%\n", PopulationGeneration, 100 * PercentPossibleAlleles);
        MutatePopulation(&Population);
    }
    
    return 1;
}
