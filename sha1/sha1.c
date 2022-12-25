/*
 
original sha1 code: Steve Reid <steve@edmweb.com>
		100% Public Domain

Test Vectors (from FIPS PUB 180-1)
"abc"
  A9993E36 4706816A BA3E2571 7850C26C 9CD0D89D
"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"
  84983E44 1C3BD26E BAAE4AA1 F95129E5 E54670F1
A million repetitions of "a"
  34AA973C D4C4DAA4 F61EEB2B DBAD2731 6534016F
*/
/*
 changes 2022 misc. Got this down from 7.5kB to 1kB.
 bsd 3clause license. It's just not my fault.

 If SHA1HANDSOFF is not defined, the content of the buffer to hash is changed (!)
 In favor of codesize this now works only on little endian (amd64, intel x64) machines.
 if you'd like to have the stack variables overwritten, 
 undefine the bzero macro below.
 Albite I'd suggest, instead of trying (! this is not guaranteed to work) to erase
 the stackvariables, better hash another random buffer.

 The original try to erase the stackvariables might be unsuccessfull in each case.
 In most circumstances the compiler is smart enough to see, the value 0 is written,
 but not read anymore. so. Why write it there.. ?
 Inline assembly might be saver.
 But I'm not sure, what processor internal predictors are going to . predict.
 The code below might have the advantage to stay within the cpu,
 it's squeezed enough, to work only with registers.
 No memory access needed, besides reading the buffer to hash.

 */

//#define LITTLE_ENDIAN /* This should be #define'd already, if true. */

#define SHA1HANDSOFF  /* Copies data before messing with it. */
#define SHA1_ERASE // overwrite data at the stack



#ifndef MLIB
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#endif

//#define SHA1HANDSOFF

typedef struct{
	 //union {
	 uint32_t count[2];
	//	 ulong countl;
	 //};
    unsigned char buffer[64];
    uint32_t state[5];
} SHA1_CTX;



// Don't overwrite stack vars
#define bzero(x,z) 

#define rol(value, bits) (((value) << (bits)) | ((value) >> (32 - (bits))))
//#define rol(v,b) ({ uint32_t i = v; ROL(b,i); i; })

/* blk0() and blk() perform the initial expand. */
/* I got the idea of expanding during the round function from SSLeay */
#if BYTE_ORDER == LITTLE_ENDIAN

//#define blk0(i) (block[i] = (rol(block[i],24)&0xFF00FF00) 
//    |(rol(block[i],8)&0x00FF00FF))

//#define blk0(i) ({ BSWAP(block[i]); block[i]; })


#elif BYTE_ORDER == BIG_ENDIAN
#define blk0(i) block[i]
#else
#error "Endianness not defined!"
#endif


/* Hash a single 512-bit block. This is the core of the algorithm. */
static void SHA1Transform( uint32_t state[5], unsigned char buffer[64] ){
#ifdef SHA1HANDSOFF
    uint32_t block[16];
    //memcpy(block, buffer, 64);
	 for ( int a = 0; a < 16; a++ ){ // copy and swap byteorder
		 asm( "mov %1,%0\n bswap %0" : "=r"(*(block+a)) : "m"(*((uint*)buffer+a)) );
	 }

#else
    /* The following had better never be used because it causes the
     * pointer-to-const buffer to be cast into a pointer to non-const.
     * And the result is written through.  I threw a "const" in, hoping
     * this will cause a diagnostic.
     */ /* That's not been my idea. but spares 20 Bytes. I changed the declaration,
			  removed the const, added the warning. misc */
    uint32_t *block = (uint32_t*)buffer;
	for ( uint32_t *pi = block; pi<block+16; pi++ )
		BSWAP(*pi);
//#warning buffer will be changed (SHA1Update(buffer) -> SHA1Transform(buffer) ) 
//	 define SHA1HANDSOFF if necessary
#endif

	// repl array with vars?
#if 1
#define A0 ar[0]
#define A1 ar[1]
#define A2 ar[2]
#define A3 ar[3]
#define A4 ar[4]
#else
#define A0 ar[4]
#define A1 ar[3]
#define A2 ar[2]
#define A3 ar[1]
#define A4 ar[0]
#endif

	 uint32_t ar[5];

	 for ( int i = 0; i<5; i++ )
		 ar[i] = state[4-i];


	 void rr(uint x, const uint cx){
		 A0 += (x^A1) + cx;
	 }
	 void r01f(){
		 rr (A3 & (A2 ^ A1), 0x5A827999 );
	 }
	 void r24f(const uint ic){
		 rr( A3 ^ A2 , ic );
	 }
	 void r3f(){
		 A0 += (( (A3 | A2) & A1) | (A3 & A2) ) 
			 + 0x8f1bbcdc;
	 }


	for ( int i = 0; i<80; i++ ){

		if ( i<20 )
			r01f();
		else if ( i<40 )
			r24f(0x6ed9eba1);
		else if ( i < 60 )
			r3f();	
		else 
			r24f(0xca62c1d6);

		if ( i > 15 )
			block[i&15] = rol( block[(i+13)&15] ^ block[(i+8)&15] ^ block[(i+2)&15] ^ block[i&15] , 1 );

		A3=rol(A3,30);
		
		A0 += block[i&15] + rol(A4,5);
		
		// rotate registers
		uint32_t t = ar[0];
		for ( int a = 0; a<4; a++ )
			ar[a] = ar[a+1];
		ar[4] = t;

	}

	for ( int i = 0; i<5; i++ )
		 state[i] += ar[4-i];
}


/* SHA1Init - Initialize new context */

static void SHA1Init( SHA1_CTX * context ){
	/* SHA1 initialization constants */
	context->state[0] = 0x67452301;
	context->state[1] = 0xEFCDAB89;
	context->state[2] = 0x98BADCFE;
	context->state[3] = 0x10325476;
	context->state[4] = 0xC3D2E1F0;
	context->count[0] = context->count[1] = 0;
}


/* Run your data through this. */

static void SHA1Update( SHA1_CTX * context, unsigned char *data, uint32_t len){
	uint32_t i,j;

	j = context->count[0];
	if ((context->count[0] += len << 3) < j)
		context->count[1]++;
	context->count[1] += (len >> 29);
	j = (j >> 3) & 63;
	if ((j + len) & ~63){ //> 63)
		memcpy(&context->buffer[j], data, (i = 64 - j));
		SHA1Transform(context->state, context->buffer);
		for (; i + 63 < len; i += 64)
			SHA1Transform(context->state, &data[i]);
		j = 0;
	} else {
		i = 0;
	}
	memcpy(&context->buffer[j], &data[i], len - i);
}

// overwrite 1kB at the stack with 0
void __attribute__((noipa,noinline))erase_stack(){
	uint64_t stack[128];
	asm volatile("rep stosq" : : "D"(stack), "c"(128), "a"(0): "cc" );
}

/* Add padding and return the message digest. */

static void SHA1Final( unsigned char digest[20], SHA1_CTX * context ){
	unsigned i;
	unsigned char finalcount[8];
	unsigned char c;

	for (i = 0; i < 8; i++)
		finalcount[i] = (unsigned char) ((context->count[!(i&~3)] >> 
					((3 - (i & 3)) << 3)));    

	c = 128;
	SHA1Update(context, &c, 1);
	while ((context->count[0] & 504) != 448){
		c = 0;
		SHA1Update(context, &c, 1);
	}
	SHA1Update(context, finalcount, 8); /* Should cause a SHA1Transform() */
	for (i = 0; i < 20; i++)
		digest[i] = (unsigned char) ((context->state[i >> 2] >> ((3 - (i & 3)) << 3)) );

#ifdef SHA1_ERASE
	/* Wipe variables */
	//bzero(context, sizeof(*context));
	asm volatile( "rep stosq" : : "D"(context), "ecx"(sizeof(context)>>3) );
	//bzero(&finalcount, sizeof(finalcount));
	asm volatile( "mark:\nmovq $0,%0\n" : "+m"(*(ulong*)finalcount) ); // shoot at the stack ..
	erase_stack();
	//asm volatile( "stosq" :: "D"(&finalcount) );
#endif
}


static void SHA1( char *hash_out, const char *str, int len ){
    SHA1_CTX ctx;
    unsigned int ii;

    SHA1Init(&ctx);
    for (ii=0; ii<len; ii++)
        SHA1Update(&ctx, (unsigned char*)str + ii, 1);
    SHA1Final((unsigned char *)hash_out, &ctx);
} 

#if 0

// hmac.
static void hmac_sha1(unsigned char *key, int key_len,
    unsigned char *text, int text_len, unsigned char *hmac) {
    SHA_State context;
    unsigned char k_ipad[KEY_IOPAD_SIZE];    /* inner padding - key XORd with ipad  */
    unsigned char k_opad[KEY_IOPAD_SIZE];    /* outer padding - key XORd with opad */
    int i;

    /* start out by storing key in pads */
    memset(k_ipad, 0, sizeof(k_ipad));
    memset(k_opad, 0, sizeof(k_opad));
    memcpy(k_ipad, key, key_len);
    memcpy(k_opad, key, key_len);

    /* XOR key with ipad and opad values */
    for (i = 0; i < KEY_IOPAD_SIZE; i++) {
        k_ipad[i] ^= 0x36;
        k_opad[i] ^= 0x5c;
    }

    // perform inner SHA
    SHA_Init(&context);                    /* init context for 1st pass */
    SHA_Bytes(&context, k_ipad, KEY_IOPAD_SIZE);      /* start with inner pad */
    SHA_Bytes(&context, text, text_len); /* then text of datagram */
    SHA_Final(&context, hmac);             /* finish up 1st pass */

    // perform outer SHA
    SHA_Init(&context);                   /* init context for 2nd pass */
    SHA_Bytes(&context, k_opad, KEY_IOPAD_SIZE);     /* start with outer pad */
    SHA_Bytes(&context, hmac, SHA1_DIGEST_SIZE);     /* then results of 1st hash */
    SHA_Final(&context, hmac);          /* finish up 2nd pass */
}
#endif

