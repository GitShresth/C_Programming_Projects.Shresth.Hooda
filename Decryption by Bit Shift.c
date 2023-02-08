#include <stdio.h>
#include "cipher.h"

#ifdef MYDECRYPT_C
TODO("using Cdecrypt.c");

int
decrypt(char *iobuf, char *bookbuf, int cnt)
{
	for (int i = 0 ; i < cnt ; i++) {
		unsigned char initialChar = *(iobuf + i);    //init byte
		unsigned int final = *(bookbuf + i) ^ initialChar; //EOR
		unsigned int rightShift;	/*value of shifting bits
	       				  	  right */
		unsigned int leftShift;		/*value of shifting bits
						  left */
		rightShift = (final & 0xF0) >> 4;
		leftShift = (final & 0x0F) << 4;
		final = rightShift | leftShift;
		rightShift = (final & 0xCC) >> 2;
		leftShift = (final & 0x33) << 2;
		final = rightShift | leftShift;
		rightShift = (final & 0xAA) >> 1;
		leftShift = (final & 0x55) << 1;
		final = rightShift | leftShift;
		*(iobuf + i) = final;	
	} 
    return cnt;
}

#elif defined MYDECRYPT_S
TODO("using decrypt.S");
#else
TODO("using solution decrypt");
#endif
