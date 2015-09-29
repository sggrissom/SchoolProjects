/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Steven Grissom $
   ======================================================================== */

#define A 1
#define B 0
#define C 0
#define D 0

#define DataPointCount 1000
#define Thresh 0.0001

#include <time.h>
#include <iostream>
#include <fstream>

#include "slib.h"
#include "nn.h"

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

internal r32
TestFunction2XOR(u32 x, u32 y)
{
    r32 Result = (r32)(x ^ y);

    return Result;
}

s32 main()
{
#if INTERNAL
    srand(0);
#else
    srand((u32)time(0));
#endif
    
#if A
    const char *filename = "../data/nn1a.csv";
    
    r32 data[DataPointCount][2];
    r32 TestData[DataPointCount][2];
    
    for(u32 i = 0;
        i < ArrayCount(data);
        ++i)
    {
        data[i][0] = RandomBetween(-1,7);
        data[i][1] = FunctionProblemOne(data[i][0]);
        
        TestData[i][0] = RandomBetween(-1,7);
        TestData[i][1] = FunctionProblemOne(data[i][0]);
    }

    u32 numLayers = 3;
    u32 lSz[3] = {1,25,1};
#endif
#if B
    const char *filename = "../data/nn2a.csv";

    r32 data[DataPointCount][3];
    r32 TestData[DataPointCount][3];
    
    for(u32 i = 0;
        i < ArrayCount(data);
        ++i)
    {
        data[i][0] = RandomBetween(-3,3);
        data[i][1] = RandomBetween(-3,3);
        data[i][2] = FunctionProblemTwo(data[i][0], data[i][1]);
        
        TestData[i][0] = RandomBetween(-3,3);
        TestData[i][1] = RandomBetween(-3,3);
        TestData[i][2] = FunctionProblemTwo(data[i][0], data[i][1]);
    }

    u32 numLayers = 4;
    u32 lSz[4] = {2,25,10,1};
#endif
#if C
    const char *filename = "../data/nn1b.csv";
    
    r32 data[DataPointCount][2];
    r32 TestData[DataPointCount][2];
    
    for(u32 i = 0;
        i < ArrayCount(data);
        ++i)
    {
        data[i][0] = RandomBetween(-1,7);
        data[i][1] = FunctionProblemOneWithNoise(data[i][0]);
        
        TestData[i][0] = RandomBetween(-1,7);
        TestData[i][1] = FunctionProblemOneWithNoise(data[i][0]);
    }

    u32 numLayers = 3;
    u32 lSz[3] = {1,25,1};
#endif
#if D
    const char *filename = "../data/nn2b.csv";
        
    r32 data[DataPointCount][3];
    r32 TestData[DataPointCount][3];
    
    for(u32 i = 0;
        i < ArrayCount(data);
        ++i)
    {
        data[i][0] = RandomBetween(-3,3);
        data[i][1] = RandomBetween(-3,3);
        data[i][2] = FunctionProblemTwoWithNoise(data[i][0], data[i][1]);
        
        TestData[i][0] = RandomBetween(-5,5);
        TestData[i][1] = RandomBetween(-5,5);
        TestData[i][2] = FunctionProblemTwoWithNoise(data[i][0], data[i][1]);
    }

    u32 numLayers = 4;
    u32 lSz[4] = {2,25,10,1};
#endif

    Assert(numLayers == ArrayCount(lSz));
	
	r32 beta = 0.3f, alpha = 0.1f;
	u32 num_iter = 50000;

	CBackProp *bp = new CBackProp(numLayers, lSz, beta, alpha);

    u32 DataPointSize = lSz[0] + lSz[numLayers - 1];

    for (u32 IterationIndex = 0;
         IterationIndex < num_iter;
         ++IterationIndex)
	{
        r32 MeanSquareError = 0;
	
        for(u32 DataPointIndex = 0;
            DataPointIndex < DataPointCount;
            ++DataPointIndex)
        {
            r32 *target = &data[DataPointIndex][lSz[0]];
            bp->bpgt(data[DataPointIndex], target);
            MeanSquareError += bp->mse(target);
        }
        
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
    std::ofstream ofs;
    ofs.open (filename, std::ofstream::out);

	for (u32 i = 0 ; i < DataPointCount ; i++ )
	{
        r32 *DataPoint = data[i];
		bp->ffwd(DataPoint);
        r32 output = DataPoint[lSz[0]];
        r32 prediction = bp->Out(0);
        r32 unNormalized = (prediction * 10) - 5;

        for(u32 InputIndex = 0;
            InputIndex < lSz[0];
            ++InputIndex)
        {
            ofs << DataPoint[InputIndex] << ',';
        }
        ofs << unNormalized << std::endl;
	}
#endif

    ofs.close();
    
    return 1;
}
