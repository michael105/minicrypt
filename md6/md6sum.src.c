typedef long time_t;
typedef long suseconds_t;
typedef unsigned useconds_t;
struct timeval { time_t tv_sec; suseconds_t tv_usec; };
struct itimerval {
  struct timeval it_interval;
  struct timeval it_value;
};
typedef unsigned char uchar;
typedef int wchar_t;
typedef long double double_t;
typedef float float_t;
typedef struct { long long __ll; long double __ld; } max_align_t;
typedef struct { union { int __i[14]; volatile int __vi[14]; unsigned long __s[7]; } __u; } pthread_attr_t;
typedef struct { union { int __i[10]; volatile int __vi[10]; volatile void *volatile __p[5]; } __u; } pthread_mutex_t;
typedef struct { union { int __i[10]; volatile int __vi[10]; volatile void *volatile __p[5]; } __u; } mtx_t;
typedef struct { union { int __i[12]; volatile int __vi[12]; void *__p[6]; } __u; } pthread_cond_t;
typedef struct { union { int __i[12]; volatile int __vi[12]; void *__p[6]; } __u; } cnd_t;
typedef struct { union { int __i[14]; volatile int __vi[14]; void *__p[7]; } __u; } pthread_rwlock_t;
typedef struct { union { int __i[8]; volatile int __vi[8]; void *__p[4]; } __u; } pthread_barrier_t;
typedef unsigned long uintptr_t;
typedef long ptrdiff_t;
typedef long ssize_t;
typedef long intptr_t;
typedef long regoff_t;
typedef long register_t;
typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;
typedef signed long int64_t;
typedef signed long intmax_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long uint64_t;
typedef unsigned long u_int64_t;
typedef unsigned long uintmax_t;
typedef unsigned mode_t;
typedef unsigned long nlink_t;
typedef long off_t;
typedef unsigned long ino_t;
typedef unsigned long dev_t;
typedef long blksize_t;
typedef long blkcnt_t;
typedef unsigned long fsblkcnt_t;
typedef unsigned long fsfilcnt_t;
typedef unsigned wint_t;
typedef unsigned long wctype_t;
typedef void * timer_t;
typedef int clockid_t;
typedef long clock_t;
struct timespec { time_t tv_sec; long tv_nsec; };
typedef int pid_t;
typedef unsigned id_t;
typedef unsigned uid_t;
typedef unsigned gid_t;
typedef int key_t;
typedef struct __mbstate_t { unsigned __opaque1, __opaque2; } mbstate_t;
typedef struct __locale_struct * locale_t;
struct iovec { void *iov_base; long iov_len; };
typedef unsigned socklen_t;
typedef unsigned short sa_family_t;
typedef int8_t int_fast8_t;
typedef int64_t int_fast64_t;
typedef int8_t int_least8_t;
typedef int16_t int_least16_t;
typedef int32_t int_least32_t;
typedef int64_t int_least64_t;
typedef uint8_t uint_fast8_t;
typedef uint64_t uint_fast64_t;
typedef uint8_t uint_least8_t;
typedef uint16_t uint_least16_t;
typedef uint32_t uint_least32_t;
typedef uint64_t uint_least64_t;
typedef int __gwchar_t;
__BEGIN_DECLS
typedef struct
  {
    long long int quot;
    long long int rem;
  } imaxdiv_t;
extern intmax_t imaxabs (intmax_t __n) __THROW __attribute__ ((__const__));
extern imaxdiv_t imaxdiv (intmax_t __numer, intmax_t __denom)
      __THROW __attribute__ ((__const__));
extern intmax_t strtoimax (const char *__restrict __nptr,
      char **__restrict __endptr, int __base) __THROW;
extern uintmax_t strtoumax (const char *__restrict __nptr,
       char ** __restrict __endptr, int __base) __THROW;
extern intmax_t wcstoimax (const __gwchar_t *__restrict __nptr,
      __gwchar_t **__restrict __endptr, int __base)
     __THROW;
extern uintmax_t wcstoumax (const __gwchar_t *__restrict __nptr,
       __gwchar_t ** __restrict __endptr, int __base)
     __THROW;
__END_DECLS
typedef uint64_t md6_word;
extern int md6_default_r( int d,
     int keylen
     );
void md6_main_compression_loop( md6_word *A,
    int r
    );
int md6_compress( md6_word *C,
    md6_word *N,
    int r,
    md6_word *A
                );
typedef uint64_t md6_control_word;
md6_control_word md6_make_control_word( int r,
     int L,
     int z,
     int p,
     int keylen,
     int d
     );
typedef uint64_t md6_nodeID;
md6_nodeID md6_make_nodeID( int ell,
       int i
       );
void md6_pack( md6_word* N,
        const md6_word* Q,
        md6_word* K,
        int ell, int i,
        int r, int L, int z, int p, int keylen, int d,
        md6_word* B
        );
int md6_standard_compress(
        md6_word *C,
 const md6_word *Q,
 md6_word *K,
 int ell, int i,
 int r, int L, int z, int p, int keylen, int d,
 md6_word* B
      );
typedef struct {
  int d;
  int hashbitlen;
  unsigned char hashval[ 16*(64/8) ];
  unsigned char hexhashval[(16*(64/8))+1];
  int initialized;
  uint64_t bits_processed;
  uint64_t compression_calls;
  int finalized;
  md6_word K[ 8 ];
  int keylen;
  int L;
  int r;
  int top;
  md6_word B[ 29 ][ 64 ];
  unsigned int bits[ 29 ];
  uint64_t i_for_level[ 29 ];
} md6_state;
extern int md6_init( md6_state *st,
       int d
       );
extern int md6_full_init( md6_state *st,
     int d,
     unsigned char *key,
     int keylen,
     int L,
     int r
     );
extern int md6_update( md6_state *st,
         unsigned char *data,
         uint64_t databitlen
         );
extern int md6_final( md6_state *st,
        unsigned char *hashval
        );
extern int md6_hash( int d,
       unsigned char *data,
       uint64_t databitlen,
       unsigned char *hashval
       );
extern int md6_full_hash( int d,
     unsigned char *data,
     uint64_t databitlen,
     unsigned char *key,
     int keylen,
     int L,
     int r,
     unsigned char *hashval
     );
void (* compression_hook)(md6_word *C,
     const md6_word *Q,
     md6_word *K,
     int ell,
     int i,
     int r,
     int L,
     int z,
     int p,
     int keylen,
     int d,
     md6_word *N
     );
__BEGIN_DECLS
typedef long unsigned int long;
typedef long long __kernel_long_t;
typedef unsigned long __kernel_ulong_t;
typedef int sig_atomic_t;
typedef void (*__sighandler_t) (int);
typedef __sighandler_t sig_t;
typedef unsigned long __u64;
typedef unsigned int __u32;
typedef signed long __s64;
typedef signed int __s32;
typedef unsigned short __u16;
typedef signed short __s16;
__BEGIN_NAMESPACE_STD
typedef struct __STDIO_FILE_STRUCT FILE;
__END_NAMESPACE_STD
typedef struct __STDIO_FILE_STRUCT __FILE;
typedef int wchar_t;
typedef unsigned int wint_t;
typedef struct
{
 wchar_t __mask;
 wchar_t __wc;
} __mbstate_t;
typedef struct {
 __off_t __pos;
 __mbstate_t __mbstate;
 int __mblen_pending;
} __STDIO_fpos_t;
typedef struct {
 __off64_t __pos;
 __mbstate_t __mbstate;
 int __mblen_pending;
} __STDIO_fpos64_t;
typedef __off64_t __offmax_t;
typedef __ssize_t __io_read_fn(void *__cookie, char *__buf, long __bufsize);
typedef __ssize_t __io_write_fn(void *__cookie,
     const char *__buf, long __bufsize);
typedef int __io_seek_fn(void *__cookie, __offmax_t *__pos, int __whence);
typedef int __io_close_fn(void *__cookie);
typedef struct {
 __io_read_fn *read;
 __io_write_fn *write;
 __io_seek_fn *seek;
 __io_close_fn *close;
} _IO_cookie_io_functions_t;
struct __STDIO_FILE_STRUCT {
 unsigned short __modeflags;
 unsigned char __ungot_width[2];
 int __filedes;
 unsigned char *__bufstart;
 unsigned char *__bufend;
 unsigned char *__bufpos;
 unsigned char *__bufread;
 unsigned char *__bufgetc_u;
 unsigned char *__bufputc_u;
 struct __STDIO_FILE_STRUCT *__nextopen;
 wchar_t __ungot[2];
 __mbstate_t __state;
 int __user_locking;
 __UCLIBC_IO_MUTEX(__lock);
};
extern int __fgetc_unlocked(FILE *__stream);
extern int __fputc_unlocked(int __c, FILE *__stream);
extern FILE *__stdin;
extern FILE *__stdout;
typedef __builtin_va_list va_list;
__BEGIN_NAMESPACE_STD
typedef __STDIO_fpos_t fpos_t;
__END_NAMESPACE_STD
extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;
__BEGIN_NAMESPACE_STD
extern int remove (const char *__filename) __THROW;
extern int rename (const char *__old, const char *__new) __THROW;
__END_NAMESPACE_STD
__BEGIN_NAMESPACE_STD
extern FILE *tmpfile (void) __wur;
extern char *tmpnam (char *__s) __THROW __wur;
__END_NAMESPACE_STD
__BEGIN_NAMESPACE_STD
extern int fclose (FILE *__stream);
extern int fflush (FILE *__stream);
__END_NAMESPACE_STD
__BEGIN_NAMESPACE_STD
extern FILE *fopen (const char *__restrict __filename,
      const char *__restrict __modes) __wur;
extern FILE *freopen (const char *__restrict __filename,
        const char *__restrict __modes,
        FILE *__restrict __stream) __wur;
__END_NAMESPACE_STD
__BEGIN_NAMESPACE_STD
extern void setbuf (FILE *__restrict __stream, char *__restrict __buf) __THROW;
extern int setvbuf (FILE *__restrict __stream, char *__restrict __buf,
      int __modes, long __n) __THROW;
__END_NAMESPACE_STD
__BEGIN_NAMESPACE_STD
extern int fprintf (FILE *__restrict __stream,
      const char *__restrict __format, ...);
extern int printf (const char *__restrict __format, ...);
extern int sprintf (char *__restrict __s,
      const char *__restrict __format, ...) __THROWNL
     __attribute__ ((__format__ (__printf__, 2, 3)));
extern int vfprintf (FILE *__restrict __s, const char *__restrict __format,
       __gnuc_va_list __arg);
extern int vprintf (const char *__restrict __format, __gnuc_va_list __arg);
extern int vsprintf (char *__restrict __s, const char *__restrict __format,
       __gnuc_va_list __arg) __THROWNL
     __attribute__ ((__format__ (__printf__, 2, 0)));
__END_NAMESPACE_STD
__BEGIN_NAMESPACE_STD
extern int fscanf (FILE *__restrict __stream,
     const char *__restrict __format, ...)
     __attribute__ ((__format__ (__scanf__, 2, 3))) __wur;
extern int scanf (const char *__restrict __format, ...)
     __attribute__ ((__format__ (__scanf__, 1, 2))) __wur;
extern int sscanf (const char *__restrict __s,
     const char *__restrict __format, ...)
     __THROW __attribute__ ((__format__ (__scanf__, 2, 3)));
__END_NAMESPACE_STD
__BEGIN_NAMESPACE_STD
extern int fgetc (FILE *__stream);
extern int getc (FILE *__stream);
extern int getchar (void);
__END_NAMESPACE_STD
__BEGIN_NAMESPACE_STD
extern int fputc (int __c, FILE *__stream);
extern int putc (int __c, FILE *__stream);
extern int putchar (int __c);
__END_NAMESPACE_STD
__BEGIN_NAMESPACE_STD
extern char *fgets (char *__restrict __s, int __n, FILE *__restrict __stream)
     __wur;
extern char *gets (char *__s) __wur __attribute_deprecated__;
__END_NAMESPACE_STD
__BEGIN_NAMESPACE_STD
extern int fputs (const char *__restrict __s, FILE *__restrict __stream);
extern int puts (const char *__s);
extern int ungetc (int __c, FILE *__stream);
extern long fread (void *__restrict __ptr, long __size,
       long __n, FILE *__restrict __stream) __wur;
extern long fwrite (const void *__restrict __ptr, long __size,
        long __n, FILE *__restrict __s) __wur;
__END_NAMESPACE_STD
__BEGIN_NAMESPACE_STD
extern int fseek (FILE *__stream, long int __off, int __whence);
extern long int ftell (FILE *__stream) __wur;
extern void rewind (FILE *__stream);
__END_NAMESPACE_STD
__BEGIN_NAMESPACE_STD
extern int fgetpos (FILE *__restrict __stream, fpos_t *__restrict __pos);
extern int fsetpos (FILE *__stream, const fpos_t *__pos);
__END_NAMESPACE_STD
__BEGIN_NAMESPACE_STD
extern void clearerr (FILE *__stream) __THROW;
extern int feof (FILE *__stream) __THROW __wur;
extern int ferror (FILE *__stream) __THROW __wur;
__END_NAMESPACE_STD
__BEGIN_NAMESPACE_STD
extern void perror (const char *__s);
__END_NAMESPACE_STD
__END_DECLS
__BEGIN_DECLS
__BEGIN_NAMESPACE_STD
typedef struct
  {
    int quot;
    int rem;
  } div_t;
typedef struct
  {
    long int quot;
    long int rem;
  } ldiv_t;
__END_NAMESPACE_STD
extern long _stdlib_mb_cur_max (void) __THROW __wur;
__BEGIN_NAMESPACE_STD
extern double atof (const char *__nptr)
     __THROW __attribute_pure__ __nonnull ((1)) __wur;
extern int atoi (const char *__nptr)
     __THROW __attribute_pure__ __nonnull ((1)) __wur;
extern long int atol (const char *__nptr)
     __THROW __attribute_pure__ __nonnull ((1)) __wur;
__END_NAMESPACE_STD
__BEGIN_NAMESPACE_STD
extern double strtod (const char *__restrict __nptr,
        char **__restrict __endptr)
     __THROW __nonnull ((1)) __wur;
__END_NAMESPACE_STD
__BEGIN_NAMESPACE_STD
extern long int strtol (const char *__restrict __nptr,
   char **__restrict __endptr, int __base)
     __THROW __nonnull ((1)) __wur;
extern unsigned long int strtoul (const char *__restrict __nptr,
      char **__restrict __endptr, int __base)
     __THROW __nonnull ((1)) __wur;
__END_NAMESPACE_STD
__BEGIN_NAMESPACE_STD
extern int rand (void) __THROW;
extern void srand (unsigned int __seed) __THROW;
__END_NAMESPACE_STD
__BEGIN_NAMESPACE_STD
extern void *malloc (long __size) __THROW __attribute_malloc__ __wur;
extern void *calloc (long __nmemb, long __size)
     __THROW __attribute_malloc__ __wur;
__END_NAMESPACE_STD
__BEGIN_NAMESPACE_STD
extern void *realloc (void *__ptr, long __size)
     __THROW __wur;
extern void free (void *__ptr) __THROW;
__END_NAMESPACE_STD
__BEGIN_NAMESPACE_STD
extern void abort (void) __THROW __attribute__ ((__noreturn__));
extern int atexit (void (*__func) (void)) __THROW __nonnull ((1));
__END_NAMESPACE_STD
__BEGIN_NAMESPACE_STD
extern void exit (int __status) __THROW __attribute__ ((__noreturn__));
__END_NAMESPACE_STD
__BEGIN_NAMESPACE_STD
extern char *getenv (const char *__name) __THROW __nonnull ((1)) __wur;
__END_NAMESPACE_STD
extern char *secure_getenv (const char *__name)
     __THROW __nonnull ((1)) __wur;
__BEGIN_NAMESPACE_STD
extern int system (const char *__command) __wur;
__END_NAMESPACE_STD
typedef int (*__compar_fn_t) (const void *, const void *);
__BEGIN_NAMESPACE_STD
extern void *bsearch (const void *__key, const void *__base,
        long __nmemb, long __size, __compar_fn_t __compar)
     __nonnull ((1, 2, 5)) __wur;
extern void qsort (void *__base, long __nmemb, long __size,
     __compar_fn_t __compar) __nonnull ((1, 4));
extern int abs (int __x) __THROW __attribute__ ((__const__)) __wur;
extern long int labs (long int __x) __THROW __attribute__ ((__const__)) __wur;
__END_NAMESPACE_STD
__BEGIN_NAMESPACE_STD
extern div_t div (int __numer, int __denom)
     __THROW __attribute__ ((__const__)) __wur;
extern ldiv_t ldiv (long int __numer, long int __denom)
     __THROW __attribute__ ((__const__)) __wur;
__END_NAMESPACE_STD
__BEGIN_NAMESPACE_STD
extern int mblen (const char *__s, long __n) __THROW __wur;
extern int mbtowc (wchar_t *__restrict __pwc,
     const char *__restrict __s, long __n) __THROW __wur;
extern int wctomb (char *__s, wchar_t __wchar) __THROW __wur;
extern long mbstowcs (wchar_t *__restrict __pwcs,
   const char *__restrict __s, long __n) __THROW;
extern long wcstombs (char *__restrict __s,
   const wchar_t *__restrict __pwcs, long __n)
     __THROW;
__END_NAMESPACE_STD
__END_DECLS
__BEGIN_DECLS
__BEGIN_NAMESPACE_STD
extern void *memcpy (void *__restrict __dest,
       const void *__restrict __src, long __n)
     __THROW __nonnull ((1, 2));
extern void *memmove (void *__dest, const void *__src, long __n)
     __THROW __nonnull ((1, 2));
__END_NAMESPACE_STD
__BEGIN_NAMESPACE_STD
extern void *bzero(void *__s,long __n) __THROW __nonnull ((1));
extern int memcmp (const void *__s1, const void *__s2, long __n)
     __THROW __attribute_pure__ __nonnull ((1, 2));
extern void *memchr (const void *__s, int __c, long __n)
      __THROW __attribute_pure__ __nonnull ((1));
__END_NAMESPACE_STD
__BEGIN_NAMESPACE_STD
extern char *strcpy (char *__restrict __dest, const char *__restrict __src)
     __THROW __nonnull ((1, 2));
extern char *strncpy (char *__restrict __dest,
        const char *__restrict __src, long __n)
     __THROW __nonnull ((1, 2));
extern char *strcat (char *__restrict __dest, const char *__restrict __src)
     __THROW __nonnull ((1, 2));
extern char *strncat (char *__restrict __dest, const char *__restrict __src,
        long __n) __THROW __nonnull ((1, 2));
extern int strcmp (const char *__s1, const char *__s2)
     __THROW __attribute_pure__ __nonnull ((1, 2));
extern int strncmp (const char *__s1, const char *__s2, long __n)
     __THROW __attribute_pure__ __nonnull ((1, 2));
extern int strcoll (const char *__s1, const char *__s2)
     __THROW __attribute_pure__ __nonnull ((1, 2));
extern long strxfrm (char *__restrict __dest,
         const char *__restrict __src, long __n)
     __THROW __nonnull ((2));
__END_NAMESPACE_STD
__BEGIN_NAMESPACE_STD
extern char *strchr (const char *__s, int __c)
     __THROW __attribute_pure__ __nonnull ((1));
extern char *strrchr (const char *__s, int __c)
     __THROW __attribute_pure__ __nonnull ((1));
__END_NAMESPACE_STD
__BEGIN_NAMESPACE_STD
extern long strcspn (const char *__s, const char *__reject)
     __THROW __attribute_pure__ __nonnull ((1, 2));
extern long strspn (const char *__s, const char *__accept)
     __THROW __attribute_pure__ __nonnull ((1, 2));
extern char *strpbrk (const char *__s, const char *__accept)
     __THROW __attribute_pure__ __nonnull ((1, 2));
extern char *strstr (const char *__haystack, const char *__needle)
     __THROW __attribute_pure__ __nonnull ((1, 2));
extern char *strtok (char *__restrict __s, const char *__restrict __delim)
     __THROW __nonnull ((2));
__END_NAMESPACE_STD
__BEGIN_NAMESPACE_STD
extern long strlen (const char *__s)
     __THROW __attribute_pure__ __nonnull ((1));
__END_NAMESPACE_STD
__BEGIN_NAMESPACE_STD
extern char *strerror (int __errnum) __THROW;
__END_NAMESPACE_STD
__END_DECLS
static const md6_word S0 = (md6_word)0x0123456789abcdefULL;
static const md6_word Smask = (md6_word)0x7311c2812425cfa0ULL;
void md6_main_compression_loop( md6_word* A , int r )
{ md6_word x, S;
  int i,j;
  S = S0;
 void loop_bodyf(int rs, int ls,int step){
      x = S;
      x^= A[i+step-89];
      x^= A[i+step-89];
      x^= A[i+step-17];
      x^= A[i+step-17];
      x^= ( A[i+step-18] & A[i+step-21] );
      x^= ( A[i+step-18] & A[i+step-21] );
      x^= ( A[i+step-31] & A[i+step-67] );
      x^= ( A[i+step-31] & A[i+step-67] );
      x^= (x >> rs);
      x^= (x >> rs);
      A[i+step] = x^ (x << ls);
      A[i+step] = x^ (x << ls);
 }
 static const char lp[16][3] = {
{10,11, 0},
{ 5,24, 1},
{13, 9, 2},
{10,16, 3},
{11,15, 4},
{12, 9, 5},
{ 2,27, 6},
{ 7,15, 7},
{14, 6, 8},
{15, 2, 9},
{ 7,29,10},
{13, 8,11},
{11,15,12},
{ 7, 5,13},
{ 6,31,14},
{12, 9,15} };
  for (j = 0, i = 89; j<r*16; j+=16)
    {
   for ( int a = 0; a<16; a++ )
    loop_bodyf(lp[a][0],lp[a][1],lp[a][2]);
      S = (S << 1)^ (S >> (64 -1)) ^ (S & Smask);
      S = (S << 1)^ (S >> (64 -1)) ^ (S & Smask);
      i += 16;
    }
}
int md6_compress( md6_word *C,
    md6_word *N,
    int r,
    md6_word *A
   )
{ md6_word* A_as_given = A;
  if ( N == ((void *)0)) return 9;
  if ( C == ((void *)0)) return 15;
  if ( r<0 || r > 255) return 17;
  if ( A == ((void *)0)) A = calloc(r*16 +89,sizeof(md6_word));
  if ( A == ((void *)0)) return 18;
  memcpy( A, N, 89*sizeof(md6_word) );
  md6_main_compression_loop( A, r );
  memcpy( C, A+(r-1)*16 +89, 16*sizeof(md6_word) );
  if ( A_as_given == ((void *)0) )
    {
   bzero(A,(r*16 +89)*sizeof(md6_word));
      free(A);
    }
  return 0;
}
md6_control_word md6_make_control_word( int r,
     int L,
     int z,
     int p,
     int keylen,
     int d
     )
{ md6_control_word V;
  V = ( (((md6_control_word) 0) << 60) |
 (((md6_control_word) r) << 48) |
 (((md6_control_word) L) << 40) |
 (((md6_control_word) z) << 36) |
 (((md6_control_word) p) << 20) |
 (((md6_control_word) keylen) << 12 ) |
        (((md6_control_word) d)) );
  return V;
}
md6_nodeID md6_make_nodeID( int ell,
         int i
       )
{ md6_nodeID U;
  U = ( (((md6_nodeID) ell) << 56) |
 ((md6_nodeID) i) );
  return U;
}
void md6_pack( md6_word*N,
        const md6_word* Q,
        md6_word* K,
        int ell, int i,
        int r, int L, int z, int p, int keylen, int d,
        md6_word* B )
{ int j;
  int ni;
  md6_nodeID U;
  md6_control_word V;
  ni = 0;
  for (j=0;j<15;j++) N[ni++] = Q[j];
  for (j=0;j<8;j++) N[ni++] = K[j];
  U = md6_make_nodeID(ell,i);
  memcpy((unsigned char *)&N[ni],
  &U,
  (((64/64)*(64/8))<(sizeof(md6_nodeID))? ((64/64)*(64/8)) : (sizeof(md6_nodeID))));
  ni += (64/64);
  V = md6_make_control_word(
   r,L,z,p,keylen,d);
  memcpy((unsigned char *)&N[ni],
  &V,
  (((64/64)*(64/8))<(sizeof(md6_control_word))? ((64/64)*(64/8)) : (sizeof(md6_control_word))));
  ni += (64/64);
  memcpy(N+ni,B,64*sizeof(md6_word));
}
int md6_standard_compress( md6_word* C,
      const md6_word* Q,
      md6_word* K,
      int ell, int i,
      int r, int L, int z, int p, int keylen, int d,
      md6_word* B
      )
{ md6_word N[89];
  md6_word A[5000];
  if ( (C == ((void *)0)) ) return 15;
  if ( (B == ((void *)0)) ) return 10;
  if ( (r<0) | (r>255) ) return 17;
  if ( (L<0) | (L>255) ) return 16;
  if ( (ell < 0) || (ell > 255) ) return 11;
  if ( (p < 0) || (p > 64*64 ) ) return 12;
  if ( (d <= 0) || (d > 16*64/2) ) return 2;
  if ( (K == ((void *)0)) ) return 13;
  if ( (Q == ((void *)0)) ) return 14;
  md6_pack(N,Q,K,ell,i,r,L,z,p,keylen,d,B);
  if (compression_hook != ((void *)0))
    compression_hook(C,Q,K,ell,i,r,L,z,p,keylen,d,B);
  return md6_compress(C,N,r,A);
}
int md6_default_r( int d ,
                   int keylen )
{ int r;
  r = 40 + (d/4);
  if (keylen>0)
    r = ((80)>(r)? (80) : (r));
  return r;
}
static const md6_word Q[15] =
  {
    0x7311c2812425cfa0ULL,
    0x6432286434aac8e7ULL,
    0xb60450e9ef68b7c1ULL,
    0xe8fb23908d9f06f1ULL,
    0xdd2e76cba691e5bfULL,
    0x0cd0d63b2c30bc41ULL,
    0x1f8ccf6823058f8aULL,
    0x54e5ed5b88e3775dULL,
    0x4ad12aae0a6d6031ULL,
    0x3e7f16bb88222e0dULL,
    0x8af8671d3fb50c2cULL,
    0x995ad1178bd25c31ULL,
    0xc878c1dd04c4b633ULL,
    0x3b72066c7a1552acULL,
    0x0d6f3522631effcbULL,
  };
void md6_detect_byte_order( void )
{
}
md6_word md6_byte_reverse( md6_word x )
{
  x = (x << 32) | (x >> 32);
  x = ((x & ((md6_word)0x0000ffff0000ffffULL)) << 16) | ((x & ~((md6_word)0x0000ffff0000ffffULL)) >> 16);
  x = ((x & ((md6_word)0x00ff00ff00ff00ffULL)) << 8) | ((x & ~((md6_word)0x00ff00ff00ff00ffULL)) >> 8);
  return x;
}
void md6_reverse_little_endian( md6_word *x, int count )
{
  int i;
  if ((1))
    for (i=0;i<count;i++)
      x[i] = md6_byte_reverse(x[i]);
}
void append_bits( unsigned char *dest, unsigned int destlen,
    unsigned char *src, unsigned int srclen )
{ int i, di, accumlen;
  uint16_t accum;
  int srcbytes;
  if (srclen == 0) return;
  accum = 0;
  accumlen = 0;
  if (destlen%8 != 0)
    { accumlen = destlen%8;
      accum = dest[destlen/8];
      accum = accum >> (8-accumlen);
    }
  di = destlen/8;
  srcbytes = (srclen+7)/8;
  for (i=0;i<srcbytes;i++)
    {
      if (i != srcbytes-1)
 { accum = (accum << 8)^ src[i];
 { accum = (accum << 8)^ src[i];
   accumlen += 8;
 }
      else
 { int newbits = ((srclen%8 == 0) ? 8 : (srclen%8));
   accum = (accum << newbits) | (src[i] >> (8-newbits));
   accumlen += newbits;
 }
      while ( ( (i != srcbytes-1) & (accumlen >= 8) ) ||
       ( (i == srcbytes-1) & (accumlen > 0) ) )
 { int numbits = ((8)<(accumlen)? (8) : (accumlen));
   unsigned char bits;
   bits = accum >> (accumlen - numbits);
   bits = bits << (8-numbits);
   bits &= (0xff00 >> numbits);
   dest[di++] = bits;
   accumlen -= numbits;
 }
    }
}
int md6_full_init( md6_state *st,
     int d,
     unsigned char *key,
     int keylen,
     int L,
     int r
     )
{
  if (st == ((void *)0)) return 3;
  if ( (key != ((void *)0)) && ((keylen < 0) || (keylen > 8*(64/8))) )
    return 4;
  if ( d < 1 || d > 512 || d > 64*16/2 ) return 2;
  md6_detect_byte_order();
  bzero(st,sizeof(md6_state));
  st->d = d;
  if (key != ((void *)0) && keylen > 0)
    { memcpy(st->K,key,keylen);
      st->keylen = keylen;
      md6_reverse_little_endian(st->K,8);
    }
  else
    st->keylen = 0;
  if ( (L<0) | (L>255) ) return 16;
  st->L = L;
  if ( (r<0) | (r>255) ) return 17;
  st->r = r;
  st->initialized = 1;
  st->top = 1;
  if (L==0) st->bits[1] = 16*64;
  compression_hook = ((void *)0);
  return 0;
}
int md6_init( md6_state *st,
       int d
       )
{ return md6_full_init(st,
         d,
         ((void *)0),
         0,
         64,
         md6_default_r(d,0)
         );
}
int md6_compress_block( md6_word *C,
   md6_state *st,
   int ell,
   int z
   )
{ int p, err;
  if ( st == ((void *)0)) return 3;
  if ( st->initialized == 0 ) return 5;
  if ( ell < 0 ) return 6;
  if ( ell >= 29 -1 ) return 7;
  st->compression_calls++;
  if (ell==1)
    { if (ell<(st->L + 1))
 md6_reverse_little_endian(&(st->B[ell][0]),64);
      else
 md6_reverse_little_endian(&(st->B[ell][16]),64 -16);
    }
  p = 64*64 - st->bits[ell];
  err =
    md6_standard_compress(
      C,
      Q,
      st->K,
      ell, st->i_for_level[ell],
      st->r, st->L, z, p, st->keylen, st->d,
      st->B[ell]
      );
  if (err) return err;
  st->bits[ell] = 0;
  st->i_for_level[ell]++;
  bzero(&(st->B[ell][0]),64*sizeof(md6_word));
  return 0;
}
int md6_process( md6_state *st,
   int ell,
   int final )
{ int err, z, next_level;
  md6_word C[16];
  if ( st == ((void *)0)) return 3;
  if ( st->initialized == 0 ) return 5;
  if (!final)
    {
      if ( st->bits[ell] < 64*64 )
 return 0;
    }
  else
    { if ( ell == st->top )
 { if (ell == (st->L + 1))
     { if ( st->bits[ell]==16*64 && st->i_for_level[ell]>0 )
  return 0;
     }
           else
      { if ( ell>1 && st->bits[ell]==16*64)
   return 0;
      }
 }
    }
  z = 0; if (final && (ell == st->top)) z = 1;
  if ((err = md6_compress_block(C,st,ell,z)))
      return err;
  if (z==1)
    { memcpy( st->hashval, C, 16*(64/8) );
      return 0;
    }
  next_level = ((ell+1)<(st->L+1)? (ell+1) : (st->L+1));
  if (next_level == st->L + 1
      && st->i_for_level[next_level]==0
      && st->bits[next_level]==0 )
    st->bits[next_level] = 16*64;
  memcpy((char *)st->B[next_level] + st->bits[next_level]/8,
  C,
  16*(64/8));
  st->bits[next_level] += 16*64;
  if (next_level > st->top) st->top = next_level;
  return md6_process(st,next_level,final);
}
int md6_update( md6_state *st,
  unsigned char *data,
  uint64_t databitlen )
{ unsigned int j, portion_size;
  int err;
  if ( st == ((void *)0) ) return 3;
  if ( st->initialized == 0 ) return 5;
  if ( data == ((void *)0) ) return 8;
  j = 0;
  while (j<databitlen)
    {
      portion_size = ((databitlen-j)<((unsigned int)(64*64 -(st->bits[1])))? (databitlen-j) : ((unsigned int)(64*64 -(st->bits[1]))));
      if ((portion_size % 8 == 0) &&
   (st->bits[1] % 8 == 0) &&
   (j % 8 == 0))
 {
   memcpy((char *)st->B[1] + st->bits[1]/8,
   &(data[j/8]),
   portion_size/8);
 }
      else
 { append_bits((unsigned char *)st->B[1],
        st->bits[1],
        &(data[j/8]),
        portion_size);
 }
      j += portion_size;
      st->bits[1] += portion_size;
      st->bits_processed += portion_size;
      if (st->bits[1] == 64*64 && j<databitlen)
 { if ((err=md6_process(st,
          1,
          0
          )))
     return err;
 }
    }
  return 0;
}
int md6_compute_hex_hashval( md6_state *st )
{ int i;
  static const unsigned char hex_digits[] = "0123456789abcdef";
  if ( st == ((void *)0) ) return 3;
  for (i=0;i<((st->d+7)/8);i++)
    { st->hexhashval[2*i]
 = hex_digits[ ((st->hashval[i])>>4) & 0xf ];
      st->hexhashval[2*i+1]
 = hex_digits[ (st->hashval[i]) & 0xf ];
    }
  st->hexhashval[(st->d+3)/4] = 0;
  return 0;
}
void trim_hashval(md6_state *st)
{
  int full_or_partial_bytes = (st->d+7)/8;
  int bits = st->d % 8;
  int i;
  for ( i=0; i<full_or_partial_bytes; i++ )
    st->hashval[i] = st->hashval[16*(64/8)-full_or_partial_bytes+i];
  for ( i=full_or_partial_bytes; i<16*(64/8); i++ )
    st->hashval[i] = 0;
  if (bits>0)
    { for ( i=0; i<full_or_partial_bytes; i++ )
 { st->hashval[i] = (st->hashval[i] << (8-bits));
   if ( (i+1) < 16*(64/8) )
     st->hashval[i] |= (st->hashval[i+1] >> bits);
 }
    }
}
int md6_final( md6_state *st , unsigned char *hashval)
{ int ell, err;
  if ( st == ((void *)0)) return 3;
  if ( st->initialized == 0 ) return 5;
  if ( st->finalized == 1 ) return 0;
  if (st->top == 1) ell = 1;
  else for (ell=1; ell<=st->top; ell++)
  if (st->bits[ell]>0) break;
  err = md6_process(st,ell,1);
  if (err) return err;
  md6_reverse_little_endian( (md6_word*)st->hashval, 16 );
  trim_hashval( st );
  if (hashval != ((void *)0)) memcpy( hashval, st->hashval, (st->d+7)/8 );
  md6_compute_hex_hashval( st );
  st->finalized = 1;
  return 0;
}
int md6_full_hash( int d,
     unsigned char *data,
     uint64_t databitlen,
     unsigned char *key,
     int keylen,
     int L,
     int r,
     unsigned char *hashval
     )
{ md6_state st;
  int err;
  err = md6_full_init(&st,d,key,keylen,L,r);
  if (err) return err;
  err = md6_update(&st,data,databitlen);
  if (err) return err;
  md6_final(&st,hashval);
  if (err) return err;
  return 0;
}
int md6_hash( int d,
              unsigned char *data,
       uint64_t databitlen,
       unsigned char *hashval
      )
{ int err;
  err = md6_full_hash(d,data,databitlen,
        ((void *)0),0,64,md6_default_r(d,0),hashval);
  if (err) return err;
  return 0;
}
typedef unsigned char BitSequence;
typedef unsigned long long DataLength;
typedef enum { SUCCESS = 0, FAIL = 1, BAD_HASHLEN = 2 } HashReturn;
typedef md6_state hashState;
HashReturn Init( hashState *state,
   int hashbitlen
   );
HashReturn Update( hashState *state,
     const BitSequence *data,
     DataLength databitlen
     );
HashReturn Final( hashState *state,
    BitSequence *hashval
    );
HashReturn Hash( int hashbitlen,
   const BitSequence *data,
   DataLength databitlen,
   BitSequence *hashval
   );
HashReturn Init( hashState *state,
   int hashbitlen
   )
{ int err;
  if ((err = md6_init( (md6_state *) state,
         hashbitlen
         )))
    return err;
  state->hashbitlen = hashbitlen;
  return SUCCESS;
}
HashReturn Update( hashState *state,
     const BitSequence *data,
     DataLength databitlen
     )
{ return md6_update( (md6_state *) state,
       (unsigned char *)data,
       (uint64_t) databitlen );
}
HashReturn Final( hashState *state,
    BitSequence *hashval
    )
{ return md6_final( (md6_state *) state,
      (unsigned char *) hashval
      );
}
HashReturn Hash( int hashbitlen,
   const BitSequence *data,
   DataLength databitlen,
   BitSequence *hashval
   )
{ int err;
  md6_state state;
  if ((err = Init( &state, hashbitlen )))
    return err;
  if ((err = Update( &state, data, databitlen )))
    return err;
  return Final( &state, hashval );
}
typedef struct {
int d;
int L;
int r;
int use_default_r;
unsigned char K[100];
int keylen;
md6_state st;
} args;
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
  if ((err=md6_final(&a->st,((void *)0))))
    {
   writes("Error\n");
  exit(err);
    }
}
MAIN{
 args a;
  a.d = 512;
  a.keylen = 0;
  a.L = 64;
  a.r = md6_default_r(a.d,a.keylen);
  a.use_default_r = 1;
 char buf[1024];
 int r;
 hash_init(&a);
 while ( (r=nread(0,(char*)buf,1024)) ){
  hash_update(&a,buf,r*8);
 }
 hash_final(&a);
 write(1,a.st.hexhashval,a.d/4);
 write(1,"\n",1);
 exit(0);
}
typedef long jmp_buf[8];
typedef unsigned int ml_size_t;
typedef int ml_off_t;
typedef unsigned int uint32_t;
typedef unsigned char uint8_t;
typedef unsigned long size_t;
typedef int bool;
inline void __attribute__((noreturn,always_inline))__ABORT(){
 asm("xor %esp,%esp\nxor %ebp,%ebp\nret");
 __builtin_unreachable();
}
typedef struct _text_match { char *pos; int len; } text_match;
typedef int FILE;
typedef union { FILE *F; int i, fd;} mfl_union;
typedef struct { int quot; int rem; } div_t;
typedef struct { long int quot; long int rem; } ldiv_t;
typedef void (*sighandler_t)(int);
typedef void (functionp)(void);
typedef struct {
  int tz_minuteswest;
  int tz_dsttime;
} timezone;
typedef unsigned long fd_mask;
typedef struct {
 unsigned long fds_bits[1024 / 8 / sizeof(long)];
} fd_set;
typedef __builtin_va_list va_list;
enum EPOLL_EVENTS { __EPOLL_DUMMY };
typedef union epoll_data {
    void *ptr;
     int fd;
    int u32;
    long u64;
} epoll_data_t;
struct epoll_event {
    int events;
     epoll_data_t data;
} __attribute__ ((__packed__));
typedef long time_t;
typedef long suseconds_t;
typedef unsigned useconds_t;
struct timeval { time_t tv_sec; suseconds_t tv_usec; };
struct itimerval {
  struct timeval it_interval;
  struct timeval it_value;
};
typedef unsigned char uchar;
typedef int wchar_t;
typedef long double double_t;
typedef float float_t;
typedef struct { long long __ll; long double __ld; } max_align_t;
typedef struct { union { int __i[14]; volatile int __vi[14]; unsigned long __s[7]; } __u; } pthread_attr_t;
typedef struct { union { int __i[10]; volatile int __vi[10]; volatile void *volatile __p[5]; } __u; } pthread_mutex_t;
typedef struct { union { int __i[10]; volatile int __vi[10]; volatile void *volatile __p[5]; } __u; } mtx_t;
typedef struct { union { int __i[12]; volatile int __vi[12]; void *__p[6]; } __u; } pthread_cond_t;
typedef struct { union { int __i[12]; volatile int __vi[12]; void *__p[6]; } __u; } cnd_t;
typedef struct { union { int __i[14]; volatile int __vi[14]; void *__p[7]; } __u; } pthread_rwlock_t;
typedef struct { union { int __i[8]; volatile int __vi[8]; void *__p[4]; } __u; } pthread_barrier_t;
typedef unsigned long uintptr_t;
typedef long ptrdiff_t;
typedef long ssize_t;
typedef long intptr_t;
typedef long regoff_t;
typedef long register_t;
typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;
typedef signed long int64_t;
typedef signed long intmax_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long uint64_t;
typedef unsigned long u_int64_t;
typedef unsigned long uintmax_t;
typedef unsigned mode_t;
typedef unsigned long nlink_t;
typedef long off_t;
typedef unsigned long ino_t;
typedef unsigned long dev_t;
typedef long blksize_t;
typedef long blkcnt_t;
typedef unsigned long fsblkcnt_t;
typedef unsigned long fsfilcnt_t;
typedef unsigned wint_t;
typedef unsigned long wctype_t;
typedef void * timer_t;
typedef int clockid_t;
typedef long clock_t;
struct timespec { time_t tv_sec; long tv_nsec; };
typedef int pid_t;
typedef unsigned id_t;
typedef unsigned uid_t;
typedef unsigned gid_t;
typedef int key_t;
typedef struct __mbstate_t { unsigned __opaque1, __opaque2; } mbstate_t;
typedef struct __locale_struct * locale_t;
struct iovec { void *iov_base; long iov_len; };
typedef unsigned socklen_t;
typedef unsigned short sa_family_t;
struct winsize {
 unsigned short ws_row;
 unsigned short ws_col;
 unsigned short ws_xpixel;
 unsigned short ws_ypixel;
};
struct stat {
  dev_t st_dev;
  ino_t st_ino;
  nlink_t st_nlink;
  mode_t st_mode;
  uid_t st_uid;
  gid_t st_gid;
  unsigned int __pad0;
  dev_t st_rdev;
  off_t st_size;
  blksize_t st_blksize;
  blkcnt_t st_blocks;
  struct timespec st_atime;
  struct timespec st_mtime;
  struct timespec st_ctime;
  long __unused[3];
};
int isalnum_l(int, locale_t);
int isalpha_l(int, locale_t);
int isblank_l(int, locale_t);
int iscntrl_l(int, locale_t);
int isdigit_l(int, locale_t);
int isgraph_l(int, locale_t);
int islower_l(int, locale_t);
int isprint_l(int, locale_t);
int ispunct_l(int, locale_t);
int isspace_l(int, locale_t);
int isupper_l(int, locale_t);
int isxdigit_l(int, locale_t);
int tolower_l(int, locale_t);
int toupper_l(int, locale_t);
typedef int8_t int_fast8_t;
typedef int64_t int_fast64_t;
typedef int8_t int_least8_t;
typedef int16_t int_least16_t;
typedef int32_t int_least32_t;
typedef int64_t int_least64_t;
typedef uint8_t uint_fast8_t;
typedef uint64_t uint_fast64_t;
typedef uint8_t uint_least8_t;
typedef uint16_t uint_least16_t;
typedef uint32_t uint_least32_t;
typedef uint64_t uint_least64_t;
typedef unsigned char u_int8_t;
typedef unsigned short u_int16_t;
typedef unsigned u_int32_t;
typedef char *caddr_t;
typedef unsigned char u_char;
typedef unsigned short u_short, ushort;
typedef unsigned u_int, uint;
typedef unsigned long u_long, ulong;
typedef long long quad_t;
typedef unsigned long long u_quad_t;
struct sockaddr {
 sa_family_t sa_family;
 char sa_data[14];
};
struct itimerspec {
 struct timespec it_interval;
 struct timespec it_value;
};
struct oldold_utsname {
        char sysname[9];
        char nodename[9];
        char release[9];
        char version[9];
        char machine[9];
};
struct old_utsname {
        char sysname[65];
        char nodename[65];
        char release[65];
        char version[65];
        char machine[65];
};
struct new_utsname {
        char sysname[64 + 1];
        char nodename[64 + 1];
        char release[64 + 1];
        char version[64 + 1];
        char machine[64 + 1];
        char domainname[64 + 1];
};
typedef long __band_t;
typedef union sigval {
  int sival_int;
  void *sival_ptr;
} sigval_t;
typedef struct siginfo {
  int32_t si_signo;
  int32_t si_errno;
  int32_t si_code;
  union {
    char __pad[128 - 2*sizeof(int) - sizeof(long)];
    struct {
      pid_t _pid;
      uid_t _uid;
    } _kill;
    struct {
      uint32_t _timer1;
      uint32_t _timer2;
    } _timer;
    struct {
      pid_t _pid;
      uid_t _uid;
      sigval_t _sigval;
    } _rt;
    struct {
      pid_t _pid;
      uid_t _uid;
      int32_t _status;
      clock_t _utime;
      clock_t _stime;
    } _sigchld;
    struct {
      void *_addr;
    } _sigfault;
    struct {
      __band_t _band;
      int32_t _fd;
    } _sigpoll;
  } _sifields;
} siginfo_t;
typedef struct __sigset_t { unsigned long sig; } sigset_t;
struct sigaction {
  void (*sa_handler)(int);
  unsigned long sa_flags;
  void (*restorer)(void);
  sigset_t sa_mask;
};
struct prctl_mm_map {
 uint64_t start_code;
 uint64_t end_code;
 uint64_t start_data;
 uint64_t end_data;
 uint64_t start_brk;
 uint64_t brk;
 uint64_t start_stack;
 uint64_t arg_start;
 uint64_t arg_end;
 uint64_t env_start;
 uint64_t env_end;
 uint64_t *auxv;
 uint32_t auxv_size;
 uint32_t exe_fd;
};
struct tms
  {
    clock_t tms_utime;
    clock_t tms_stime;
    clock_t tms_cutime;
    clock_t tms_cstime;
  };
struct inotify_event {
 int wd;
 uint32_t mask, cookie, len;
 char name[];
};
typedef long long __kernel_long_t;
typedef unsigned long __kernel_ulong_t;
typedef int sig_atomic_t;
typedef void (*__sighandler_t) (int);
typedef __sighandler_t sig_t;
typedef unsigned long __u64;
typedef unsigned int __u32;
typedef signed long __s64;
typedef signed int __s32;
typedef unsigned short __u16;
typedef signed short __s16;
struct statx_timestamp{
  int64_t tv_sec;
  uint32_t tv_nsec;
  int32_t __statx_timestamp_pad1[1];
};
struct statx{
  uint32_t stx_mask;
  uint32_t stx_blksize;
  uint64_t stx_attributes;
  uint32_t stx_nlink;
  uint32_t stx_uid;
  uint32_t stx_gid;
  uint16_t stx_mode;
  uint16_t __statx_pad1[1];
  uint64_t stx_ino;
  uint64_t stx_size;
  uint64_t stx_blocks;
  uint64_t stx_attributes_mask;
  struct statx_timestamp stx_atime;
  struct statx_timestamp stx_btime;
  struct statx_timestamp stx_ctime;
  struct statx_timestamp stx_mtime;
  uint32_t stx_rdev_major;
  uint32_t stx_rdev_minor;
  uint32_t stx_dev_major;
  uint32_t stx_dev_minor;
  uint64_t __statx_pad2[14];
};
enum fsconfig_command {
 FSCONFIG_SET_FLAG = 0,
 FSCONFIG_SET_STRING = 1,
 FSCONFIG_SET_BINARY = 2,
 FSCONFIG_SET_PATH = 3,
 FSCONFIG_SET_PATH_EMPTY = 4,
 FSCONFIG_SET_FD = 5,
 FSCONFIG_CMD_CREATE = 6,
 FSCONFIG_CMD_RECONFIGURE = 7,
};
struct seccomp_data {
 int nr;
 __u32 arch;
 __u64 instruction_pointer;
 __u64 args[6];
};
struct seccomp_notif_sizes {
 __u16 seccomp_notif;
 __u16 seccomp_notif_resp;
 __u16 seccomp_data;
};
struct seccomp_notif {
 __u64 id;
 __u32 pid;
 __u32 flags;
 struct seccomp_data data;
};
struct seccomp_notif_resp {
 __u64 id;
 __s64 val;
 __s32 error;
 __u32 flags;
};
struct rusage {
 struct timeval ru_utime;
 struct timeval ru_stime;
 __kernel_long_t ru_maxrss;
 __kernel_long_t ru_ixrss;
 __kernel_long_t ru_idrss;
 __kernel_long_t ru_isrss;
 __kernel_long_t ru_minflt;
 __kernel_long_t ru_majflt;
 __kernel_long_t ru_nswap;
 __kernel_long_t ru_inblock;
 __kernel_long_t ru_oublock;
 __kernel_long_t ru_msgsnd;
 __kernel_long_t ru_msgrcv;
 __kernel_long_t ru_nsignals;
 __kernel_long_t ru_nvcsw;
 __kernel_long_t ru_nivcsw;
};
struct rlimit {
 __kernel_ulong_t rlim_cur;
 __kernel_ulong_t rlim_max;
};
typedef unsigned long rlim_t;
struct rlimit64 {
 __u64 rlim_cur;
 __u64 rlim_max;
};
struct udiv_t { unsigned int quot, rem; };
extern int errno;
struct tm {
 int tm_sec;
 int tm_min;
 int tm_hour;
 int tm_mday;
 int tm_mon;
 int tm_year;
 int tm_wday;
 int tm_yday;
 int tm_isdst;
 long __tm_gmtoff;
 const char *__tm_zone;
};
struct __dirstream{
 off_t tell;
 int fd;
 int buf_pos;
 int buf_end;
 int align;
 char buf[0];
};
typedef struct __dirstream DIR;
struct dirent {
 ino_t d_ino;
 off_t d_off;
 unsigned short d_reclen;
 char d_name[];
};
typedef struct {char c;} __guarded_memory;
typedef unsigned int index_t;
typedef unsigned int brk_data_t;
struct ml_malloc_area{
 void *pos;
 unsigned short int size;
 unsigned short int free;
 unsigned short int freegaps;
 unsigned short int largestfree;
};
struct ml_malloc_def;
struct ml_malloc_freearray{
  unsigned int pos;
  unsigned int largestfree;
  unsigned int data[0];
  unsigned int size;
  unsigned int freegaps;
  unsigned int _data[];
};
struct ml_malloc_def{
 struct ml_malloc_area *area;
 struct ml_malloc_freestorage* free;
 void* (*find_free)(struct ml_malloc_def*,int size);
 int (*resize_area)(struct ml_malloc_area*,int size);
 int (*check_area)(struct ml_malloc_area*);
 const int movposfactor;
 const int typesize;
 const int btval,btfree,btprevfree,bttop;
};
typedef struct ml_malloc_area ml_malloc_area_t;
static void ml_ewriteui(unsigned int);
static void ewriteui(unsigned int i){
 ml_ewriteui(i);
}
static void __attribute__((noreturn))__ERR(int errnum, const char* func );
static inline void __attribute__((noreturn,always_inline))exit(int ret){
 asm("jmp __exit"::"D"(ret));
 __builtin_unreachable();
}
typedef struct {
 int tm_sec;
 int tm_min;
 int tm_hour;
 int tm_mday;
 int tm_mon;
 int tm_year;
 int tm_wday;
 int tm_yday;
 int tm_isdst;
} tm;
typedef struct {
  time_t actime;
  time_t modtime;
} utimbuf;
typedef struct {
  int errno;
  int sysret;
  int srand;
  int pstream;
  int mbufsize;
  int _intbufsize;
  int align[1];
  char **environ;
  unsigned long brk;
  unsigned long brk_start;
  void *appdata;
  int stream[16];
} __globals_shadow;
typedef struct {
 union {
  char __pmlgl[sizeof(__globals_shadow)];
  struct {
  int errno;
  int sysret;
  int srand;
  int pstream;
  struct ml_malloc_freearray *freearray;
  int *stacktop ; char **environ ; unsigned long brk ; unsigned long brk_start ;
  unsigned long cbrk;
  unsigned int largestfree;
  unsigned int freeingaps;
  unsigned int brk_preallocate;
  void *_globalappdata;
  int *stream;
  int _stream[16];
  };};
} minilib_globals;
static volatile void __attribute__((noipa,cold)) optimization_fence(void*p,...){}
static inline __seg_fs minilib_globals* __attribute__((always_inline))mlgl(){
 return(0);
}
extern void volatile __attribute__((naked,noipa,cold)) opt_fence(void*p,...);
asm(".global opt_fence\nopt_fence:\nretq");
struct stat;
struct timeval;
struct timezone;
typedef unsigned long u64;
typedef unsigned int u32;
typedef long aio_context_t;
typedef long key_serial_t;
typedef long loff_t;
typedef long qid_t;
typedef long mqd_t;
typedef long stack_t;
typedef long umode_t;
typedef long cap_user_data_t;
typedef long cap_user_header_t;
typedef long semun_u;
struct sockaddr;
struct iovcc;
struct rusage;
struct file_handle;
struct utimbuf;
struct sched_attr;
struct sched_param;
struct itimerspec;
struct kexec_segment;
struct task_struct;
struct msghdr;
struct iocb;
struct io_event;
struct linux_dirent64;
struct pt_regs;
struct perf_event_attr;
struct __sysctl_args;
struct timex;
struct msgbuf;
struct pollfd;
struct getcpu_cache;
struct bpf_attr;
struct shmid_ds;
struct epoll_event;
struct mq_attr;
struct poll_fd;
struct mq_attr;
struct sembuf;
struct statfs;
struct sysinfo;
struct rlimit;
struct ustat;
struct itimerval;
struct linux_dirent;
struct robust_list_head;
struct sigevent;
struct siginfo;
struct bpf_attr;
struct msqid_ds;
struct rlimit64;
struct sigaction;
struct mmsghdr;
struct msqid_ds;

























































;






















































#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wint-conversion"
static inline int volatile __attribute__((always_inline)) arch_prctl( int a1, void *a2 ){ int sysret; asm volatile ("syscall" : "=a" (sysret) : "a" ( (( 158 | 0 ) | 0 ) ) , "D" (a1) , "S" (a2) : "memory","rcx", "r11" ); return( sysret ); }
static inline int volatile __attribute__((always_inline)) write( int a1, const void *a2, int a3 ){ int sysret; asm volatile ("syscall" : "=a" (sysret) : "a" ( (( 1 | 0 ) | 0 ) ) , "D" (a1) , "S" (a2) , "d" (a3) : "memory","rcx", "r11" ); return( sysret ); }
#pragma GCC diagnostic pop
struct ml_malloc_freearray* ml_freearray_new();
static long getbrk();
static int nread(int fd, char *buf, int len);
brk_data_t* ml_shrink(unsigned int diff, index_t e);
static void free(void *m);
void* ml_attach(unsigned int *size, index_t tolower);
index_t ml_join(index_t lower, index_t upper, unsigned int *newsize);
static int rwfd(const int syscallnumber, int fd, char *buf, int len);
static int ml_malloc_brk_addmem(void* brkmin);
static void* __attribute__((optimize("O0"))) mmap(void* addr, ml_size_t len, int prot, int flags, int fd, ml_off_t off);
static long sys_brk(unsigned long addr);
index_t ml_find(unsigned int size,void **addr,unsigned int *esize );
static void* memcpy( void*d, const void *s, ml_size_t n );
static int brk( const void* addr );
ml_size_t ml_remove(index_t index);
index_t ml_add(void *addr, unsigned int size);
void ml_expand(unsigned int diff, index_t uppertomove);
static void* malloc(ml_size_t size);
static void ml_ewriteui(unsigned int ui);
static void* memcpy8( void*d, const void *s, ml_size_t n );
static inline void bzero(void *s, int n);
static inline void *memset( void *s, int c, ml_size_t n);
static void* calloc(ml_size_t nmemb, ml_size_t size);
void __attribute__((naked,noreturn))_start(){
__asm__ volatile("#.global _start\n#_start:\n xorl %ebp, %ebp\n	popq %rdi\n	movq %rsp,%rsi\n	movq %rdi,%rax\n	leaq  8(%rsi,%rdi,8),%rdx\n"
 "call _startup\n"
 "xor %esp,%esp\nret"
 );
};
static void setmlgl(minilib_globals* ml){
 arch_prctl(0x1002,ml);
}
minilib_globals __mlgl;
int main();
void __attribute__((noreturn))_startup(int argc, char **argv, char **envp ){
setmlgl(&__mlgl);
minilib_globals *pmlgl = &__mlgl;
 int a = 0;
 getbrk();
 mlgl()->brk_start = mlgl()->brk;
 mlgl()->cbrk=mlgl()->brk;
 mlgl()->largestfree=0;
 mlgl()->freeingaps=0;
 mlgl()->brk_preallocate = 8192;
 mlgl()->freearray = ml_freearray_new();
 int i = 0;
 pmlgl->stream = pmlgl->_stream;
for (; i<3; i++ )
 mlgl()->stream[i]=i;
mlgl()->pstream = i;
int ret = 0;
mlgl()->stacktop = &ret;
asm("jmp 1f");
goto __NOOPT;
__NOOPT2:
asm("1:");
 ret = main(argc,argv,envp);
__NOOPT:
 asm volatile (".global __exit\n__exit:\nmovq $60,%%rax\nsyscall\n"::"D"(ret));
 opt_fence((void*)pmlgl);
 goto __NOOPT2;
}
struct ml_malloc_freearray* ml_freearray_new(){
 struct ml_malloc_freearray *m = (struct ml_malloc_freearray*) mmap(0,4096,1|2,
   0x20|0x02|0x0100,-1,0);
 return(m-4096);
}
ml_size_t ml_remove(index_t index){
 ml_size_t size = mlgl()->freearray->data[index];
 mlgl()->freeingaps -= mlgl()->freearray->data[index];
 *(uint64_t*)(mlgl()->freearray->data+index) = 0;
 if ( index == mlgl()->freearray->pos ){
  mlgl()->freearray->pos -= 2;
  while ( (mlgl()->freearray->data[mlgl()->freearray->pos] == 0) && mlgl()->freearray->freegaps ){
   mlgl()->freearray->freegaps--;
   mlgl()->freearray->pos -= 2;
  }
 } else {
  mlgl()->freearray->freegaps++;
 }
 return(size);
}
index_t ml_join(index_t lower, index_t upper, unsigned int *newsize){
 if ( lower > upper ){
  {lower^=upper;upper^=lower;lower^=upper;};
  mlgl()->freearray->data[lower+1] = mlgl()->freearray->data[upper+1];
 }
 mlgl()->freearray->data[lower] += mlgl()->freearray->data[upper];
 mlgl()->freeingaps += mlgl()->freearray->data[upper];
 *newsize = mlgl()->freearray->data[lower];
 ml_remove(upper);
 return(lower);
}
void* ml_attach(unsigned int *size, index_t tolower){
 mlgl()->freeingaps += *size;
 mlgl()->freearray->data[tolower] += *size;
 *size = mlgl()->freearray->data[tolower];
 return((void*) (unsigned long int)(mlgl()->freearray->data[tolower+1]));
}
void ml_expand(unsigned int diff, index_t uppertomove){
 mlgl()->freeingaps += diff;
 mlgl()->freearray->data[uppertomove]+=diff;
 mlgl()->freearray->data[uppertomove+1]-=diff;
}
brk_data_t* ml_shrink(unsigned int diff, index_t e){
 mlgl()->freeingaps -= diff;
 mlgl()->freearray->data[e] -= diff;
 mlgl()->freearray->data[e+1] += diff;
 return( (brk_data_t*)(ulong)mlgl()->freearray->data[e+1] );
}
brk_data_t* ml_shrink_down(unsigned int diff, index_t e){
 mlgl()->freeingaps -= diff;
 mlgl()->freearray->data[e] -= diff;
 return( (brk_data_t*)(unsigned long int)(mlgl()->freearray->data[e+1]+mlgl()->freearray->data[e]) );
}
ml_size_t ml_get_size(index_t e){
 return(mlgl()->freearray->data[e]);
}
brk_data_t* ml_get_addr(index_t e){
 return((brk_data_t*)(unsigned long int)(mlgl()->freearray->data[e+1]));
}
index_t ml_add(void *addr, unsigned int size){
 if ( (uint32_t)((uint64_t)addr) != (uint64_t)addr ){
  { write(2,"\033[1;31m" "NOMEM5" "\n" "\033[0;38;40m",sizeof("\033[1;31m" "NOMEM5" "\n" "\033[0;38;40m"));exit(14); };
 }
 if ( size > mlgl()->freearray->largestfree )
  mlgl()->freearray->largestfree = size;
 index_t index = mlgl()->freearray->pos+2;
 if ( mlgl()->freearray->freegaps > 0 ){
  brk_data_t* data = mlgl()->freearray->data + 2;
  index >>= 1;
  asm volatile("repne scasq" : "+D"(data), "+c"(index) : "a"(0) : "cc");
  index = (data - mlgl()->freearray->_data);
  if(index>=mlgl()->freearray->pos)
   { write(2,"\033[1;31m" "IND" "\n" "\033[0;38;40m",sizeof("\033[1;31m" "IND" "\n" "\033[0;38;40m"));exit(14); };
  mlgl()->freearray->freegaps--;
 } else {
  mlgl()->freearray->pos+=2;
 }
 mlgl()->freeingaps += size;
 mlgl()->freearray->data[index] = size;
 mlgl()->freearray->data[index+1] = (brk_data_t)((unsigned long int)addr);
 return(index);
}
index_t ml_find(unsigned int size,void **addr,unsigned int *esize ){
 if ( (size > mlgl()->freearray->largestfree) || (size > mlgl()->freeingaps) ){
  return(0);
 }
 index_t _max = 0xffffffffU,*bestfit = &_max,largest=0;
 for ( index_t *i = mlgl()->freearray->_data, *e = (mlgl()->freearray->data+mlgl()->freearray->pos); i<=e; i+=2 ){
  if ( *i == size ){
   bestfit = i;
   break;
  }
  if ( *i > largest )
   largest = *i;
  if ( (*i > size) && (*i <= *bestfit) )
   bestfit = i;
 }
 mlgl()->freearray->largestfree = largest;
 if ( bestfit != &_max ){
  *esize = *bestfit;
  *addr = (void*)(unsigned long int)*(bestfit + 1);
  return ( bestfit - mlgl()->freearray->data );
 }
 return(0);
}
static void ml_ewriteui(unsigned int ui){
 unsigned int _ewriteui( unsigned int i, int digit){
  if ( digit*10 < i )
   i = _ewriteui(i,digit*10);
  char c = '0' + i/digit;
  i -= i/digit * digit;
  write(1,&c,1);
  return(i);
 }
 _ewriteui(ui,1);
}
static long sys_brk(unsigned long addr){
  long ret;
  asm volatile ("syscall" : "=a" (ret) : "a" ( (( 12 | 0 ) | 0 ) ) , "D" (addr) : "memory","rcx", "r11" );
  return(ret);
}
static long getbrk(){
 if ( !mlgl()->brk ){
  mlgl()->brk=sys_brk(0);
 }
 return(mlgl()->brk);
}
static void free(void *m){
 if ( m == 0 )
  return;
 int *i = (int*)m - 1;
}
static void* __attribute__((optimize("O0"))) mmap(void* addr, ml_size_t len, int prot, int flags, int fd, ml_off_t off){
  long ret;
  if ( fd <0 )
    fd=0;
  register long int r10 asm ("r10") = flags ; register long int r8 asm ("r8") = fd ; register long int r9 asm ("r9") = off; asm volatile ("syscall" : "=a" (ret) : "a" ( (( 9 | 0 ) | 0 ) ) , "D" (addr), "S" (len), "d" (prot), "r" (r10), "r" (r8), "r" (r9) : "memory","rcx", "r11" );
  return((void*)ret);
}
static void* memcpy8( void*d, const void *s, ml_size_t n ){
 char *ret = d;
 asm volatile("rep movsq" :
  "+c"(n),"+S"(s), "+D"(d) :: "memory","cc" );
 return(ret);
}
static int nread(int fd, char *buf, int len){
 return(rwfd(0 | 0,fd,buf,len));
}
static int ml_malloc_brk_addmem(void* brkmin){
  mlgl()->brk_preallocate <<=1;
  mlgl()->brk_preallocate += ((mlgl()->brk_preallocate>>19)<<19);
  mlgl()->brk_preallocate = ((mlgl()->brk_preallocate-1) & ((1<<27)-1) )+1;
  if ( brk( (void*)((unsigned long int)(brkmin + (mlgl()->brk_preallocate +4096 -1) ) & (~(4096 -1))) ) < 0 ){
   {write(2,"\033[0;31m" "NOMEM2\n" "\033[0;38;40m""\n",sizeof("\033[0;31m" "NOMEM2\n" "\033[0;38;40m""\n"));};
   return(0);
  }
  return(1);
}
static void __attribute__((noreturn))__ERR(int errnum, const char* func ){
 write(2,"\n (",sizeof("\n ("));
 write(2,func,strlen(func));
 write(2,"), errnum: ",sizeof("), errnum: "));
 ewriteui(errnum);
 write(2,"\n",sizeof("\n"));
 exit(errnum);
}
static inline void bzero(void *s, int n){
 asm volatile("xor %%eax,%%eax\n rep stosb\n" : "+c"(n), "+D"(s) :: "eax", "memory" );
}
static int brk( const void* addr ){
  long newbrk = sys_brk((unsigned long)addr);
  int ret = 0;
  if ( newbrk >0 ){
    mlgl()->brk = newbrk;
    return(0);
  }
  return(ret);
}
static inline void *memset( void *s, int c, ml_size_t n){
 char *ret = s;
 asm __volatile__( "rep stosb": "+D"(s), "+c"(n) : "a"(c) : "memory","cc" );
 return(ret);
}
static void* malloc(ml_size_t size){
 void *addr;
 brk_data_t esize;
 index_t reuse;
 size = ((size-1)& ~(sizeof(unsigned int)-1))+(2*sizeof(unsigned int) );
 if (( reuse = ml_find(size,&addr,&esize) )){
  if ( (esize - size) <= (2*sizeof(unsigned int)) ){
   ml_remove(reuse);
   size = esize;
   *(unsigned int*)(addr+esize) &= (~0x2);
  } else {
   brk_data_t *newaddr = ml_shrink(size,reuse);
   *newaddr = reuse | 0x1;
  }
 } else {
  if ( ( mlgl()->cbrk + size + sizeof(unsigned int) >= mlgl()->brk ) &&
    !ml_malloc_brk_addmem((void*)(mlgl()->cbrk+size+sizeof(unsigned int))) )
   return(0);
  addr = (void*)(mlgl()->cbrk);
  mlgl()->cbrk += size;
  *(unsigned int*)(mlgl()->cbrk) = 0;
 }
 *(unsigned int*)(addr) = size;
 return( addr + sizeof(unsigned int) );
}
static void* calloc(ml_size_t nmemb, ml_size_t size){
 char* p = malloc(size*nmemb);
 if ( !p )
  return(0);
 bzero(p,size*nmemb);
 return(p);
}
static int rwfd(const int syscallnumber, int fd, char *buf, int len){
 char *b = buf;
 char *e = buf+len;
 int ret;
 do {
  asm volatile ("syscall" : "=a" (ret) : "a" ( (syscallnumber | 0 ) ) , "D" (fd), "S" (b), "d" ((e-b)) : "memory","rcx", "r11" );
  if ( ret <= 0 ){
   if ( ret == -11 || ret == -12 || ret == -4 )
    continue;
   return( b-buf ? b-buf : ret );
  }
  b+=ret;
 } while ( b < e );
 return( len );
}
static void* memcpy( void*d, const void *s, ml_size_t n ){
 char *ret = d;
 asm volatile("rep movsb" :
  "+c"(n),"+S"(s), "+D"(d) :: "memory","cc" );
 return(ret);
}
typedef uint64_t md6_word;
extern int md6_default_r( int d,
     int keylen
     );
void md6_main_compression_loop( md6_word *A,
    int r
    );
int md6_compress( md6_word *C,
    md6_word *N,
    int r,
    md6_word *A
                );
typedef uint64_t md6_control_word;
md6_control_word md6_make_control_word( int r,
     int L,
     int z,
     int p,
     int keylen,
     int d
     );
typedef uint64_t md6_nodeID;
md6_nodeID md6_make_nodeID( int ell,
       int i
       );
void md6_pack( md6_word* N,
        const md6_word* Q,
        md6_word* K,
        int ell, int i,
        int r, int L, int z, int p, int keylen, int d,
        md6_word* B
        );
int md6_standard_compress(
        md6_word *C,
 const md6_word *Q,
 md6_word *K,
 int ell, int i,
 int r, int L, int z, int p, int keylen, int d,
 md6_word* B
      );
typedef struct {
  int d;
  int hashbitlen;
  unsigned char hashval[ 16*(64/8) ];
  unsigned char hexhashval[(16*(64/8))+1];
  int initialized;
  uint64_t bits_processed;
  uint64_t compression_calls;
  int finalized;
  md6_word K[ 8 ];
  int keylen;
  int L;
  int r;
  int top;
  md6_word B[ 29 ][ 64 ];
  unsigned int bits[ 29 ];
  uint64_t i_for_level[ 29 ];
} md6_state;
extern int md6_init( md6_state *st,
       int d
       );
extern int md6_full_init( md6_state *st,
     int d,
     unsigned char *key,
     int keylen,
     int L,
     int r
     );
extern int md6_update( md6_state *st,
         unsigned char *data,
         uint64_t databitlen
         );
extern int md6_final( md6_state *st,
        unsigned char *hashval
        );
extern int md6_hash( int d,
       unsigned char *data,
       uint64_t databitlen,
       unsigned char *hashval
       );
extern int md6_full_hash( int d,
     unsigned char *data,
     uint64_t databitlen,
     unsigned char *key,
     int keylen,
     int L,
     int r,
     unsigned char *hashval
     );
void (* compression_hook)(md6_word *C,
     const md6_word *Q,
     md6_word *K,
     int ell,
     int i,
     int r,
     int L,
     int z,
     int p,
     int keylen,
     int d,
     md6_word *N
     );
static const md6_word S0 = (md6_word)0x0123456789abcdefULL;
static const md6_word Smask = (md6_word)0x7311c2812425cfa0ULL;
void md6_main_compression_loop( md6_word* A , int r )
{ md6_word x, S;
  int i,j;
  S = S0;
 void loop_bodyf(int rs, int ls,int step){
      x = S;
      x^= A[i+step-89];
      x^= A[i+step-89];
      x^= A[i+step-17];
      x^= A[i+step-17];
      x^= ( A[i+step-18] & A[i+step-21] );
      x^= ( A[i+step-18] & A[i+step-21] );
      x^= ( A[i+step-31] & A[i+step-67] );
      x^= ( A[i+step-31] & A[i+step-67] );
      x^= (x >> rs);
      x^= (x >> rs);
      A[i+step] = x^ (x << ls);
      A[i+step] = x^ (x << ls);
 }
 static const char lp[16][3] = {
{10,11, 0},
{ 5,24, 1},
{13, 9, 2},
{10,16, 3},
{11,15, 4},
{12, 9, 5},
{ 2,27, 6},
{ 7,15, 7},
{14, 6, 8},
{15, 2, 9},
{ 7,29,10},
{13, 8,11},
{11,15,12},
{ 7, 5,13},
{ 6,31,14},
{12, 9,15} };
  for (j = 0, i = 89; j<r*16; j+=16)
    {
   for ( int a = 0; a<16; a++ )
    loop_bodyf(lp[a][0],lp[a][1],lp[a][2]);
      S = (S << 1)^ (S >> (64 -1)) ^ (S & Smask);
      S = (S << 1)^ (S >> (64 -1)) ^ (S & Smask);
      i += 16;
    }
}
int md6_compress( md6_word *C,
    md6_word *N,
    int r,
    md6_word *A
   )
{ md6_word* A_as_given = A;
  if ( N == 0) return 9;
  if ( C == 0) return 15;
  if ( r<0 || r > 255) return 17;
  if ( A == 0) A = calloc(r*16 +89,sizeof(md6_word));
  if ( A == 0) return 18;
  memcpy( A, N, 89*sizeof(md6_word) );
  md6_main_compression_loop( A, r );
  memcpy( C, A+(r-1)*16 +89, 16*sizeof(md6_word) );
  if ( A_as_given == 0 )
    {
   bzero(A,(r*16 +89)*sizeof(md6_word));
      free(A);
    }
  return 0;
}
md6_control_word md6_make_control_word( int r,
     int L,
     int z,
     int p,
     int keylen,
     int d
     )
{ md6_control_word V;
  V = ( (((md6_control_word) 0) << 60) |
 (((md6_control_word) r) << 48) |
 (((md6_control_word) L) << 40) |
 (((md6_control_word) z) << 36) |
 (((md6_control_word) p) << 20) |
 (((md6_control_word) keylen) << 12 ) |
        (((md6_control_word) d)) );
  return V;
}
md6_nodeID md6_make_nodeID( int ell,
         int i
       )
{ md6_nodeID U;
  U = ( (((md6_nodeID) ell) << 56) |
 ((md6_nodeID) i) );
  return U;
}
void md6_pack( md6_word*N,
        const md6_word* Q,
        md6_word* K,
        int ell, int i,
        int r, int L, int z, int p, int keylen, int d,
        md6_word* B )
{ int j;
  int ni;
  md6_nodeID U;
  md6_control_word V;
  ni = 0;
  for (j=0;j<15;j++) N[ni++] = Q[j];
  for (j=0;j<8;j++) N[ni++] = K[j];
  U = md6_make_nodeID(ell,i);
  memcpy((unsigned char *)&N[ni],
  &U,
  (((64/64)*(64/8))<(sizeof(md6_nodeID))? ((64/64)*(64/8)) : (sizeof(md6_nodeID))));
  ni += (64/64);
  V = md6_make_control_word(
   r,L,z,p,keylen,d);
  memcpy((unsigned char *)&N[ni],
  &V,
  (((64/64)*(64/8))<(sizeof(md6_control_word))? ((64/64)*(64/8)) : (sizeof(md6_control_word))));
  ni += (64/64);
  memcpy(N+ni,B,64*sizeof(md6_word));
}
int md6_standard_compress( md6_word* C,
      const md6_word* Q,
      md6_word* K,
      int ell, int i,
      int r, int L, int z, int p, int keylen, int d,
      md6_word* B
      )
{ md6_word N[89];
  md6_word A[5000];
  if ( (C == 0) ) return 15;
  if ( (B == 0) ) return 10;
  if ( (r<0) | (r>255) ) return 17;
  if ( (L<0) | (L>255) ) return 16;
  if ( (ell < 0) || (ell > 255) ) return 11;
  if ( (p < 0) || (p > 64*64 ) ) return 12;
  if ( (d <= 0) || (d > 16*64/2) ) return 2;
  if ( (K == 0) ) return 13;
  if ( (Q == 0) ) return 14;
  md6_pack(N,Q,K,ell,i,r,L,z,p,keylen,d,B);
  if (compression_hook != 0)
    compression_hook(C,Q,K,ell,i,r,L,z,p,keylen,d,B);
  return md6_compress(C,N,r,A);
}
int md6_default_r( int d ,
                   int keylen )
{ int r;
  r = 40 + (d/4);
  if (keylen>0)
    r = ((80)>(r)? (80) : (r));
  return r;
}
static const md6_word Q[15] =
  {
    0x7311c2812425cfa0ULL,
    0x6432286434aac8e7ULL,
    0xb60450e9ef68b7c1ULL,
    0xe8fb23908d9f06f1ULL,
    0xdd2e76cba691e5bfULL,
    0x0cd0d63b2c30bc41ULL,
    0x1f8ccf6823058f8aULL,
    0x54e5ed5b88e3775dULL,
    0x4ad12aae0a6d6031ULL,
    0x3e7f16bb88222e0dULL,
    0x8af8671d3fb50c2cULL,
    0x995ad1178bd25c31ULL,
    0xc878c1dd04c4b633ULL,
    0x3b72066c7a1552acULL,
    0x0d6f3522631effcbULL,
  };
void md6_detect_byte_order( void )
{
}
md6_word md6_byte_reverse( md6_word x )
{
  x = (x << 32) | (x >> 32);
  x = ((x & ((md6_word)0x0000ffff0000ffffULL)) << 16) | ((x & ~((md6_word)0x0000ffff0000ffffULL)) >> 16);
  x = ((x & ((md6_word)0x00ff00ff00ff00ffULL)) << 8) | ((x & ~((md6_word)0x00ff00ff00ff00ffULL)) >> 8);
  return x;
}
void md6_reverse_little_endian( md6_word *x, int count )
{
  int i;
  if ((1))
    for (i=0;i<count;i++)
      x[i] = md6_byte_reverse(x[i]);
}
void append_bits( unsigned char *dest, unsigned int destlen,
    unsigned char *src, unsigned int srclen )
{ int i, di, accumlen;
  uint16_t accum;
  int srcbytes;
  if (srclen == 0) return;
  accum = 0;
  accumlen = 0;
  if (destlen%8 != 0)
    { accumlen = destlen%8;
      accum = dest[destlen/8];
      accum = accum >> (8-accumlen);
    }
  di = destlen/8;
  srcbytes = (srclen+7)/8;
  for (i=0;i<srcbytes;i++)
    {
      if (i != srcbytes-1)
 { accum = (accum << 8)^ src[i];
 { accum = (accum << 8)^ src[i];
   accumlen += 8;
 }
      else
 { int newbits = ((srclen%8 == 0) ? 8 : (srclen%8));
   accum = (accum << newbits) | (src[i] >> (8-newbits));
   accumlen += newbits;
 }
      while ( ( (i != srcbytes-1) & (accumlen >= 8) ) ||
       ( (i == srcbytes-1) & (accumlen > 0) ) )
 { int numbits = ((8)<(accumlen)? (8) : (accumlen));
   unsigned char bits;
   bits = accum >> (accumlen - numbits);
   bits = bits << (8-numbits);
   bits &= (0xff00 >> numbits);
   dest[di++] = bits;
   accumlen -= numbits;
 }
    }
}
int md6_full_init( md6_state *st,
     int d,
     unsigned char *key,
     int keylen,
     int L,
     int r
     )
{
  if (st == 0) return 3;
  if ( (key != 0) && ((keylen < 0) || (keylen > 8*(64/8))) )
    return 4;
  if ( d < 1 || d > 512 || d > 64*16/2 ) return 2;
  md6_detect_byte_order();
  bzero(st,sizeof(md6_state));
  st->d = d;
  if (key != 0 && keylen > 0)
    { memcpy(st->K,key,keylen);
      st->keylen = keylen;
      md6_reverse_little_endian(st->K,8);
    }
  else
    st->keylen = 0;
  if ( (L<0) | (L>255) ) return 16;
  st->L = L;
  if ( (r<0) | (r>255) ) return 17;
  st->r = r;
  st->initialized = 1;
  st->top = 1;
  if (L==0) st->bits[1] = 16*64;
  compression_hook = 0;
  return 0;
}
int md6_init( md6_state *st,
       int d
       )
{ return md6_full_init(st,
         d,
         0,
         0,
         64,
         md6_default_r(d,0)
         );
}
int md6_compress_block( md6_word *C,
   md6_state *st,
   int ell,
   int z
   )
{ int p, err;
  if ( st == 0) return 3;
  if ( st->initialized == 0 ) return 5;
  if ( ell < 0 ) return 6;
  if ( ell >= 29 -1 ) return 7;
  st->compression_calls++;
  if (ell==1)
    { if (ell<(st->L + 1))
 md6_reverse_little_endian(&(st->B[ell][0]),64);
      else
 md6_reverse_little_endian(&(st->B[ell][16]),64 -16);
    }
  p = 64*64 - st->bits[ell];
  err =
    md6_standard_compress(
      C,
      Q,
      st->K,
      ell, st->i_for_level[ell],
      st->r, st->L, z, p, st->keylen, st->d,
      st->B[ell]
      );
  if (err) return err;
  st->bits[ell] = 0;
  st->i_for_level[ell]++;
  bzero(&(st->B[ell][0]),64*sizeof(md6_word));
  return 0;
}
int md6_process( md6_state *st,
   int ell,
   int final )
{ int err, z, next_level;
  md6_word C[16];
  if ( st == 0) return 3;
  if ( st->initialized == 0 ) return 5;
  if (!final)
    {
      if ( st->bits[ell] < 64*64 )
 return 0;
    }
  else
    { if ( ell == st->top )
 { if (ell == (st->L + 1))
     { if ( st->bits[ell]==16*64 && st->i_for_level[ell]>0 )
  return 0;
     }
           else
      { if ( ell>1 && st->bits[ell]==16*64)
   return 0;
      }
 }
    }
  z = 0; if (final && (ell == st->top)) z = 1;
  if ((err = md6_compress_block(C,st,ell,z)))
      return err;
  if (z==1)
    { memcpy( st->hashval, C, 16*(64/8) );
      return 0;
    }
  next_level = ((ell+1)<(st->L+1)? (ell+1) : (st->L+1));
  if (next_level == st->L + 1
      && st->i_for_level[next_level]==0
      && st->bits[next_level]==0 )
    st->bits[next_level] = 16*64;
  memcpy((char *)st->B[next_level] + st->bits[next_level]/8,
  C,
  16*(64/8));
  st->bits[next_level] += 16*64;
  if (next_level > st->top) st->top = next_level;
  return md6_process(st,next_level,final);
}
int md6_update( md6_state *st,
  unsigned char *data,
  uint64_t databitlen )
{ unsigned int j, portion_size;
  int err;
  if ( st == 0 ) return 3;
  if ( st->initialized == 0 ) return 5;
  if ( data == 0 ) return 8;
  j = 0;
  while (j<databitlen)
    {
      portion_size = ((databitlen-j)<((unsigned int)(64*64 -(st->bits[1])))? (databitlen-j) : ((unsigned int)(64*64 -(st->bits[1]))));
      if ((portion_size % 8 == 0) &&
   (st->bits[1] % 8 == 0) &&
   (j % 8 == 0))
 {
   memcpy((char *)st->B[1] + st->bits[1]/8,
   &(data[j/8]),
   portion_size/8);
 }
      else
 { append_bits((unsigned char *)st->B[1],
        st->bits[1],
        &(data[j/8]),
        portion_size);
 }
      j += portion_size;
      st->bits[1] += portion_size;
      st->bits_processed += portion_size;
      if (st->bits[1] == 64*64 && j<databitlen)
 { if ((err=md6_process(st,
          1,
          0
          )))
     return err;
 }
    }
  return 0;
}
int md6_compute_hex_hashval( md6_state *st )
{ int i;
  static const unsigned char hex_digits[] = "0123456789abcdef";
  if ( st == 0 ) return 3;
  for (i=0;i<((st->d+7)/8);i++)
    { st->hexhashval[2*i]
 = hex_digits[ ((st->hashval[i])>>4) & 0xf ];
      st->hexhashval[2*i+1]
 = hex_digits[ (st->hashval[i]) & 0xf ];
    }
  st->hexhashval[(st->d+3)/4] = 0;
  return 0;
}
void trim_hashval(md6_state *st)
{
  int full_or_partial_bytes = (st->d+7)/8;
  int bits = st->d % 8;
  int i;
  for ( i=0; i<full_or_partial_bytes; i++ )
    st->hashval[i] = st->hashval[16*(64/8)-full_or_partial_bytes+i];
  for ( i=full_or_partial_bytes; i<16*(64/8); i++ )
    st->hashval[i] = 0;
  if (bits>0)
    { for ( i=0; i<full_or_partial_bytes; i++ )
 { st->hashval[i] = (st->hashval[i] << (8-bits));
   if ( (i+1) < 16*(64/8) )
     st->hashval[i] |= (st->hashval[i+1] >> bits);
 }
    }
}
int md6_final( md6_state *st , unsigned char *hashval)
{ int ell, err;
  if ( st == 0) return 3;
  if ( st->initialized == 0 ) return 5;
  if ( st->finalized == 1 ) return 0;
  if (st->top == 1) ell = 1;
  else for (ell=1; ell<=st->top; ell++)
  if (st->bits[ell]>0) break;
  err = md6_process(st,ell,1);
  if (err) return err;
  md6_reverse_little_endian( (md6_word*)st->hashval, 16 );
  trim_hashval( st );
  if (hashval != 0) memcpy( hashval, st->hashval, (st->d+7)/8 );
  md6_compute_hex_hashval( st );
  st->finalized = 1;
  return 0;
}
int md6_full_hash( int d,
     unsigned char *data,
     uint64_t databitlen,
     unsigned char *key,
     int keylen,
     int L,
     int r,
     unsigned char *hashval
     )
{ md6_state st;
  int err;
  err = md6_full_init(&st,d,key,keylen,L,r);
  if (err) return err;
  err = md6_update(&st,data,databitlen);
  if (err) return err;
  md6_final(&st,hashval);
  if (err) return err;
  return 0;
}
int md6_hash( int d,
              unsigned char *data,
       uint64_t databitlen,
       unsigned char *hashval
      )
{ int err;
  err = md6_full_hash(d,data,databitlen,
        0,0,64,md6_default_r(d,0),hashval);
  if (err) return err;
  return 0;
}
typedef unsigned char BitSequence;
typedef unsigned long long DataLength;
typedef enum { SUCCESS = 0, FAIL = 1, BAD_HASHLEN = 2 } HashReturn;
typedef md6_state hashState;
HashReturn Init( hashState *state,
   int hashbitlen
   );
HashReturn Update( hashState *state,
     const BitSequence *data,
     DataLength databitlen
     );
HashReturn Final( hashState *state,
    BitSequence *hashval
    );
HashReturn Hash( int hashbitlen,
   const BitSequence *data,
   DataLength databitlen,
   BitSequence *hashval
   );
HashReturn Init( hashState *state,
   int hashbitlen
   )
{ int err;
  if ((err = md6_init( (md6_state *) state,
         hashbitlen
         )))
    return err;
  state->hashbitlen = hashbitlen;
  return SUCCESS;
}
HashReturn Update( hashState *state,
     const BitSequence *data,
     DataLength databitlen
     )
{ return md6_update( (md6_state *) state,
       (unsigned char *)data,
       (uint64_t) databitlen );
}
HashReturn Final( hashState *state,
    BitSequence *hashval
    )
{ return md6_final( (md6_state *) state,
      (unsigned char *) hashval
      );
}
HashReturn Hash( int hashbitlen,
   const BitSequence *data,
   DataLength databitlen,
   BitSequence *hashval
   )
{ int err;
  md6_state state;
  if ((err = Init( &state, hashbitlen )))
    return err;
  if ((err = Update( &state, data, databitlen )))
    return err;
  return Final( &state, hashval );
}
typedef struct {
int d;
int L;
int r;
int use_default_r;
unsigned char K[100];
int keylen;
md6_state st;
} args;
void hash_init( args* a)
{ int err;
  if ((err=md6_full_init(&a->st,a->d,a->K,a->keylen,a->L,a->r)))
    {
  write(1,"Error\n",sizeof("Error\n"));
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
   write(1,"Error\n",sizeof("Error\n"));
  exit(err);
    }
}
void hash_final(args *a)
{ int err;
  if ((err=md6_final(&a->st,0)))
    {
   write(1,"Error\n",sizeof("Error\n"));
  exit(err);
    }
}
int main(int argc, char *argv[]){
 args a;
  a.d = 512;
  a.keylen = 0;
  a.L = 64;
  a.r = md6_default_r(a.d,a.keylen);
  a.use_default_r = 1;
 char buf[1024];
 int r;
 hash_init(&a);
 while ( (r=nread(0,(char*)buf,1024)) ){
  hash_update(&a,buf,r*8);
 }
 hash_final(&a);
 write(1,a.st.hexhashval,a.d/4);
 write(1,"\n",1);
 exit(0);
}

