#include "Linx/TestConfig.h"
#ifdef TEST_FT
#include "FT.h"
#include <vector>

using namespace std;
using namespace Linx;

int main()
{
	vector<complex<double>>c1(32); auto pc1 = c1.data();
	double c2[32], c3[32]{ 0 };
	complex<double> out[32];
	for (int i=0;i<32;i++)
	{
		c1[i] = cos(2 * PI * i * 0.1);
		c2[i] = cos(2 * PI * i * 0.1);
	}
	FFT<32>(begin(c1), out);

	double d[32];
	SpectrumToAmp(out, out + 32, d);
	SpectrumToPhase(out, out + 32, d);

	IFFT<32>(begin(out), c1);

	FFT<32>(c2, c3, out);
	SpectrumToAmp(out, out + 32, d);

	getchar();
}

#endif