#if 0
mini_start
mini_write 
mini_writes
mini_read

COMPILE nread

LDSCRIPT textandbss
SHRINKELF
#STRIPFLAG

INCLUDESRC

return
#endif

// misc, BSD 3clause

#define BUF 4000

//void __attribute__((noreturn))usage(){
void usage(){
	writes("tohex. convert stdin to hexadecimal output.\n");
	exit(0);
}



int main(int argc, char **argv){

	if ( argc>1 )
		usage();

	const char *table = "0123456789abcdef";
	char c[BUF/2];
	char o[BUF];
	int r = 0;
	while ( (r=nread(0,c,BUF/2) ) > 0 ){
		for (int a=0; a<r; a++ ){
			o[a<<1]= table[ (uchar)c[a]>>4 ]; // logic shift (without cast it's arithmetic (!!))
			o[(a<<1)+1] = table[ c[a] & 0xf ];
		}
		write(1, o, r<<1 );
	}

	exit(0);
}
