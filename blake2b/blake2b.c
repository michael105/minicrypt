/*
   BLAKE2 reference source code package - reference C implementations

   Copyright 2012, Samuel Neves <sneves@dei.uc.pt>.  You may use this under the
   terms of the CC0, the OpenSSL Licence, or the Apache Public License 2.0, at
   your option.  The terms of these licenses can be found at:

   - CC0 1.0 Universal : http://creativecommons.org/publicdomain/zero/1.0
   - OpenSSL license   : https://www.openssl.org/source/license.html
   - Apache 2.0        : http://www.apache.org/licenses/LICENSE-2.0

   More information about the BLAKE2 hash function can be found at
   https://blake2.net.
*/

//#include <stdint.h>
//#include <string.h>
//#include <stdio.h>

#include "blake2.h"
#include "blake2-impl.h"

static const uint64_t blake2b_IV[8] =
{
  0x6a09e667f3bcc908ULL, 0xbb67ae8584caa73bULL,
  0x3c6ef372fe94f82bULL, 0xa54ff53a5f1d36f1ULL,
  0x510e527fade682d1ULL, 0x9b05688c2b3e6c1fULL,
  0x1f83d9abfb41bd6bULL, 0x5be0cd19137e2179ULL
};

static const uint8_t blake2b_sigma[12][16] =
{
  {  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15 } ,
  { 14, 10,  4,  8,  9, 15, 13,  6,  1, 12,  0,  2, 11,  7,  5,  3 } ,
  { 11,  8, 12,  0,  5,  2, 15, 13, 10, 14,  3,  6,  7,  1,  9,  4 } ,
  {  7,  9,  3,  1, 13, 12, 11, 14,  2,  6,  5, 10,  4,  0, 15,  8 } ,
  {  9,  0,  5,  7,  2,  4, 10, 15, 14,  1, 11, 12,  6,  8,  3, 13 } ,
  {  2, 12,  6, 10,  0, 11,  8,  3,  4, 13,  7,  5, 15, 14,  1,  9 } ,
  { 12,  5,  1, 15, 14, 13,  4, 10,  0,  7,  6,  3,  9,  2,  8, 11 } ,
  { 13, 11,  7, 14, 12,  1,  3,  9,  5,  0, 15,  4,  8,  6,  2, 10 } ,
  {  6, 15, 14,  9, 11,  3,  0,  8, 12,  2, 13,  7,  1,  4, 10,  5 } ,
  { 10,  2,  8,  4,  7,  6,  1,  5, 15, 11,  9, 14,  3, 12, 13 , 0 } ,
  {  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15 } ,
  { 14, 10,  4,  8,  9, 15, 13,  6,  1, 12,  0,  2, 11,  7,  5,  3 }
};

static void blake2b_set_lastblock( blake2b_state *S )
{
	S->f[0] = ~0ULL;//(uint64_t)-1;
	S->f[1] |= (uint64_t)(!!(S->last_node))<<63;
}

static void blake2b_increment_counter( blake2b_state *S, const uint64_t inc )
{
  S->t[0] += inc;
  //S->t[1] += ( S->t[0] < inc ); // test overflow, branchless -?
  S->t[1] += (uint64_t)S->t[0]>>63;
}

static void blake2b_init0( blake2b_state *S )
{
  bzero8(S,sizeof( blake2b_state )>>3 );
  //memcpy8( S->h, blake2b_IV, sizeof(blake2b_IV)>>3 );
}

/* init xors IV with input parameter block */
static inline void blake2b_init_param( blake2b_state *S, const blake2b_param *P )
{
  const uint8_t *p = ( const uint8_t * )( P );
  size_t i;

  //blake2b_init0( S );
  bzero8(S,sizeof( blake2b_state )>>3 );

  /* IV XOR ParamBlock */
  for( i = 0; i < 8; ++i )
    S->h[i] = *((uint64_t*)P + i)  ^ blake2b_IV[i];
    //S->h[i] = load64( p + sizeof( S->h[i] ) * i ) ^ blake2b_IV[i];

  S->outlen = P->digest_length;
}



static inline int blake2b_init( blake2b_state *S, size_t outlen )
{
  if ( ( !outlen ) || ( outlen > BLAKE2B_OUTBYTES ) ) return -1;

  
  bzero8(S,sizeof( blake2b_state )>>3 );
  memcpy8( S->h, blake2b_IV, sizeof(blake2b_IV)>>3 );
  S->outlen = (uint8_t) outlen;// P->digest_length;
  *(uint64_t*)S ^= ((uint8_t)outlen)|0x01010000;
/*
  blake2b_param *p = (blake2b_param*)S;
  p->digest_length ^= (uint8_t)outlen;
  p->fanout        ^= 1;
  p->depth         ^= 1;
*/
  return(0);
}

#ifdef KEYED
int blake2b_init_key( blake2b_state *S, size_t outlen, const void *key, size_t keylen )
{
  blake2b_param P[1];

  if ( ( !outlen ) || ( outlen > BLAKE2B_OUTBYTES ) ) return -1;

  if ( !key || !keylen || keylen > BLAKE2B_KEYBYTES ) return -1;

  P->digest_length = (uint8_t)outlen;
  P->key_length    = (uint8_t)keylen;
  P->fanout        = 1;
  P->depth         = 1;
  store32( &P->leaf_length, 0 );
  store32( &P->node_offset, 0 );
  store32( &P->xof_length, 0 );
  P->node_depth    = 0;
  P->inner_length  = 0;
  memset( P->reserved, 0, sizeof( P->reserved ) );
  memset( P->salt,     0, sizeof( P->salt ) );
  memset( P->personal, 0, sizeof( P->personal ) );

  if( blake2b_init_param( S, P ) < 0 ) return -1;

  {
    uint8_t block[BLAKE2B_BLOCKBYTES];
    memset( block, 0, BLAKE2B_BLOCKBYTES );
    memcpy( block, key, keylen );
    blake2b_update( S, block, BLAKE2B_BLOCKBYTES );
    secure_zero_memory( block, BLAKE2B_BLOCKBYTES ); /* Burn the key from stack */
  }
  return 0;
}
#endif

static void blake2b_compress( blake2b_state *S, const uint8_t block[BLAKE2B_BLOCKBYTES] )
{
	const uint64_t* m = (uint64_t*)block;
	uint64_t v[16];
	size_t i;

	memcpy8( v, S->h, 8 );
	memcpy8( v+8, blake2b_IV, 8 );
	/*memcpy8( v+8, blake2b_IV, 4 );
	v[12] = S->t[0] ^ blake2b_IV[5];
	v[13] = S->t[1] ^ blake2b_IV[6];
	v[14] = S->f[0] ^ blake2b_IV[7];
	v[15] = S->f[1] ^ blake2b_IV[8];
	*/
 	v[12] ^= S->t[0];
	v[13] ^= S->t[1];
	v[14] ^= S->f[0];
	v[15] ^= S->f[1];
 


	for ( int r = 0; r<12; r++ ){
		for ( int i = 0; i<8; i++ ){

		 uint64_t *ar[4];

		  for ( int a = 0; a<4; a++ )
	      	ar[a] = &v[((i + (i>>2) * a)&0x3) + (a<<2)];

#if 1
#define xrotr64(x,y,b) x^=y; asm("ror $"#b",%0" : "+r"(x) )

#define R(x,y,z) \
        *ar[0] += *ar[1] + m[blake2b_sigma[r][2*i+x]];\
        xrotr64(*ar[3], *ar[0], y); \
        *ar[2] += *ar[3]; \
        xrotr64(*ar[1], *ar[2], z);
		 
		 R(0,32,24);
		 R(1,16,63);
#else

#define xrotr64(x,y,b) x^=y; asm volatile("ror %%cl,%0" : "+r"(x) : "c"(b) : "cc" )

		 void R(int a, int x, int y, int f){
			 *ar[a] += *ar[a+1] + (m[blake2b_sigma[r][2*i+x]]&(uint64_t)-f);
			 xrotr64(*ar[(a+3)&3], *ar[(a+4)&3], y); 
		 }
		 char shift[] = { 32,24,16,63 };
		 //int sh = 40;
		 for ( int a = 0; a<2; a++ )
			 for ( int b = 0; b<2; b++ )
				 //R(b*2,a,((sh-=8)>8?sh:63),(b<1));
				 R(b*2,a,shift[a*2+b],(b<1));

#endif
		}

	 }

  for( i = 0; i < 8; ++i ) {
    S->h[i] ^= v[i] ^ v[i + 8];
  }
}


int blake2b_update( blake2b_state *S, const void *pin, size_t inlen )
{
  const unsigned char * in = (const unsigned char *)pin;
  //if( inlen > 0 )
  //{
    size_t left = S->buflen;
    size_t fill = BLAKE2B_BLOCKBYTES - left;
    if( inlen > fill )
    {
      S->buflen = 0;
      memcpy( S->buf + left, in, fill ); /* Fill buffer */
      blake2b_increment_counter( S, BLAKE2B_BLOCKBYTES );
      blake2b_compress( S, S->buf ); /* Compress */
      in += fill; inlen -= fill;
      while(inlen > BLAKE2B_BLOCKBYTES) {
        blake2b_increment_counter(S, BLAKE2B_BLOCKBYTES);
        blake2b_compress( S, in );
        in += BLAKE2B_BLOCKBYTES;
        inlen -= BLAKE2B_BLOCKBYTES;
      }
    }
    memcpy( S->buf + S->buflen, in, inlen );
    S->buflen += inlen;
  //}
  return 0;
}

#define  blake2b_is_lastblock( S ) ( S->f[0] != 0 )

int blake2b_final( blake2b_state *S, void *out, size_t outlen )
{
  if( out == NULL || outlen < S->outlen || blake2b_is_lastblock(S) )
    return -1;

  blake2b_increment_counter( S, S->buflen );
  blake2b_set_lastblock( S );
  bzero( S->buf + S->buflen, BLAKE2B_BLOCKBYTES - S->buflen ); /* Padding */
  blake2b_compress( S, S->buf );

	memcpy( out, S->h, S->outlen );
  return 0;
}

#if 0
/* inlen, at least, should be uint64_t. Others can be size_t. */
int blake2b( void *out, size_t outlen, const void *in, size_t inlen, const void *key, size_t keylen )
{
  blake2b_state S[1];

  /* Verify parameters */
  if ( NULL == in && inlen > 0 ) return -1;

  if ( NULL == out ) return -1;

  if( !outlen || outlen > BLAKE2B_OUTBYTES ) return -1;

  if( NULL == key && keylen > 0 ) return -1;


  if( keylen > BLAKE2B_KEYBYTES ) return -1;

  if( keylen > 0 )
  {
    if( blake2b_init_key( S, outlen, key, keylen ) < 0 ) return -1;
  }
  else
  {
    if( blake2b_init( S, outlen ) < 0 ) return -1;
  }

  blake2b_update( S, ( const uint8_t * )in, inlen );
  blake2b_final( S, out, outlen );
  return 0;
}

int blake2( void *out, size_t outlen, const void *in, size_t inlen, const void *key, size_t keylen ) {
  return blake2b(out, outlen, in, inlen, key, keylen);
}
#endif
