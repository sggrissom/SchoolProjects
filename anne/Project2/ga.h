#if !defined(GA_H)

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

#define LINE_BREAK printf("--------\n");

internal u32
LotteryWinner(u32 LotteryEntrantsCount)
{
    u32 LotteryTicketCount = LotteryEntrantsCount * (LotteryEntrantsCount + 1) / 2;

    u32 ChosenTicket = (u32)(RandomBetween(0, (r32)LotteryTicketCount) + 0.5f);
    ChosenTicket = (ChosenTicket >= LotteryTicketCount) ? LotteryTicketCount-1 : ChosenTicket;

    u32 WinnerIndex = 0;
    u32 TicketIndex = ChosenTicket;

    u32 TicketCutoff = PopulationSize;
    while(TicketIndex >= TicketCutoff)
    {
        ++WinnerIndex;
        TicketIndex -= TicketCutoff--;
    }

    Assert(WinnerIndex >= 0 && WinnerIndex < PopulationSize);
    
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
Quicksort(individual_fitness *Array, s32 First, s32 Last)
{
    s32 Pivot, I, J;
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
            while(J > Pivot && Array[J].FitnessScore <= Array[Pivot].FitnessScore)
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
            r32 Offset = RandomBetween(-1,1);
            Offset = (r32)pow(Offset, 5);
            Offset *= MutationMagnitude;
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
        individual Individual = Population->Individuals[IndividualIndex];
         PopulationFitnessRanked[IndividualIndex].FitnessScore =
             FitnessScore(Individual.Chromosome[0], Individual.Chromosome[1]);
    }

    Quicksort(PopulationFitnessRanked, 0, PopulationSize-1);

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

#define GA_H
#endif
