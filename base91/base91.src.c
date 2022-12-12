struct basE91 {
 unsigned long queue;
 unsigned int nbits;
 int val;
};
void basE91_init(struct basE91 *);
size_t basE91_encode(struct basE91 *, const void *, size_t, void *);
size_t basE91_encode_end(struct basE91 *, void *);
size_t basE91_decode(struct basE91 *, const void *, size_t, void *);
size_t basE91_decode_end(struct basE91 *, void *);
const unsigned char enctab[91] = {
 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
 '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '!', '#', '$',
 '%', '&', '(', ')', '*', '+', ',', '.', '/', ':', ';', '<', '=',
 '>', '?', '@', '[', ']', '^', '_', '`' , '{', '|', '}', '~', '-'
};
const unsigned char dectab[256] = {
            62, 90 , 63, 64 , 65, 66, 91, 67, 68, 69, 70, 71, 90 , 72, 73,
 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 74, 75, 76, 77, 78, 79,
 80, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 81, 91, 82, 83, 84,
 85, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35 , 36, 37, 38, 39, 40,
 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 86, 87, 88, 89
};
void basE91_init(struct basE91 *b)
{
 b->queue = 0;
 b->nbits = 0;
 b->val = -1;
}
size_t basE91_encode(struct basE91 *b, const void *i, size_t len, void *o)
{
 const unsigned char *ib = i;
 unsigned char *ob = o;
 size_t n = 0;
 while (len--) {
  b->queue |= *ib++ << b->nbits;
  b->nbits += 8;
  if (b->nbits > 13) {
   unsigned int val = b->queue & 8191;
   if (val > 88) {
    b->queue >>= 13;
    b->nbits -= 13;
   } else {
    val = b->queue & 16383;
    b->queue >>= 14;
    b->nbits -= 14;
   }
   ob[n++] = enctab[val % 91];
   ob[n++] = enctab[val / 91];
  }
 }
 return n;
}
size_t basE91_encode_end(struct basE91 *b, void *o)
{
 unsigned char *ob = o;
 size_t n = 0;
 if (b->nbits) {
  ob[n++] = enctab[b->queue % 91];
  if (b->nbits > 7 || b->queue > 90)
   ob[n++] = enctab[b->queue / 91];
 }
 b->queue = 0;
 b->nbits = 0;
 b->val = -1;
 return n;
}
size_t basE91_decode(struct basE91 *b, const void *i, size_t len, void *o)
{
 const unsigned char *ib = i;
 unsigned char *ob = o;
 size_t n = 0;
 unsigned int d;
 while (len--) {
  const uchar *p = ib;
  ib++;
  if ( *p > 126 || *p <=32 ){
   continue;
  }
  d = dectab[*p-33];
  if (d == 91)
   continue;
  if (b->val == -1)
   b->val = d;
  else {
   b->val += d * 91;
   b->queue |= b->val << b->nbits;
   b->nbits += (b->val & 8191) > 88 ? 13 : 14;
   do {
    ob[n++] = b->queue;
    b->queue >>= 8;
    b->nbits -= 8;
   } while (b->nbits > 7);
   b->val = -1;
  }
 }
 return n;
}
size_t basE91_decode_end(struct basE91 *b, void *o)
{
 unsigned char *ob = o;
 size_t n = 0;
 if (b->val != -1)
  ob[n++] = b->queue | b->val << b->nbits;
 b->queue = 0;
 b->nbits = 0;
 b->val = -1;
 return n;
}
void encode(int fd, uint w){
 char buf[4000];
 char obuf[2*4000];
 struct basE91 b;
 int r;
 int ow = 0;
 void output(){
  char *p = obuf;
  while ( w && (r+ow > w) ){
   write(1,p,w-ow);
   write(1,"\n",1);
   r -= w-ow;
   p += w-ow;
   ow=0;
  }
  write( 1, p, r );
  ow = r;
 }
 while ((r=nread(fd,buf,4000))){
  r = basE91_encode(&b,buf,r,obuf);
  output();
 }
 r = basE91_encode_end(&b,obuf );
 output();
 if ( ow )
  write(1,"\n",1);
}
void decode(int fd){
 char buf[4000];
 char obuf[4000];
 struct basE91 b;
 int r;
 basE91_init(&b);
 while ((r=nread(fd,buf,4000))){
  r = basE91_decode(&b,buf,r,obuf);
  write(1,obuf,r);
 }
 r = basE91_decode_end(&b,obuf );
 write(1,obuf,r);
}
int main(int argc, char **argv){
 uint w = 76;
 int d = 0;
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
static int nread(int fd, char *buf, int len){
 return(rwfd(0 | 0,fd,buf,len));
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
struct basE91 {
 unsigned long queue;
 unsigned int nbits;
 int val;
};
void basE91_init(struct basE91 *);
long basE91_encode(struct basE91 *, const void *, long, void *);
long basE91_encode_end(struct basE91 *, void *);
long basE91_decode(struct basE91 *, const void *, long, void *);
long basE91_decode_end(struct basE91 *, void *);
const unsigned char enctab[91] = {
 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
 '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '!', '#', '$',
 '%', '&', '(', ')', '*', '+', ',', '.', '/', ':', ';', '<', '=',
 '>', '?', '@', '[', ']', '^', '_', '`' , '{', '|', '}', '~', '-'
};
const unsigned char dectab[256] = {
            62, 90 , 63, 64 , 65, 66, 91, 67, 68, 69, 70, 71, 90 , 72, 73,
 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 74, 75, 76, 77, 78, 79,
 80, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 81, 91, 82, 83, 84,
 85, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35 , 36, 37, 38, 39, 40,
 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 86, 87, 88, 89
};
void basE91_init(struct basE91 *b)
{
 b->queue = 0;
 b->nbits = 0;
 b->val = -1;
}
long basE91_encode(struct basE91 *b, const void *i, long len, void *o)
{
 const unsigned char *ib = i;
 unsigned char *ob = o;
 long n = 0;
 while (len--) {
  b->queue |= *ib++ << b->nbits;
  b->nbits += 8;
  if (b->nbits > 13) {
   unsigned int val = b->queue & 8191;
   if (val > 88) {
    b->queue >>= 13;
    b->nbits -= 13;
   } else {
    val = b->queue & 16383;
    b->queue >>= 14;
    b->nbits -= 14;
   }
   ob[n++] = enctab[val % 91];
   ob[n++] = enctab[val / 91];
  }
 }
 return n;
}
long basE91_encode_end(struct basE91 *b, void *o)
{
 unsigned char *ob = o;
 long n = 0;
 if (b->nbits) {
  ob[n++] = enctab[b->queue % 91];
  if (b->nbits > 7 || b->queue > 90)
   ob[n++] = enctab[b->queue / 91];
 }
 b->queue = 0;
 b->nbits = 0;
 b->val = -1;
 return n;
}
long basE91_decode(struct basE91 *b, const void *i, long len, void *o)
{
 const unsigned char *ib = i;
 unsigned char *ob = o;
 long n = 0;
 unsigned int d;
 while (len--) {
  const uchar *p = ib;
  ib++;
  if ( *p > 126 || *p <=32 ){
   continue;
  }
  d = dectab[*p-33];
  if (d == 91)
   continue;
  if (b->val == -1)
   b->val = d;
  else {
   b->val += d * 91;
   b->queue |= b->val << b->nbits;
   b->nbits += (b->val & 8191) > 88 ? 13 : 14;
   do {
    ob[n++] = b->queue;
    b->queue >>= 8;
    b->nbits -= 8;
   } while (b->nbits > 7);
   b->val = -1;
  }
 }
 return n;
}
long basE91_decode_end(struct basE91 *b, void *o)
{
 unsigned char *ob = o;
 long n = 0;
 if (b->val != -1)
  ob[n++] = b->queue | b->val << b->nbits;
 b->queue = 0;
 b->nbits = 0;
 b->val = -1;
 return n;
}
void encode(int fd, uint w){
 char buf[4000];
 char obuf[2*4000];
 struct basE91 b;
 int r;
 int ow = 0;
 void output(){
  char *p = obuf;
  while ( w && (r+ow > w) ){
   write(1,p,w-ow);
   write(1,"\n",1);
   r -= w-ow;
   p += w-ow;
   ow=0;
  }
  write( 1, p, r );
  ow = r;
 }
 while ((r=nread(fd,buf,4000))){
  r = basE91_encode(&b,buf,r,obuf);
  output();
 }
 r = basE91_encode_end(&b,obuf );
 output();
 if ( ow )
  write(1,"\n",1);
}
void decode(int fd){
 char buf[4000];
 char obuf[4000];
 struct basE91 b;
 int r;
 basE91_init(&b);
 while ((r=nread(fd,buf,4000))){
  r = basE91_decode(&b,buf,r,obuf);
  write(1,obuf,r);
 }
 r = basE91_decode_end(&b,obuf );
 write(1,obuf,r);
}
int main(int argc, char **argv){
 uint w = 76;
 int d = 0;
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
     write(1,"base91 [-w col] [-d]\n",sizeof("base91 [-w col] [-d]\n"));
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

[01;32mOK[00m
