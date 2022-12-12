#if 0
COMPILE prints printl itohex printf fmtd strcpy strcat
SHRINKELF

return
#endif

// misc, 2022. BSD 3-clause
/*
BSD 3-Clause License

Copyright (c) 2020,2022, Michael (misc) Myer (https://github.com/michael105)
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

MAIN{
	int opt=0;
	char fmt[32]; 
	strcpy(fmt,"%2x:");
	for ( *argv++; *argv; *argv++ ){
		if ( argv[0][0] == '-' ){
			for ( char *o = *argv + 1; *o; o++ )
			switch ( *o ){
				case 'd':
					opt |= 1;
					strcpy(fmt,"%3d:");
					break;
				case 'x':
					opt |= 2;
					break;
				case 'c':
					opt |= 4;
					break;

				default:
					writes("ascii [-d|h|x]\n");
					exit(0);
			}
		}
	}
	if ( (opt&3) == 3 )
		strcpy(fmt,"%3d(%2x):");
	if ( (opt&4) )
		strcat(fmt," \e[36m%c\e[37m  ");
	else
		strcat(fmt," %c  ");

	for ( int a = 32; a<256; a++ ){
		//if ( a>0x7f && a<0xa0 )
		//	continue;
		printf(fmt, a, a, a );

		if ( !((a+1)%8) )
			printl();
	}
	exit(0);
}
