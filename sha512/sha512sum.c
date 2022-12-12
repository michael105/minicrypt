#if 0

COMPILE memcpy8 nread bzero8 bzero

SHRINKELF
#STRIPFLAG
#FULLDEBUG

SEGMENTS text
NODEFAULTS

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



#include "sha512.c"


#define BUFL 4000


MAIN{

	uchar buf[BUFL];
	uchar obuf[64];
	int r;

	crypto_hash_sha512_state ctx;
	crypto_hash_sha512_init(&ctx);


	while ( (r=nread(0,(char*)buf,BUFL)) ){
		crypto_hash_sha512_update(&ctx,buf,r);
	}

	crypto_hash_sha512_final(&ctx,obuf);

	//write(1,obuf,64);
	
	for ( int a = 0; a<64; a++ ){
		char c = obuf[a];
		buf[2*a +1] = (c&0xf)>9 ? ((c&0xf) + 'a' - 0xa) : (c&0xf) + '0';
		c >>=4;
		buf[2*a] = (c&0xf)>9 ? ((c&0xf) + 'a' - 0xa) : (c&0xf) + '0';
	}
	buf[128] = '\n';
	write(1,buf,129);
	
	/*
	ulong *lb = (ulong*)buf;
	for ( ulong *l = obuf; l < obuf + 64; l++){
		for ( int a = 0; a<16; a++ ){
			*lb = ((*lb)<<4) | ( ((*l)&0xF)>9 ? (((*l)&0xf) + 'a' - 0xa) : ((*l)&0xf) + '0');
			(*l) >>=4;
		}
		lb += 2;
		
	}
		buf[128] = '\n';
	write(1,buf,129);
	*/

		//printf("%2x",(char*)obuf[a]);

	exit(0);
}
