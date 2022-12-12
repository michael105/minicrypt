#if 0
COMPILE nread
SHRINKELF
SEGMENTS text
NODEFAULTS

BINARY base91

return
#endif

// quick hack. BSD 3-clause. misc, 2022


#include "base91.c"

#define BUFSZ 4000

void encode(int fd, uint w){
	char buf[BUFSZ];
	char obuf[2*BUFSZ];

	struct basE91 b;
	int r;
	int ow = 0; // uint takes 33Bytes in the binary !?

	// output, and paginate
	void output(){
		char *p = obuf;
		while ( w && (r+ow > w) ){
			write(1,p,w-ow); // possible to speed up by using a further output buffer
			write(1,"\n",1);
			r -= w-ow;
			p += w-ow;
			ow=0;
		}
		write( 1, p, r );
		ow = r;
	}

	while ((r=nread(fd,buf,BUFSZ))){
		r = basE91_encode(&b,buf,r,obuf);
		output();
	}
	
	r = basE91_encode_end(&b,obuf );
	output();
	if ( ow )
		write(1,"\n",1);
}

void decode(int fd){
	char buf[BUFSZ];
	char obuf[BUFSZ];
	struct basE91 b;
	int r;

	basE91_init(&b);

	while ((r=nread(fd,buf,BUFSZ))){
		r = basE91_decode(&b,buf,r,obuf);
		write(1,obuf,r);
	}
	r = basE91_decode_end(&b,obuf );
	write(1,obuf,r);

}


int main(int argc, char **argv){
	uint w = 76; // default linebreak
	int d = 0;

	// parse arguments
	for ( *argv++; *argv && (**argv == '-'); *argv++ ){
		for ( char *o = *argv+1; *o; o++ ){
			switch (*o){
				case 'w':
					w = 0;
					*argv++;
					for ( char *n=*argv; n && ((*n>='0') && (*n<= '9')); n++ )
						w = w*10 + *n-'0';
					break;
				case 'd':
					d = 1;
					break;

				default:
					writes("base91 [-w col] [-d]\n");
					exit(0);
			}
		}
	}

	if ( !d )
		encode(0,w);
	else 
		decode(0);

	exit(0);
}

