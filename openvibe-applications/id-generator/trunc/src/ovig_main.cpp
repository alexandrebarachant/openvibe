#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int main(int argc, char** argv)
{
	srand((unsigned int)time(NULL));
	for(int i=0; i<15; i++)
	{
		unsigned long int l_ui32Vale1=(rand()&0x00ffffff);
		unsigned long int l_ui32Vale2=(rand()&0xffffffff);
		printf("#define OV_ClassId_                                        OpenViBE::CIdentifier(0x%08X, 0x%08X)\n", (int)l_ui32Vale1, (int)l_ui32Vale2);
	}

	return 0;
}
