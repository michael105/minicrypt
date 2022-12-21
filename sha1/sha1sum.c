#if 0

COMPILE memcpy8 nread bzero8 bzero memset memcpy

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



#include "sha1.c"

#define CTX SHA1_CTX
#define INIT SHA1Init
#define UPDATE SHA1Update
#define FINAL(ctx,buf) SHA1Final(buf,ctx)

#define HASHLEN 20

#define BUFL 4000


MAIN{

	uchar buf[BUFL];
	//uchar obuf[64];
	uchar *obuf = buf+(HASHLEN);
	int r;

	CTX ctx;
	INIT(&ctx);


	while ( (r=nread(0,(char*)buf,BUFL)) ){
		UPDATE(&ctx,buf,r);
	}

	FINAL(&ctx,buf);

	//write(1,obuf,64);
	//const char *table = "0123456789abcdef";
	
	for ( int a = 0; a<HASHLEN; a++ ){
		char c = buf[a];
		for ( int b = 2; b--; ){
			char d = c&0xf;
			obuf[(a<<1) +b] = d>9 ? (d + 'a' - 0xa) : d + '0';
			c >>=4;
		}
		//obuf[a<<1] = (c&0xf)>9 ? ((c&0xf) + 'a' - 0xa) : (c&0xf) + '0';

		//buf[a<<1] = (((uchar)c>>4)+'0');
		//if ( buf[a<<1] > '9' )
		//	buf[a<<1] += 39 ;
	//	buf[(a<<1)+1] = ((c&0xf) +'0');
		//if ( buf[(a<<1)+1] > '9' )
		//	buf[(a<<1)+1] += 39 ;



	//	buf[a<<1]= table[ (uchar)c>>4 ]; // logic shift (without cast it's arithmetic (!!))
	//	buf[(a<<1)+1]= table[ c&0xf ];
	}
/*	for ( int a = 0; a<HASHLEN*2; a++ )
		if (  buf[a] > '9' )
		buf[a] += 39; */

	obuf[HASHLEN*2] = '\n';
	write(1,obuf,HASHLEN*2+1);
	
	exit(0);
}
