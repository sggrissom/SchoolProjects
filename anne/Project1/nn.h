#if !defined(NN_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Steven Grissom $
   ======================================================================== */

#include <math.h>

struct CBackProp
{
	r32 **out;
	r32 **delta;
	r32 ***weight;
	u32 numl;
	u32 *lsize;
	r32 beta;
	r32 alpha;
	r32 ***prevDwt;
	r32 sigmoid(r32 in);

	~CBackProp();
	CBackProp(u32 nl,u32 *sz,r32 b,r32 a);
	void bpgt(r32 *in,r32 *tgt);
	void ffwd(r32 *in);
	r32 mse(r32 *tgt) const;	
	r32 Out(u32 i) const;
};



CBackProp::CBackProp(u32 nl,u32 *sz,r32 b,r32 a):beta(b),alpha(a)
{
	numl=nl;
	lsize=new u32[numl];

	for(u32 i=0;i<numl;i++){
		lsize[i]=sz[i];
	}

	out = new r32*[numl];

	for(u32 i=0;i<numl;i++){
		out[i]=new r32[lsize[i]];
	}

	
	delta = new r32*[numl];

	for(u32 i=1;i<numl;i++){
		delta[i]=new r32[lsize[i]];
	}
	
	weight = new r32**[numl];

	for(u32 i=1;i<numl;i++){
		weight[i]=new r32*[lsize[i]];
	}
	for(u32 i=1;i<numl;i++){
		for(u32 j=0;j<lsize[i];j++){
			weight[i][j]=new r32[lsize[i-1]+1];
		}
	}

	
	prevDwt = new r32**[numl];

	for(u32 i=1;i<numl;i++){
		prevDwt[i]=new r32*[lsize[i]];

	}
	for(u32 i=1;i<numl;i++){
		for(u32 j=0;j<lsize[i];j++){
			prevDwt[i][j]=new r32[lsize[i-1]+1];
		}
	}

	srand((unsigned)(time(NULL)));
	for(u32 i=1;i<numl;i++)
		for(u32 j=0;j<lsize[i];j++)
			for(u32 k=0;k<lsize[i-1]+1;k++)
				weight[i][j][k]=(r32)(rand())/(RAND_MAX/2) - 1;

	
	for(u32 i=1;i<numl;i++)
		for(u32 j=0;j<lsize[i];j++)
			for(u32 k=0;k<lsize[i-1]+1;k++)
				prevDwt[i][j][k]=(r32)0.0;
}



CBackProp::~CBackProp()
{
	for(u32 i=0;i<numl;i++)
		delete[] out[i];
	delete[] out;
	
	for(u32 i=1;i<numl;i++)
		delete[] delta[i];
	delete[] delta;

	for(u32 i=1;i<numl;i++)
		for(u32 j=0;j<lsize[i];j++)
			delete[] weight[i][j];
	for(u32 i=1;i<numl;i++)
		delete[] weight[i];
	delete[] weight;

	
	for(u32 i=1;i<numl;i++)
		for(u32 j=0;j<lsize[i];j++)
			delete[] prevDwt[i][j];
	for(u32 i=1;i<numl;i++)
		delete[] prevDwt[i];
	delete[] prevDwt;

	
	delete[] lsize;
}


r32 CBackProp::sigmoid(r32 in)
{
    return (r32)(1/(1+exp(-in)));
}

r32 CBackProp::mse(r32 *tgt) const
{
	r32 mse=0;
	for(u32 i = 0;
        i < lsize[numl-1];
        ++i)
    {
		mse+=(tgt[i]-out[numl-1][i])*(tgt[i]-out[numl-1][i]);
	}
	return mse/2;
}

r32 CBackProp::Out(u32 i) const
{
	return out[numl-1][i];
}

void CBackProp::ffwd(r32 *in)
{
	for(u32 i = 0;
        i < lsize[0];
        ++i)
    {
		out[0][i]=in[i];
    }
	
	for(u32 i = 1;
        i < numl;
        ++i)
    {				
		for(u32 j = 0;
            j < lsize[i];
            ++j)
        {
			r32 sum = 0.0;
			for(u32 k = 0;
                k < lsize[i-1];
                ++k)
            {
				sum+= out[i-1][k]*weight[i][j][k];	
			}
			sum+=weight[i][j][lsize[i-1]];		
			out[i][j]=sigmoid(sum);				
		}
	}
}

void CBackProp::bpgt(r32 *in,r32 *tgt)
{
	r32 sum;
	
	ffwd(in);
	
	for(u32 i=0;
        i < lsize[numl-1];
        ++i)
    {
		delta[numl-1][i]=out[numl-1][i]*
		(1-out[numl-1][i])*(tgt[i]-out[numl-1][i]);
	}
	
	for(u32 i = numl-2;
        i > 0;
        --i)
    {
		for(u32 j = 0;
            j < lsize[i];
            ++j)
        {
			sum=0.0;
			for(u32 k = 0;
                k < lsize[i+1];
                ++k)
            {
				sum+=delta[i+1][k]*weight[i+1][k][j];
			}
			delta[i][j]=out[i][j]*(1-out[i][j])*sum;
		}
	}
	
    for(u32 i = 1;
        i < numl;
        ++i)
    {
		for(u32 j = 0;
            j < lsize[i];
            ++j)
        {
			for(u32 k = 0;
                k < lsize[i-1];
                ++k)
            {
				weight[i][j][k]+=alpha*prevDwt[i][j][k];
			}
			weight[i][j][lsize[i-1]]+=alpha*prevDwt[i][j][lsize[i-1]];
		}
	}

	for(u32 i = 1;
        i < numl;
        ++i)
    {
		for(u32 j = 0;
            j < lsize[i];
            ++j)
        {
			for(u32 k = 0;
                k < lsize[i-1];
                ++k)
            {
				prevDwt[i][j][k]=beta*delta[i][j]*out[i-1][k];
				weight[i][j][k]+=prevDwt[i][j][k];
			}
            prevDwt[i][j][lsize[i-1]]=beta*delta[i][j];
			weight[i][j][lsize[i-1]]+=prevDwt[i][j][lsize[i-1]];
		}
	}
}

#define NN_H
#endif
