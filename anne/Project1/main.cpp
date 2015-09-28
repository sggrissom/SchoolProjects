/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Steven Grissom $
   ======================================================================== */

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
    srand(time(0));
#endif
    
#if 1
    const char *filename = "nn1a.csv";
    
    r32 data[100][2];
    r32 TestData[100][2];
    
    for(u32 i = 0;
        i < ArrayCount(data);
        ++i)
    {
        data[i][0] = RandomBetween(-2,10);
        data[i][1] = FunctionProblemOne(data[i][0]);
        
        TestData[i][0] = RandomBetween(-2,10);
        TestData[i][1] = FunctionProblemOne(data[i][0]);
    }

    u32 numLayers = 5;
    u32 lSz[5] = {1,5,5,5,1};
#endif
#if 0
    const char *filename = "nn2a.csv";

    r32 data[300][3];
    r32 TestData[300][3];
    
    for(u32 i = 0;
        i < ArrayCount(data);
        ++i)
    {
        data[i][0] = RandomBetween(-5,5);
        data[i][1] = RandomBetween(-5,5);
        data[i][2] = FunctionProblemTwo(data[i][0], data[i][1]);
        
        TestData[i][0] = RandomBetween(-5,5);
        TestData[i][1] = RandomBetween(-5,5);
        TestData[i][2] = FunctionProblemTwo(data[i][0], data[i][1]);
    }

    u32 numLayers = 5;
    u32 lSz[5] = {2,5,5,5,1};
#endif
#if 0
    const char *filename = "nn1b.csv";
    
    r32 data[100][2];
    r32 TestData[100][2];
    
    for(u32 i = 0;
        i < ArrayCount(data);
        ++i)
    {
        data[i][0] = RandomBetween(-2,10);
        data[i][1] = FunctionProblemOneWithNoise(data[i][0]);
        
        TestData[i][0] = RandomBetween(-2,10);
        TestData[i][1] = FunctionProblemOneWithNoise(data[i][0]);
    }

    u32 numLayers = 4;
    u32 lSz[4] = {1,3,3,1};
#endif
#if 0
    const char *filename = "nn2b.csv";
        
    r32 data[300][3];
    r32 TestData[300][3];
    
    for(u32 i = 0;
        i < ArrayCount(data);
        ++i)
    {
        data[i][0] = RandomBetween(-5,5);
        data[i][1] = RandomBetween(-5,5);
        data[i][2] = FunctionProblemTwoWithNoise(data[i][0], data[i][1]);
        
        TestData[i][0] = RandomBetween(-5,5);
        TestData[i][1] = RandomBetween(-5,5);
        TestData[i][2] = FunctionProblemTwoWithNoise(data[i][0], data[i][1]);
    }

    u32 numLayers = 4;
    u32 lSz[4] = {2,3,3,1};
#endif

    Assert(numLayers == ArrayCount(lSz));
	
	r32 beta = 0.3f, alpha = 0.1f, Thresh =  0.0001f;
	u32 num_iter = 5000000;

	CBackProp *bp = new CBackProp(numLayers, lSz, beta, alpha);

    u32 DataPointSize = lSz[0] + lSz[numLayers - 1];
    u32 DataPointCount = ArrayCount(data);
	
	for (u32 i=0; i<num_iter ; i++)
	{
		bp->bpgt(data[i%DataPointCount], &data[i%DataPointCount][lSz[0]]);
	}

#if 1
    std::ofstream ofs;
    ofs.open (filename, std::ofstream::out);

    ofs << "neural network\n\n";

    r32 ErrorSum = 0;

	for (u32 i = 0 ; i < DataPointCount ; i++ )
	{
        r32 *DataPoint = TestData[i];
		bp->ffwd(DataPoint);
        r32 output = DataPoint[lSz[0]];
        r32 prediction = bp->Out(0);
        r32 unNormalized = (prediction * 10) - 5;

        r32 ErrorDelta = output - prediction;
        ErrorSum += ErrorDelta;
        
        r32 epsilon = 0.02f;
        if(ErrorDelta > epsilon || ErrorDelta < -epsilon)
        {
            
        } else
        {
        }
        for(u32 InputIndex = 0;
            InputIndex < lSz[0];
            ++InputIndex)
        {
            ofs << DataPoint[InputIndex] << ',';
        }
        ofs << unNormalized << std::endl;
	}
#endif

    r32 ErrorMean = ErrorSum / DataPointCount;

    printf("error: %f\n", ErrorMean);
    ofs << "\n\n" << ErrorMean << "\n";

    ofs.close();
    
    return 1;
}
