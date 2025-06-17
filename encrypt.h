#include <stdint.h>


struct State {
    uint32_t words[16];
};

void State_Init(State* matrix, uint32_t key[8], uint32_t counter, uint32_t nonce[3]);

void quarter_round(uint32_t words[16], int x, int y, int z, int w);

void ChaCha20_operate(State* matrix);


/*
The first four words (0-3) are constants: 0x61707865, 0x3320646e, 0x79622d32, 0x6b206574.

The next eight words (4-11) are taken from the 256-bit key by 
reading the bytes in little-endian order, in 4-byte chunks.

Word 12 is a block counter. Since each block is 64-byte, a 32-bit
word is enough for 256 gigabytes of data.

Words 13-15 are a nonce, which should not be repeated for the same
key. The 13th word is the first 32 bits of the input nonce taken
as a little-endian integer, while the 15th word is the last 32
bits

They are all in little endian order
*/