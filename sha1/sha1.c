/*
SHA-1 in C
By Steve Reid <steve@edmweb.com>
100% Public Domain

Test Vectors (from FIPS PUB 180-1)
"abc"
  A9993E36 4706816A BA3E2571 7850C26C 9CD0D89D
"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"
  84983E44 1C3BD26E BAAE4AA1 F95129E5 E54670F1
A million repetitions of "a"
  34AA973C D4C4DAA4 F61EEB2B DBAD2731 6534016F
*/

//#define LITTLE_ENDIAN /* This should be #define'd already, if true. */
/* #define SHA1HANDSOFF * Copies data before messing with it. */
#ifndef MLIB
#define SHA1HANDSOFF

#include <stdio.h>
#include <string.h>

/* for uint32_t */
#include <stdint.h>
#endif

#include "sha1.h"


#define rol(value, bits) (((value) << (bits)) | ((value) >> (32 - (bits))))
//#define rol(v,b) ({ uint32_t i = v; ROL(b,i); i; })

/* blk0() and blk() perform the initial expand. */
/* I got the idea of expanding during the round function from SSLeay */
#if BYTE_ORDER == LITTLE_ENDIAN

//#define blk0(i) (block->l[i] = (rol(block->l[i],24)&0xFF00FF00) \
    |(rol(block->l[i],8)&0x00FF00FF))

#define blk0(i) ({ BSWAP(block->l[i]); block->l[i]; })

//#define blk0(i) (block->l[i] = rol((rol(block->l[i],16)&0x00FF00FF) \
    |(block->l[i]&0xFF00FF00),8))
//#define blk0(i) (block->l[i] = block->l[i]

#elif BYTE_ORDER == BIG_ENDIAN
#define blk0(i) block->l[i]
#else
#error "Endianness not defined!"
#endif

#define blk(i) (block->l[i&15] = rol(block->l[(i+13)&15]^block->l[(i+8)&15] \
    ^block->l[(i+2)&15]^block->l[i&15],1))

//#define blk(i) (rol(block->l[i&15] ^= block->l[(i+13)&15]^block->l[(i+8)&15] \
    ^block->l[(i+2)&15] ,1) )


#ifdef R2
#undef R2
#endif

#ifdef R4
#undef R4
#endif


/* (R0+R1), R2, R3, R4 are the different operations used in SHA1 */

//#define R1(v,w,x,y,z,i) z+=((w&(x^y))^y)      +blk(i)+0x5A827999+rol(v,5);w=rol(w,30);
//#define R3(v,w,x,y,z,i) z+=(((w|x)&y)|(w&x))  +blk(i)+0x8F1BBCDC+rol(v,5);w=rol(w,30);

//#define R2(v,w,x,y,z,i) z+=(w^x^y)            +blk(i)+0x6ED9EBA1+rol(v,5);w=rol(w,30);
//#define R4(v,w,x,y,z,i) z+=(w^x^y)            +blk(i)+0xCA62C1D6+rol(v,5);w=rol(w,30);

#define R0(v,w,x,y,z,i) z+=((w&(x^y))^y)     +blk0(i)+0x5A827999+rol(v,5);arol();

#define R1(v,w,x,y,z,i) z+=((w&(x^y))^y)      +blkf(0x5A827999);//arol();

#define R3(v,w,x,y,z,i) z+=(((w|x)&y)|(w&x))  +blkf(0x8F1BBCDC);//arol()

#define R2(v,w,x,y,z,i) z+=(w^x^y)            +blkf(0x6ED9EBA1);//arol();
#define R4(v,w,x,y,z,i) z+=(w^x^y)            +blkf(0xCA62C1D6);//arol();



/* Hash a single 512-bit block. This is the core of the algorithm. */

void SHA1Transform(
    uint32_t state[5],
    const unsigned char buffer[64]
)
{

    typedef union
    {
        unsigned char c[64];
        uint32_t l[16];
    } CHAR64LONG16;

#ifdef SHA1HANDSOFF
    CHAR64LONG16 block[1];      /* use array to appear as a pointer */

    memcpy(block, buffer, 64);
#else
    /* The following had better never be used because it causes the
     * pointer-to-const buffer to be cast into a pointer to non-const.
     * And the result is written through.  I threw a "const" in, hoping
     * this will cause a diagnostic.
     */ /* I comment that out. That's . well. too much fun. misc */
//    CHAR64LONG16 *block = (CHAR64LONG16 *) buffer;
    CHAR64LONG16 block[1];      /* use array to appear as a pointer */
    memcpy(block, buffer, 64);
	
#endif
#if 1
	 uint32_t ar[6];
#else
	 union {
	 uint32_t ar[5];
	 ulong al[3];
	 char ac[24];
	 } u;
#define ar u.ar
#endif

	 //memcpy(ar,state,5);
	 for ( int i = 0; i<5; i++ )
		 ar[i] = state[4-i];
#define a ar[4]
#define b ar[3]
#define c ar[2]
#define d ar[1]
#define e ar[0]


	 uint i = 0;


#if 0
	 for ( ; i<16; i++ ){
	 }

	 for ( ; i<20; i++ ){
		 R1(ar[(i+4)%5],ar[(i+3)%5],ar[(i+2)%5],ar[(i+1)%5],ar[i%5],i);
	 }
	 
    for ( ; i<40; i++ ){
		 r24f(0x6ed9eba1);
	 }

	 for ( ; i<60; i++ ){
		 r3f();
	 }

	 for ( ; i<80; i++ ){
		 r24f(0xca62c1d6);
	 }

	 // below
const uint cic[] = {	
0x5A827999,
0x6ED9EBA1,
0x8F1BBCDC,
0xCA62C1D6 };

	 uint blkf(){
			return( blk(i) ); //+rol(A4,5) );
	 }




#else
#if 0
#define A0 ar[i%5]
#define A1 ar[(i+1)%5]
#define A2 ar[(i+2)%5]
#define A3 ar[(i+3)%5]
#define A4 ar[(i+4)%5]
#else
#define A0 ar[0]
#define A1 ar[1]
#define A2 ar[2]
#define A3 ar[3]
#define A4 ar[4]
#endif

	 void arol(){
				A3=rol(A3,30);
				//ROL(30,A3);
	 }

	 void r01f(){
		 A0 += ( (A3 & (A2 ^ A1)) ^ A1 ) + 0x5A827999;
	 }

	void r3f(){
		A0 += (( (A3 | A2) & A1) | (A3 & (A2)) ) 
			+ 0x8f1bbcdc;
	}

	void r24f(const uint ic){
	//void r24f(){
			A0 += ( A3 ^ A2 ^ A1 ) 
				+ ic;
	}

	for ( ; i<80; i++ ){
		if ( i<20 ){
			r01f();
			if ( i<16){
				BSWAP(block->l[i]);
				A0 += block->l[i];// += blk0(i); //+ rol(A4,5);
				goto L;
			}
		} else {
			if ( i<40 )
				//r24f();
				r24f(0x6ed9eba1);
			else 
				if ( i < 60 ){
					r3f();	
				} else
					//r24f();
					r24f(0xca62c1d6);
		}
		A0 += blk(i);
L:
		A0 += rol(A4,5);// + cic[i/20];
		arol();

		ar[5] = ar[0];
		for ( int i2=0; i2<5; i2++ )
			ar[i2] = ar[i2+1];
	}

#endif

	 for ( int i = 0; i<5; i++ )
		 state[i] += ar[4-i];

#undef a
#undef b
#undef c
#undef d
#undef e

#ifdef SHA1HANDSOFF
    memset(block, '\0', sizeof(block));
#endif
}


/* SHA1Init - Initialize new context */

void SHA1Init(
    SHA1_CTX * context
)
{
    /* SHA1 initialization constants */
    context->state[0] = 0x67452301;
    context->state[1] = 0xEFCDAB89;
    context->state[2] = 0x98BADCFE;
    context->state[3] = 0x10325476;
    context->state[4] = 0xC3D2E1F0;
    context->count[0] = context->count[1] = 0;
}


/* Run your data through this. */

void SHA1Update(
    SHA1_CTX * context,
    const unsigned char *data,
    uint32_t len
)
{
    uint32_t i;

    uint32_t j;

    j = context->count[0];
    if ((context->count[0] += len << 3) < j)
        context->count[1]++;
    context->count[1] += (len >> 29);
	 // the same? misc
	 //context->countl += len<<3 ;
    j = (j >> 3) & 63;
    if ((j + len) > 63)
    {
        memcpy(&context->buffer[j], data, (i = 64 - j));
        SHA1Transform(context->state, context->buffer);
        for (; i + 63 < len; i += 64)
        {
            SHA1Transform(context->state, &data[i]);
        }
        j = 0;
    }
    else
        i = 0;
    memcpy(&context->buffer[j], &data[i], len - i);
}


/* Add padding and return the message digest. */

void SHA1Final(
    unsigned char digest[20],
    SHA1_CTX * context
)
{
    unsigned i;

    unsigned char finalcount[8];

    unsigned char c;

#if 0    /* untested "improvement" by DHR */
    /* Convert context->count to a sequence of bytes
     * in finalcount.  Second element first, but
     * big-endian order within element.
     * But we do it all backwards.
     */
    unsigned char *fcp = &finalcount[8];

    for (i = 0; i < 2; i++)
    {
        uint32_t t = context->count[i];

        int j;

        for (j = 0; j < 4; t >>= 8, j++)
            *--fcp = (unsigned char) t}
#else
    for (i = 0; i < 8; i++)
    {
        finalcount[i] = (unsigned char) ((context->count[(i >= 4 ? 0 : 1)] >> ((3 - (i & 3)) * 8)) & 255);      /* Endian independent */
    }
#endif
    c = 0200;
    SHA1Update(context, &c, 1);
    while ((context->count[0] & 504) != 448)
    {
        c = 0000;
        SHA1Update(context, &c, 1);
    }
    SHA1Update(context, finalcount, 8); /* Should cause a SHA1Transform() */
    for (i = 0; i < 20; i++)
    {
        digest[i] = (unsigned char)
            ((context->state[i >> 2] >> ((3 - (i & 3)) * 8)) & 255);
    }
    /* Wipe variables */
    //memset(context, '\0', sizeof(*context));
    bzero(context, sizeof(*context));
    //memset(&finalcount, '\0', sizeof(finalcount));
    bzero(&finalcount, sizeof(finalcount));
}

void SHA1(
    char *hash_out,
    const char *str,
    int len)
{
    SHA1_CTX ctx;
    unsigned int ii;

    SHA1Init(&ctx);
    for (ii=0; ii<len; ii+=1)
        SHA1Update(&ctx, (const unsigned char*)str + ii, 1);
    SHA1Final((unsigned char *)hash_out, &ctx);
}

