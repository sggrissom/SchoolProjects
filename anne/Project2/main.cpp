/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Steven Grissom $
   ======================================================================== */

//A and B are function 1 and 2 without noise, respectively
//C and D are with noise
#define A 0
#define B 0
#define C 0
#define D 1

#define DataPointCount 1000
#define Thresh 0.0005

#include <time.h>
#include <iostream>
#include <fstream>

#include "slib.h"
#include "nn.h"

//macro to get a random float between two values
#define RandomBetween(min,max) (min + (r32)(rand() / ((r32)(RAND_MAX/(max - min)))))

internal r32
FunctionProblemOne(r32 x)
{
    r32 Result = 0;

    if(x >= -1 && x <= 7)
    {
        r32 atanx = atan(x);
        r32 sinx = sin(x);

        Result = atanx + sinx;
    }

    return (Result+5)/10;
}

internal r32
FunctionProblemOneWithNoise(r32 x)
{
    r32 Result = 0;

    if(x >= -1 && x <= 7)
    {
        r32 atanx = atan(x);
        r32 sinx = sin(x);
        r32 noise = RandomBetween(-0.5f, 0.5f);

        Result = atanx + sinx + noise;
    }

    return (Result+5)/10;
}

internal r32
FunctionProblemTwo(r32 x, r32 y)
{
    r32 Result = 0;

    if(x >= -3 && y <= 3)
    {
        r32 atanx = atan(x);
        r32 siny = sin(y);

        Result = atanx + siny;
    }

    return (Result+5)/10;
}

internal r32
FunctionProblemTwoWithNoise(r32 x, r32 y)
{
    r32 Result = 0;

    if(x >= -3 && y <= 3)
    {
        r32 atanx = atan(x);
        r32 siny = sin(y);
        r32 noise = RandomBetween(-0.5f, 0.5f);

        Result = atanx + siny + noise;
    }

    return (Result+5)/10;
}

s32 main()
{
    srand((u32)time(0));

    //setup data depending on which function will be run
    //TrainData will contain random values for teaching the network
    //TestData is for testing the network after it's trained
#if A
    const char *filename = "nn1a.csv";
    
    r32 TrainData[DataPointCount][2];
    r32 TestData[DataPointCount][2];
    
    for(u32 i = 0;
        i < ArrayCount(TrainData);
        ++i)
    {
        TrainData[i][0] = RandomBetween(-1,7);
        TrainData[i][1] = FunctionProblemOne(TrainData[i][0]);
        
        TestData[i][0] = RandomBetween(-1,7);
        TestData[i][1] = FunctionProblemOne(TrainData[i][0]);
    }

    u32 numLayers = 3;
    u32 lSz[3] = {1,25,1};
#endif
#if B
    const char *filename = "nn2a.csv";

    r32 TrainData[DataPointCount][3];
    r32 TestData[DataPointCount][3];
    
    for(u32 i = 0;
        i < ArrayCount(TrainData);
        ++i)
    {
        TrainData[i][0] = RandomBetween(-3,3);
        TrainData[i][1] = RandomBetween(-3,3);
        TrainData[i][2] = FunctionProblemTwo(TrainData[i][0], TrainData[i][1]);
        
        TestData[i][0] = RandomBetween(-3,3);
        TestData[i][1] = RandomBetween(-3,3);
        TestData[i][2] = FunctionProblemTwo(TrainData[i][0], TrainData[i][1]);
    }

    u32 numLayers = 4;
    u32 lSz[4] = {2,25,10,1};
#endif
#if C
    const char *filename = "nn1b.csv";
    
    r32 TrainData[DataPointCount][2];
    r32 TestData[DataPointCount][2];
    
    for(u32 i = 0;
        i < ArrayCount(TrainData);
        ++i)
    {
        TrainData[i][0] = RandomBetween(-1,7);
        TrainData[i][1] = FunctionProblemOneWithNoise(TrainData[i][0]);
        
        TestData[i][0] = RandomBetween(-1,7);
        TestData[i][1] = FunctionProblemOneWithNoise(TrainData[i][0]);
    }

    u32 numLayers = 3;
    u32 lSz[3] = {1,25,1};
#endif
#if D
    const char *filename = "nn2b.csv";
        
    r32 TrainData[DataPointCount][3];
    r32 TestData[DataPointCount][3];
    
    for(u32 i = 0;
        i < ArrayCount(TrainData);
        ++i)
    {
        TrainData[i][0] = RandomBetween(-3,3);
        TrainData[i][1] = RandomBetween(-3,3);
        TrainData[i][2] = FunctionProblemTwoWithNoise(TrainData[i][0], TrainData[i][1]);
        
        TestData[i][0] = RandomBetween(-3,3);
        TestData[i][1] = RandomBetween(-3,3);
        TestData[i][2] = FunctionProblemTwoWithNoise(TrainData[i][0], TrainData[i][1]);
    }

    u32 numLayers = 4;
    u32 lSz[4] = {2,25,10,1};
#endif

    //crash if I missed a number change
    Assert(numLayers == ArrayCount(lSz));
	
	r32 beta = 0.3f, alpha = 0.1f;
	u32 num_iter = 5000000;

	CBackProp *bp = new CBackProp(numLayers, lSz, beta, alpha);

    u32 DataPointSize = lSz[0] + lSz[numLayers - 1];

    //iterate up to the max iterations
    for (u32 IterationIndex = 0;
         IterationIndex < num_iter;
         ++IterationIndex)
	{
        r32 MeanSquareError = 0;
	
        for(u32 DataPointIndex = 0;
            DataPointIndex < DataPointCount;
            ++DataPointIndex)
        {
            //backpropogate the training data and caluculate the mse
            r32 *target = &TrainData[DataPointIndex][lSz[0]];
            bp->bpgt(TrainData[DataPointIndex], target);
            MeanSquareError += bp->mse(target);
        }

        //if average of MSE over the set of test data is below the threshold, end training
        MeanSquareError /= DataPointCount;
        if(MeanSquareError < Thresh)
        {
            printf("network trained, MSE: %f\n", MeanSquareError);
            break;
        }
        else
        {
            printf("training... MSE: %f\n", MeanSquareError);
        }
	}

#if 1

    //create ofs to create csvs of the data
    
    const char *path_prefix = "../data/";
    char OutputFilename[80];
    strcpy_s(OutputFilename, path_prefix);
    strcat_s(OutputFilename, filename);
    char TestDataFilename[80];
    strcpy_s(TestDataFilename, path_prefix);
    strcat_s(TestDataFilename, "test");
    strcat_s(TestDataFilename, filename);

    printf("%s\n%s\n", OutputFilename, TestDataFilename);
    
    std::ofstream NetworkOutput, TestDataOutput;
    NetworkOutput.open(OutputFilename, std::ofstream::out);
    TestDataOutput.open(TestDataFilename, std::ofstream::out);

    //iterate through each test data point, feed forward, and save out the results
	for (u32 i = 0 ; i < DataPointCount ; i++ )
	{
        r32 *DataPoint = TestData[i];
		bp->ffwd(DataPoint);
        r32 output = DataPoint[lSz[0]];
        r32 prediction = bp->Out(0);
        r32 reNormalized = (prediction * 10) - 5;

        for(u32 InputIndex = 0;
            InputIndex < lSz[0];
            ++InputIndex)
        {
            NetworkOutput << DataPoint[InputIndex] << ',';
            TestDataOutput << DataPoint[InputIndex] << ',';
        }
        NetworkOutput << reNormalized << std::endl;
        TestDataOutput << ((output*10)-5) << std::endl;
	}
#endif

    NetworkOutput.close();
    
    return 1;
}
