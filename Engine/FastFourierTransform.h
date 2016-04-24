#ifndef INCLUDE_FASTTOURIERTRANSFORM
#define INCLUDE_FASTTOURIERTRANSFORM

#include <math.h>
#include <stdlib.h>

/************************************************************************/
/* CFastFourierTransform                                                */
/************************************************************************/

#define PI_2 6.283185F
#define PI   3.1415925F

class CFastFourierTransform
{
public:
	CFastFourierTransform(int pSampleSize);
	~CFastFourierTransform(void);

public:
	float* Calculate(float* pSample, size_t pSampleSize);

private:
	float* xre;
	float* xim;
	float* mag;
	float* fftSin;
	float* fftCos;
	int* fftBr;
	int ss;
	int ss2;
	int nu;
	int nu1;

	int BitRev(int j, int nu);
	void PrepareFFTTables();
};

#endif
