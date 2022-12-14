#if 0

COMPILE memcpy8 nread bzero8 bzero memcpy memset

SHRINKELF
#STRIPFLAG
#FULLDEBUG

SEGMENTS text
NODEFAULTS

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

//#define memset(x,y,z) bzero(x,z)

#define BLAKE2b_OUTLEN    (64)  //Length of digest (32 is the secure/maximum/standard length). Setting this to zero switches the functions to expect an "outlen" parameter.

#include "blake2b.c"


#define BUFL 4000


MAIN{

	uchar buf[BUFL];
	uchar obuf[64];
	blake2b_state ctx;
	int r;
#define blen (64)

	blake2b_init(&ctx,64);


	while ( (r=nread(0,(char*)buf,BUFL)) ){
		blake2b_update(&ctx,buf,r);
	}

	blake2b_final(&ctx,obuf,64);

	
	for ( int a = 0; a<blen; a++ ){
		uchar c = obuf[a];
		buf[2*a +1] = (c&0xf)>9 ? ((c&0xf) + 'a' - 0xa) : (c&0xf) + '0';
		c >>=4;
		buf[2*a] = (c)>9 ? ((c) + 'a' - 0xa) : (c) + '0';
	}
	buf[blen*2] = '\n';
	write(1,buf,(blen*2+1));
	

	exit(0);
}
