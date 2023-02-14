#include <stdio.h>
#include "cipher.h"

#ifdef MYENCRYPT_C
TODO("using Cencrypt.c");

int
encrypt(char *iobuf, char *bookbuf, int cnt)
{
	for (int i = 0 ; i < cnt ; i++) {
		unsigned char value = *(iobuf + i);	/*init val of each byte */
		unsigned int rightShift;		/*val of bit shift right */
		unsigned int leftShift;  		/*val of bit shift left */
		
		rightShift = (value & 0xF0) >> 4;
	        leftShift = (value & 0x0F) << 4;
		value = rightShift | leftShift;
		rightShift = (value & 0xCC) >> 2;
		leftShift = (value & 0x33) << 2;
		value = rightShift | leftShift;
		rightShift = (value & 0xAA) >> 1;
		leftShift = (value & 0x55) << 1;
		value = rightShift | leftShift;
		unsigned int result = value;	 	/*flipped byte as int*/
		*(iobuf + i) = (*(bookbuf + i) ^ result);
	} 
        return cnt;
}

#elif defined MYENCRYPT_S
TODO("using encrypt.S");
#else
TODO("using solution encrypt");
#endif
