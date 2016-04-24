#include "FastFourierTransform.h"

/************************************************************************/
/* CFastFourierTransform                                                */
/************************************************************************/
CFastFourierTransform::CFastFourierTransform(int pSampleSize)
{
	xre = NULL;
	xim = NULL;
	mag = NULL;
	fftSin = NULL;
	fftCos = NULL;
	fftBr = NULL;

	ss = pSampleSize;
	ss2 = ss >> 1;
	nu = (int) (log((float)ss) / log((float)2));
	nu1 = nu - 1;

	xre = new float[ss]; // real part
	xim = new float[ss]; // image part
	mag = new float[ss2];

	PrepareFFTTables();
}

CFastFourierTransform::~CFastFourierTransform(void)
{
	if(xre != NULL)
		delete [] xre;

	if(xim != NULL)
		delete [] xim;

	if(mag != NULL)
		delete [] mag;

	if(fftSin != NULL)
		delete [] fftSin;

	if(fftCos != NULL)
		delete [] fftCos;

	if(fftBr != NULL)
		delete [] fftBr;

	xre = NULL;
	xim = NULL;
	mag = NULL;
	fftSin = NULL;
	fftCos = NULL;
	fftBr = NULL;
}

void CFastFourierTransform::PrepareFFTTables()
{
	int n2 = ss2;
	int nu1 = nu - 1;

	fftSin = new float[nu * n2];
	fftCos = new float[nu * n2];

	int k = 0;
	int x = 0;
	for (int l = 1; l <= nu; l++) {
		while (k < ss) {
			for (int i = 1; i <= n2; i++) {
				float p = (float)BitRev(k >> nu1, nu);
				float arg = (PI_2 * p) / (float) ss;
				fftSin[x] = (float) sin(arg);
				fftCos[x] = (float) cos(arg);
				k++;
				x++;
			}

			k += n2;
		}

		k = 0;
		nu1--;
		n2 >>= 1;
	}

	fftBr = new int[ss];
	for (k = 0; k < ss; k++)
		fftBr[k] = BitRev(k, nu);
}

int CFastFourierTransform::BitRev(int j, int nu) {
	int j1 = j;
	int k = 0;
	for (int i = 1; i <= nu; i++) {
		int j2 = j1 >> 1;
		k = ((k << 1) + j1) - (j2 << 1);
		j1 = j2;
	}

	return k;
}

float* CFastFourierTransform::Calculate(float* pSample, size_t pSampleSize) {
	int n2 = ss2;
	int nu1 = nu - 1;
	int wAps = pSampleSize / ss;
	size_t a = 0;

	for (size_t b = 0; a < pSampleSize; b++) {
		xre[b] = pSample[a];
		xim[b] = 0.0F;
		a += wAps;
	}

	int x = 0;
	for (int l = 1; l <= nu; l++) {
		for (int k = 0; k < ss; k += n2) {
			for (int i = 1; i <= n2; i++) {
				float c = fftCos[x];
				float s = fftSin[x];
				int kn2 = k + n2;
				float tr = xre[kn2] * c + xim[kn2] * s;
				float ti = xim[kn2] * c - xre[kn2] * s;
				xre[kn2] = xre[k] - tr;
				xim[kn2] = xim[k] - ti;
				xre[k] += tr;
				xim[k] += ti;
				k++;
				x++;
			}
		}

		nu1--;
		n2 >>= 1;
	}

	for (int k = 0; k < ss; k++) {
		int r = fftBr[k];
		if (r > k) {
			float tr = xre[k];
			float ti = xim[k];
			xre[k] = xre[r];
			xim[k] = xim[r];
			xre[r] = tr;
			xim[r] = ti;
		}
	}

	mag[0] = (float) sqrt(xre[0] * xre[0] + xim[0] * xim[0]) / (float) ss;
	for (int i = 1; i < ss2; i++)
		mag[i] = (2.0F * (float) sqrt(xre[i] * xre[i] + xim[i] * xim[i])) / (float) ss;

	return mag;
}
