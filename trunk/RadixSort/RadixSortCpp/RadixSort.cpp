#include <iostream>
#include "stdint.h"

using namespace std;

inline void swap_pointer( void ** a, void ** b )
{
	void * temp;
	
	temp = *a;

	*a = *b;

	*b = temp;
}

void radix_sort( float sarray[], size_t count )
{
	int zeroes = 0;

	float* temp_array = new float [count];
	float * warray = temp_array;

	for ( uint32_t radix = 1; radix; radix<<=1 )
	{
		uint32_t * iarray = reinterpret_cast< uint32_t* > ( sarray );

		size_t count0 = 0;
		size_t count1 = 0;

		zeroes = 0;

		for ( size_t j = 0; j < count; j++ )
		{
			zeroes += !(iarray[j]&radix);
		}

		count1 = zeroes;

		for ( size_t j = 0; j < count; j++ )
		{
			if( iarray[j]&radix )
			{
				warray[count1] = sarray[j];
				++count1;
			}
			else
			{
				warray[count0] = sarray[j];
				++count0;
			}
		}

		swap_pointer(
		reinterpret_cast<void**>(&warray), 
		reinterpret_cast<void**>(&sarray) );
	}

	delete[] temp_array;
}

int main()
{
	float test[] = { 10.0F, 6.0F, 12.56F, 0.567F, 99.30F, 0.0F };
	radix_sort(test, 6);

	for ( int i = 0; i < 6; i++ )
	{
		cout << test[i] << endl;
	}
	return 0;
}

/************************************************************************/
/* 
1 thread:
1101 
0011 
0110 
1010 
0101 

2 thread:
1101 
0010 
0111 
1110 
0000

*/
/************************************************************************/