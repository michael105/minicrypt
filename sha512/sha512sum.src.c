typedef struct crypto_hash_sha512_state {
    uint64_t state[8];
    uint64_t count[2];
    uint8_t buf[128];
} crypto_hash_sha512_state;
static uint64_t
load64_le(const uint8_t src[8])
{
    uint64_t w;
  w = *(ulong*)src;
    return w;
}
static void
store64_le(uint8_t dst[8], uint64_t w)
{
  *(ulong*)dst = w;
}
static uint64_t
load64_be(const uint8_t src[8])
{
  uint64_t w = *((ulong*)src);
  BSWAP(w);
    return w;
}
static void
store64_be(uint8_t dst[8], uint64_t w)
{
  *((ulong*)dst) = w;
  BSWAP(*((ulong*)dst));
}
static void
be64enc_vect(unsigned char *dst, const uint64_t *src, size_t len)
{
    size_t i;
    for (i = 0; i < len / 8; i++) {
        store64_be((dst + i * 8), (src[i]));
    }
}
static void
be64dec_vect(uint64_t *dst, const unsigned char *src, size_t len)
{
    size_t i;
    for (i = 0; i < len / 8; i++) {
        dst[i] = load64_be(src + i * 8);
    }
}
static void
SHA512_Transform(uint64_t *state, const uint8_t block[128], uint64_t W[80],
                 uint64_t S[8])
{
static const uint64_t Krnd[80] = {
    0x428a2f98d728ae22ULL, 0x7137449123ef65cdULL, 0xb5c0fbcfec4d3b2fULL,
    0xe9b5dba58189dbbcULL, 0x3956c25bf348b538ULL, 0x59f111f1b605d019ULL,
    0x923f82a4af194f9bULL, 0xab1c5ed5da6d8118ULL, 0xd807aa98a3030242ULL,
    0x12835b0145706fbeULL, 0x243185be4ee4b28cULL, 0x550c7dc3d5ffb4e2ULL,
    0x72be5d74f27b896fULL, 0x80deb1fe3b1696b1ULL, 0x9bdc06a725c71235ULL,
    0xc19bf174cf692694ULL, 0xe49b69c19ef14ad2ULL, 0xefbe4786384f25e3ULL,
    0x0fc19dc68b8cd5b5ULL, 0x240ca1cc77ac9c65ULL, 0x2de92c6f592b0275ULL,
    0x4a7484aa6ea6e483ULL, 0x5cb0a9dcbd41fbd4ULL, 0x76f988da831153b5ULL,
    0x983e5152ee66dfabULL, 0xa831c66d2db43210ULL, 0xb00327c898fb213fULL,
    0xbf597fc7beef0ee4ULL, 0xc6e00bf33da88fc2ULL, 0xd5a79147930aa725ULL,
    0x06ca6351e003826fULL, 0x142929670a0e6e70ULL, 0x27b70a8546d22ffcULL,
    0x2e1b21385c26c926ULL, 0x4d2c6dfc5ac42aedULL, 0x53380d139d95b3dfULL,
    0x650a73548baf63deULL, 0x766a0abb3c77b2a8ULL, 0x81c2c92e47edaee6ULL,
    0x92722c851482353bULL, 0xa2bfe8a14cf10364ULL, 0xa81a664bbc423001ULL,
    0xc24b8b70d0f89791ULL, 0xc76c51a30654be30ULL, 0xd192e819d6ef5218ULL,
    0xd69906245565a910ULL, 0xf40e35855771202aULL, 0x106aa07032bbd1b8ULL,
    0x19a4c116b8d2d0c8ULL, 0x1e376c085141ab53ULL, 0x2748774cdf8eeb99ULL,
    0x34b0bcb5e19b48a8ULL, 0x391c0cb3c5c95a63ULL, 0x4ed8aa4ae3418acbULL,
    0x5b9cca4f7763e373ULL, 0x682e6ff3d6b2b8a3ULL, 0x748f82ee5defb2fcULL,
    0x78a5636f43172f60ULL, 0x84c87814a1f0ab72ULL, 0x8cc702081a6439ecULL,
    0x90befffa23631e28ULL, 0xa4506cebde82bde9ULL, 0xbef9a3f7b2c67915ULL,
    0xc67178f2e372532bULL, 0xca273eceea26619cULL, 0xd186b8c721c0c207ULL,
    0xeada7dd6cde0eb1eULL, 0xf57d4f7fee6ed178ULL, 0x06f067aa72176fbaULL,
    0x0a637dc5a2c898a6ULL, 0x113f9804bef90daeULL, 0x1b710b35131c471bULL,
    0x28db77f523047d84ULL, 0x32caab7b40c72493ULL, 0x3c9ebe0a15c9bebcULL,
    0x431d67c49c100d4cULL, 0x4cc5d4becb3e42b6ULL, 0x597f299cfc657e2aULL,
    0x5fcb6fab3ad6faecULL, 0x6c44198c4a475817ULL
};
    int i;
    be64dec_vect(W, block, 128);
    memcpy8(S,state,64>>3);
    for (i = 0; i < 80; i += 16) {
     for ( int a=0; a<16; a++ ){
    S[((87 - a)&0x7)] += (({ ulong l = S[((84 - a)&0x7)]; ROR(14,l); l; })^ ({ ulong l = S[((84 - a)&0x7)]; ROR(18,l); l; }) ^ ({ ulong l = S[((84 - a)&0x7)]; ROR(41,l); l; })) + ((S[((84 - a)&0x7)] & (S[((85 - a)&0x7)] ^ S[((86 - a)&0x7)])) ^ S[((86 - a)&0x7)]) + W[a + i] + Krnd[a + i]; S[((83 - a)&0x7)] += S[((87 - a)&0x7)]; S[((87 - a)&0x7)] += (({ ulong l = S[((80 - a)&0x7)]; ROR(28,l); l; }) ^ ({ ulong l = S[((80 - a)&0x7)]; ROR(34,l); l; }) ^ ({ ulong l = S[((80 - a)&0x7)]; ROR(39,l); l; })) + ((S[((80 - a)&0x7)] & (S[((81 - a)&0x7)] | S[((82 - a)&0x7)])) | (S[((81 - a)&0x7)] & S[((82 - a)&0x7)]));;
    S[((87 - a)&0x7)] += (({ ulong l = S[((84 - a)&0x7)]; ROR(14,l); l; })^ ({ ulong l = S[((84 - a)&0x7)]; ROR(18,l); l; }) ^ ({ ulong l = S[((84 - a)&0x7)]; ROR(41,l); l; })) + ((S[((84 - a)&0x7)] & (S[((85 - a)&0x7)] ^ S[((86 - a)&0x7)])) ^ S[((86 - a)&0x7)]) + W[a + i] + Krnd[a + i]; S[((83 - a)&0x7)] += S[((87 - a)&0x7)]; S[((87 - a)&0x7)] += (({ ulong l = S[((80 - a)&0x7)]; ROR(28,l); l; }) ^ ({ ulong l = S[((80 - a)&0x7)]; ROR(34,l); l; }) ^ ({ ulong l = S[((80 - a)&0x7)]; ROR(39,l); l; })) + ((S[((80 - a)&0x7)] & (S[((81 - a)&0x7)] | S[((82 - a)&0x7)])) | (S[((81 - a)&0x7)] & S[((82 - a)&0x7)]));;
   }
        if (i == 64) {
            break;
        }
     for ( int a=0; a<16; a++ ){
        W[i + a + 16] = (({ ulong l = W[i + a + 14]; ROR(19,l); l; })^ ({ ulong l = W[i + a + 14]; ROR(61,l); l; }) ^ (W[i + a + 14] >> 6)) + W[i + a + 9] + (({ ulong l = W[i + a + 1]; ROR(1,l); l; }) ^ ({ ulong l = W[i + a + 1]; ROR(8,l); l; }) ^ (W[i + a + 1] >> 7)) + W[i + a];
        W[i + a + 16] = (({ ulong l = W[i + a + 14]; ROR(19,l); l; })^ ({ ulong l = W[i + a + 14]; ROR(61,l); l; }) ^ (W[i + a + 14] >> 6)) + W[i + a + 9] + (({ ulong l = W[i + a + 1]; ROR(1,l); l; }) ^ ({ ulong l = W[i + a + 1]; ROR(8,l); l; }) ^ (W[i + a + 1] >> 7)) + W[i + a];
   }
    }
    for (i = 0; i < 8; i++) {
        state[i] += S[i];
    }
}
static void
SHA512_Pad(crypto_hash_sha512_state *state, uint64_t tmp64[80 + 8])
{
    unsigned int r;
    unsigned int i;
    r = (unsigned int) ((state->count[1] >> 3) & 0x7f);
  bzero(state->buf+r,112 -r);
  state->buf[r] = 0x80;
    if (r > 111) {
        SHA512_Transform(state->state, state->buf, &tmp64[0], &tmp64[80]);
        bzero8(&state->buf[0], 112<<3);
    }
    be64enc_vect(&state->buf[112], state->count, 16);
    SHA512_Transform(state->state, state->buf, &tmp64[0], &tmp64[80]);
}
int
crypto_hash_sha512_init(crypto_hash_sha512_state *state)
{
    static const uint64_t sha512_initial_state[8] = {
        0x6a09e667f3bcc908ULL, 0xbb67ae8584caa73bULL, 0x3c6ef372fe94f82bULL,
        0xa54ff53a5f1d36f1ULL, 0x510e527fade682d1ULL, 0x9b05688c2b3e6c1fULL,
        0x1f83d9abfb41bd6bULL, 0x5be0cd19137e2179ULL
    };
    state->count[0] = state->count[1] = (uint64_t) 0U;
    memcpy8(state->state,sha512_initial_state,sizeof sha512_initial_state>>3);
    return 0;
}
int
crypto_hash_sha512_update(crypto_hash_sha512_state *state,
                          const unsigned char *in, unsigned long long inlen)
{
    uint64_t tmp64[80 + 8];
    uint64_t bitlen[2];
    unsigned long long i;
    unsigned long long r;
    if (inlen <= 0U) {
        return 0;
    }
    r = (unsigned long long) ((state->count[1] >> 3) & 0x7f);
    bitlen[1] = ((uint64_t) inlen) << 3;
    bitlen[0] = ((uint64_t) inlen) >> 61;
    if ((state->count[1] += bitlen[1]) < bitlen[1]) {
        state->count[0]++;
    }
    state->count[0] += bitlen[0];
    if (inlen < 128 - r) {
    memcpy8(state->buf+r,in,inlen>>3);
        return 0;
    }
    for (i = 0; i < 128 - r; i++) {
        state->buf[r + i] = in[i];
    }
    SHA512_Transform(state->state, state->buf, &tmp64[0], &tmp64[80]);
    in += 128 - r;
    inlen -= 128 - r;
    while (inlen >= 128) {
        SHA512_Transform(state->state, in, &tmp64[0], &tmp64[80]);
        in += 128;
        inlen -= 128;
    }
    inlen &= 127;
    for (i = 0; i < inlen; i++) {
        state->buf[i] = in[i];
    }
    return 0;
}
int
crypto_hash_sha512_final(crypto_hash_sha512_state *state, unsigned char *out)
{
    uint64_t tmp64[80 + 8];
    SHA512_Pad(state, tmp64);
    be64enc_vect(out, state->state, 64);
    bzero8((void *) tmp64,sizeof tmp64>>3);
    bzero8((void *) state,sizeof *state>>3);
    return 0;
}
int
crypto_hash_sha512(unsigned char *out, const unsigned char *in,
                   unsigned long long inlen)
{
    crypto_hash_sha512_state state;
    crypto_hash_sha512_init(&state);
    crypto_hash_sha512_update(&state, in, inlen);
    crypto_hash_sha512_final(&state, out);
    return 0;
}
MAIN{
 uchar buf[4000];
 uchar obuf[64];
 int r;
 crypto_hash_sha512_state ctx;
 crypto_hash_sha512_init(&ctx);
 while ( (r=nread(0,(char*)buf,4000)) ){
  crypto_hash_sha512_update(&ctx,buf,r);
 }
 crypto_hash_sha512_final(&ctx,obuf);
 for ( int a = 0; a<64; a++ ){
  char c = obuf[a];
  buf[2*a +1] = (c&0xf)>9 ? ((c&0xf) + 'a' - 0xa) : (c&0xf) + '0';
  c >>=4;
  buf[2*a] = (c&0xf)>9 ? ((c&0xf) + 'a' - 0xa) : (c&0xf) + '0';
 }
 buf[128] = '\n';
 write(1,buf,129);
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
static inline int volatile __attribute__((always_inline)) write( int a1, const void *a2, int a3 ){ int sysret; asm volatile ("syscall" : "=a" (sysret) : "a" ( (( 1 | 0 ) | 0 ) ) , "D" (a1) , "S" (a2) , "d" (a3) : "memory","rcx", "r11" ); return( sysret ); }
#pragma GCC diagnostic pop
static int nread(int fd, char *buf, int len);
static int rwfd(const int syscallnumber, int fd, char *buf, int len);
static void ml_ewriteui(unsigned int ui);
static void* memcpy8( void*d, const void *s, ml_size_t n );
static inline void bzero(void *s, int n);
static inline void bzero8(void *s, int n);
void __attribute__((naked,noreturn))_start(){
__asm__ volatile("#.global _start\n#_start:\n xorl %ebp, %ebp\n	popq %rdi\n	movq %rsp,%rsi\n	movq %rdi,%rax\n	leaq  8(%rsi,%rdi,8),%rdx\n"
 "call main\n"
 "movq %rax, %rdi\n"
".global __exit\n"
 "__exit:\n"
 "movq $60, %rax\n"
 "syscall\n"
 "xor %esp,%esp\nret"
 );
};
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
static void* memcpy8( void*d, const void *s, ml_size_t n ){
 char *ret = d;
 asm volatile("rep movsq" :
  "+c"(n),"+S"(s), "+D"(d) :: "memory","cc" );
 return(ret);
}
static int nread(int fd, char *buf, int len){
 return(rwfd(0 | 0,fd,buf,len));
}
static inline void bzero(void *s, int n){
 asm volatile("xor %%eax,%%eax\n rep stosb\n" : "+c"(n), "+D"(s) :: "eax", "memory" );
}
static inline void bzero8(void *s, int n){
 asm volatile("xor %%eax,%%eax\n rep stosq\n" : "+c"(n), "+D"(s) :: "eax", "memory" );
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
typedef struct crypto_hash_sha512_state {
    uint64_t state[8];
    uint64_t count[2];
    uint8_t buf[128];
} crypto_hash_sha512_state;
static uint64_t
load64_le(const uint8_t src[8])
{
    uint64_t w;
  w = *(ulong*)src;
    return w;
}
static void
store64_le(uint8_t dst[8], uint64_t w)
{
  *(ulong*)dst = w;
}
static uint64_t
load64_be(const uint8_t src[8])
{
  uint64_t w = *((ulong*)src);
  asm volatile("bswap %0" : "+r"(w) );
    return w;
}
static void
store64_be(uint8_t dst[8], uint64_t w)
{
  *((ulong*)dst) = w;
  asm volatile("bswap %0" : "+r"(*((ulong*)dst)) );
}
static void
be64enc_vect(unsigned char *dst, const uint64_t *src, long len)
{
    long i;
    for (i = 0; i < len / 8; i++) {
        store64_be((dst + i * 8), (src[i]));
    }
}
static void
be64dec_vect(uint64_t *dst, const unsigned char *src, long len)
{
    long i;
    for (i = 0; i < len / 8; i++) {
        dst[i] = load64_be(src + i * 8);
    }
}
static void
SHA512_Transform(uint64_t *state, const uint8_t block[128], uint64_t W[80],
                 uint64_t S[8])
{
static const uint64_t Krnd[80] = {
    0x428a2f98d728ae22ULL, 0x7137449123ef65cdULL, 0xb5c0fbcfec4d3b2fULL,
    0xe9b5dba58189dbbcULL, 0x3956c25bf348b538ULL, 0x59f111f1b605d019ULL,
    0x923f82a4af194f9bULL, 0xab1c5ed5da6d8118ULL, 0xd807aa98a3030242ULL,
    0x12835b0145706fbeULL, 0x243185be4ee4b28cULL, 0x550c7dc3d5ffb4e2ULL,
    0x72be5d74f27b896fULL, 0x80deb1fe3b1696b1ULL, 0x9bdc06a725c71235ULL,
    0xc19bf174cf692694ULL, 0xe49b69c19ef14ad2ULL, 0xefbe4786384f25e3ULL,
    0x0fc19dc68b8cd5b5ULL, 0x240ca1cc77ac9c65ULL, 0x2de92c6f592b0275ULL,
    0x4a7484aa6ea6e483ULL, 0x5cb0a9dcbd41fbd4ULL, 0x76f988da831153b5ULL,
    0x983e5152ee66dfabULL, 0xa831c66d2db43210ULL, 0xb00327c898fb213fULL,
    0xbf597fc7beef0ee4ULL, 0xc6e00bf33da88fc2ULL, 0xd5a79147930aa725ULL,
    0x06ca6351e003826fULL, 0x142929670a0e6e70ULL, 0x27b70a8546d22ffcULL,
    0x2e1b21385c26c926ULL, 0x4d2c6dfc5ac42aedULL, 0x53380d139d95b3dfULL,
    0x650a73548baf63deULL, 0x766a0abb3c77b2a8ULL, 0x81c2c92e47edaee6ULL,
    0x92722c851482353bULL, 0xa2bfe8a14cf10364ULL, 0xa81a664bbc423001ULL,
    0xc24b8b70d0f89791ULL, 0xc76c51a30654be30ULL, 0xd192e819d6ef5218ULL,
    0xd69906245565a910ULL, 0xf40e35855771202aULL, 0x106aa07032bbd1b8ULL,
    0x19a4c116b8d2d0c8ULL, 0x1e376c085141ab53ULL, 0x2748774cdf8eeb99ULL,
    0x34b0bcb5e19b48a8ULL, 0x391c0cb3c5c95a63ULL, 0x4ed8aa4ae3418acbULL,
    0x5b9cca4f7763e373ULL, 0x682e6ff3d6b2b8a3ULL, 0x748f82ee5defb2fcULL,
    0x78a5636f43172f60ULL, 0x84c87814a1f0ab72ULL, 0x8cc702081a6439ecULL,
    0x90befffa23631e28ULL, 0xa4506cebde82bde9ULL, 0xbef9a3f7b2c67915ULL,
    0xc67178f2e372532bULL, 0xca273eceea26619cULL, 0xd186b8c721c0c207ULL,
    0xeada7dd6cde0eb1eULL, 0xf57d4f7fee6ed178ULL, 0x06f067aa72176fbaULL,
    0x0a637dc5a2c898a6ULL, 0x113f9804bef90daeULL, 0x1b710b35131c471bULL,
    0x28db77f523047d84ULL, 0x32caab7b40c72493ULL, 0x3c9ebe0a15c9bebcULL,
    0x431d67c49c100d4cULL, 0x4cc5d4becb3e42b6ULL, 0x597f299cfc657e2aULL,
    0x5fcb6fab3ad6faecULL, 0x6c44198c4a475817ULL
};
    int i;
    be64dec_vect(W, block, 128);
    memcpy8(S,state,64>>3);
    for (i = 0; i < 80; i += 16) {
     for ( int a=0; a<16; a++ ){
    S[((87 - a)&0x7)] += (({ ulong l = S[((84 - a)&0x7)]; asm("ror $""14"",%0" : "+r"(l) ); l; }) ^ ({ ulong l = S[((84 - a)&0x7)]; asm("ror $""18"",%0" : "+r"(l) ); l; }) ^ ({ ulong l = S[((84 - a)&0x7)]; asm("ror $""41"",%0" : "+r"(l) ); l; })) + ((S[((84 - a)&0x7)] & (S[((85 - a)&0x7)] ^ S[((86 - a)&0x7)])) ^ S[((86 - a)&0x7)]) + W[a + i] + Krnd[a + i]; S[((83 - a)&0x7)] += S[((87 - a)&0x7)]; S[((87 - a)&0x7)] += (({ ulong l = S[((80 - a)&0x7)]; asm("ror $""28"",%0" : "+r"(l) ); l; }) ^ ({ ulong l = S[((80 - a)&0x7)]; asm("ror $""34"",%0" : "+r"(l) ); l; }) ^ ({ ulong l = S[((80 - a)&0x7)]; asm("ror $""39"",%0" : "+r"(l) ); l; })) + ((S[((80 - a)&0x7)] & (S[((81 - a)&0x7)] | S[((82 - a)&0x7)])) | (S[((81 - a)&0x7)] & S[((82 - a)&0x7)]));;
   }
        if (i == 64) {
            break;
        }
     for ( int a=0; a<16; a++ ){
        W[i + a + 16] = (({ ulong l = W[i + a + 14]; asm("ror $""19"",%0" : "+r"(l) ); l; }) ^ ({ ulong l = W[i + a + 14]; asm("ror $""61"",%0" : "+r"(l) ); l; }) ^ (W[i + a + 14] >> 6)) + W[i + a + 9] + (({ ulong l = W[i + a + 1]; asm("ror $""1"",%0" : "+r"(l) ); l; }) ^ ({ ulong l = W[i + a + 1]; asm("ror $""8"",%0" : "+r"(l) ); l; }) ^ (W[i + a + 1] >> 7)) + W[i + a];
   }
    }
    for (i = 0; i < 8; i++) {
        state[i] += S[i];
    }
}
static void
SHA512_Pad(crypto_hash_sha512_state *state, uint64_t tmp64[80 + 8])
{
    unsigned int r;
    unsigned int i;
    r = (unsigned int) ((state->count[1] >> 3) & 0x7f);
  bzero(state->buf+r,112 -r);
  state->buf[r] = 0x80;
    if (r > 111) {
        SHA512_Transform(state->state, state->buf, &tmp64[0], &tmp64[80]);
        bzero8(&state->buf[0], 112<<3);
    }
    be64enc_vect(&state->buf[112], state->count, 16);
    SHA512_Transform(state->state, state->buf, &tmp64[0], &tmp64[80]);
}
int
crypto_hash_sha512_init(crypto_hash_sha512_state *state)
{
    static const uint64_t sha512_initial_state[8] = {
        0x6a09e667f3bcc908ULL, 0xbb67ae8584caa73bULL, 0x3c6ef372fe94f82bULL,
        0xa54ff53a5f1d36f1ULL, 0x510e527fade682d1ULL, 0x9b05688c2b3e6c1fULL,
        0x1f83d9abfb41bd6bULL, 0x5be0cd19137e2179ULL
    };
    state->count[0] = state->count[1] = (uint64_t) 0U;
    memcpy8(state->state,sha512_initial_state,sizeof sha512_initial_state>>3);
    return 0;
}
int
crypto_hash_sha512_update(crypto_hash_sha512_state *state,
                          const unsigned char *in, unsigned long long inlen)
{
    uint64_t tmp64[80 + 8];
    uint64_t bitlen[2];
    unsigned long long i;
    unsigned long long r;
    if (inlen <= 0U) {
        return 0;
    }
    r = (unsigned long long) ((state->count[1] >> 3) & 0x7f);
    bitlen[1] = ((uint64_t) inlen) << 3;
    bitlen[0] = ((uint64_t) inlen) >> 61;
    if ((state->count[1] += bitlen[1]) < bitlen[1]) {
        state->count[0]++;
    }
    state->count[0] += bitlen[0];
    if (inlen < 128 - r) {
    memcpy8(state->buf+r,in,inlen>>3);
        return 0;
    }
    for (i = 0; i < 128 - r; i++) {
        state->buf[r + i] = in[i];
    }
    SHA512_Transform(state->state, state->buf, &tmp64[0], &tmp64[80]);
    in += 128 - r;
    inlen -= 128 - r;
    while (inlen >= 128) {
        SHA512_Transform(state->state, in, &tmp64[0], &tmp64[80]);
        in += 128;
        inlen -= 128;
    }
    inlen &= 127;
    for (i = 0; i < inlen; i++) {
        state->buf[i] = in[i];
    }
    return 0;
}
int
crypto_hash_sha512_final(crypto_hash_sha512_state *state, unsigned char *out)
{
    uint64_t tmp64[80 + 8];
    SHA512_Pad(state, tmp64);
    be64enc_vect(out, state->state, 64);
    bzero8((void *) tmp64,sizeof tmp64>>3);
    bzero8((void *) state,sizeof *state>>3);
    return 0;
}
int
crypto_hash_sha512(unsigned char *out, const unsigned char *in,
                   unsigned long long inlen)
{
    crypto_hash_sha512_state state;
    crypto_hash_sha512_init(&state);
    crypto_hash_sha512_update(&state, in, inlen);
    crypto_hash_sha512_final(&state, out);
    return 0;
}
int main(int argc, char *argv[]){
 uchar buf[4000];
 uchar obuf[64];
 int r;
 crypto_hash_sha512_state ctx;
 crypto_hash_sha512_init(&ctx);
 while ( (r=nread(0,(char*)buf,4000)) ){
  crypto_hash_sha512_update(&ctx,buf,r);
 }
 crypto_hash_sha512_final(&ctx,obuf);
 for ( int a = 0; a<64; a++ ){
  char c = obuf[a];
  buf[2*a +1] = (c&0xf)>9 ? ((c&0xf) + 'a' - 0xa) : (c&0xf) + '0';
  c >>=4;
  buf[2*a] = (c&0xf)>9 ? ((c&0xf) + 'a' - 0xa) : (c&0xf) + '0';
 }
 buf[128] = '\n';
 write(1,buf,129);
 exit(0);
}

