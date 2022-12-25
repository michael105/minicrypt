#if 0
mini_start
mini_write 
mini_writes
mini_nread

INCLUDESRC
SHRINKELF
return
#endif

// misc, BSD 3clause

#define BUF 4000


void usage(){
	writes("fromhex. convert hexadecimal input to ascii.\n\
Doesn't check for errors.(0x, linebreaks,..)\n");
	exit(0);
}


int main(int argc, char **argv){

	if ( argc>1 )
		usage();

	char buf[BUF];
	char obuf[BUF/2];
	char *o = obuf;
	int r, s = 1;
	while ( (r=nread(0,buf,BUF) ) > 0 ){

		const char *b = buf;
		while ( *b && ( b < buf + r )){
			char diff = 0;
			if ( *b >= '0' && *b <= '9' )
				diff = '0';
			else if ( *b >= 'a' && *b<='f' )
				diff = 'a' - 10;
			else if ( *b >= 'A' && *b<='F' )
				diff = 'A' - 10;

			if ( diff ){
				if ( s ){
					*o = (*b - diff) << 4;
					s = 0;
				} else {
					*o |= (*b-diff);
					s = 1;
					o++;
				}
			}
			b++;
		}
		write(1,obuf,o-obuf);
	}

#if 0
		for (int a=0; a<r; a+=2 ){
			char t=c[a];
			char t2=c[a+1];
			o[a>>1]= ((47 < t && t < 58 ?(t-48) : (t-87) )<<4) | 
				(47<t2 && t2<58 ? (t2-48) : (t2-87));
		}
		write(1,o,r>>1);
	}
#endif

	exit(0);
}
