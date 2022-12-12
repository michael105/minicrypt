#if 0

COMPILE memcpy8 nread memset memcpy globals malloc_brk free calloc arch_prctl bzero

SHRINKELF
#STRIPFLAG
#FULLDEBUG

SEGMENTS text bss

NODEFAULTS

MALLOC brk

HEADERGUARDS
return
#endif
/*
 misc, 2022. BSD 3-clause

BSD 3-Clause License

Copyright (c) 2022, Michael (misc) Myer (https://github.com/michael105)
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/




#define memset(a,b,c) bzero(a,c)

#include "md6.h"
#include "md6_compress.c"
#include "md6_mode.c"
#include "md6_nist.c"


#define BUFL 1024



/* Constants for MD6 */

#define w  md6_w
#define c  md6_c
#define n  md6_n
#define b  md6_b
#define u  md6_u
#define v  md6_v
#define k  md6_k
#define q  md6_q

/* Useful macros: min and max */
#ifndef min
#define min(a,b) ((a)<(b)? (a) : (b))
#endif
#ifndef max
#define max(a,b) ((a)>(b)? (a) : (b))
#endif

/* MD6 parameters */
typedef struct {
int d;                    /* digest length */
int L;                    /* mode parameter */
int r;                    /* number of rounds */
int use_default_r;        /* 1 if r should be set to the default, 0 if r is explicitly provided */
unsigned char K[100];     /* key */
int keylen;               /* key length in bytes (at most 64) */
md6_state st;             /* md6 computation state */
} args;

//char msg[5000];           /* message to be hashed (if given with -M) */
//int msglenbytes;          /* message length in bytes */



void hash_init( args* a)
{ int err;
  if ((err=md6_full_init(&a->st,a->d,a->K,a->keylen,a->L,a->r)))
    {       
		writes("Error\n");
		exit(err);

	 }
}

void hash_update(args *a, char* data, 
		 uint64_t databitlen)
{ int err;
  if ((err=md6_update(&a->st, 
		      (unsigned char *)data, 
		      databitlen)))
    { 
		 writes("Error\n");
		exit(err);
    }
}

void hash_final(args *a)
{ int err;
  if ((err=md6_final(&a->st,NULL)))
    {	
		 writes("Error\n");
		exit(err);
    }
}


#undef c

MAIN{

	char buf[BUFL];

	/* set default md6 parameter settings */
	args a;
	//a.d = 512;                
	a.d = 256;                
	a.keylen = 0;
	a.L = 64;
	a.r = md6_default_r(a.d,a.keylen);
	a.use_default_r = 1;

	int r;


	// parse arguments
	for ( *argv++; *argv && (**argv == '-'); *argv++ ){
		for ( char *o = *argv+1; *o; o++ ){
			switch (*o){
				case 'b':
					a.d = 0;
					*argv++;
					for ( char *c=*argv; c && ((*c>='0') && (*c<= '9')); c++ )
						a.d = a.d*10 + *c-'0';
					if ( a.d > 3 && a.d < 513 )
						break;

				default:
					writes("md6sum [-b bits of hash(4-512)]\n");
					exit(0);
			}
		}
	}


	hash_init(&a);

	while ( (r=nread(0,(char*)buf,BUFL)) ){
		hash_update(&a,buf,r*8);
	}

	hash_final(&a);

	//printf("%s\n",st.hexhashval);
	write(1,a.st.hexhashval,a.d/4);
	write(1,"\n",1);

	exit(0);
}
