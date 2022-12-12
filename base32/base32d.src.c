int nread( int fd, char *buf, int len ){
 char *b = buf;
 char *e = buf+len;
 do {
  errno = 0;
  int ret = read(fd,b,(e-b));
  if ( ret <= 0 ){
   if ( errno == EAGAIN || errno == ENOMEM || errno == EINTR )
    continue;
   return( b-buf );
  }
  b+=ret;
 } while ( b < e );
 return( len );
}
int main(int argc, char **argv){
 char buf[2000];
 char obuf[2000];
 int r;
 int len = 0;
 unsigned long l;
 int marge = 64;
 char *bs = buf;
 int err = 0;
 while( marge ) {
  r=nread(0,bs,2000 -marge);
  if ( r<2000 -marge )
   *(long*)((char*)bs+r) = (long)0;
  if ( r == 0 ){
   marge=0;
  }
  char *pbuf = buf;
  char *pobuf = obuf;
  int lb = 0;
  while ( pbuf + marge < bs+r ){
   l = 0;
   for ( int a = 0; a<8; a ++ ){
    while ( *pbuf && ( (*pbuf < '2') || (*pbuf >'Z') ) ){
     pbuf++;
     lb++;
     if ( pbuf > (bs+r) ){
      write(1,"Bad input\n",10);
      err = EINVAL;
      continue;
     }
    }
    char c = *pbuf;
    if ( !(c == '=' || c==0) ){
     if ( c >= 'A' )
      c-='A';
     else
      c -= 24;
     l = ( l << 5 ) | c;
    } else {
     l <<= 5;
     marge = 0;
     if ( ! len )
      len = (( pbuf - buf -lb ) * 5) >> 3;
    }
    pbuf ++;
   }
   asm("bswap %0" : "+r"(l) );
   l >>= 24;
   *(long*)pobuf = l;
   pobuf += 5;
  }
  if ( marge ){
   write(1,obuf,(pobuf-obuf));
   memcpy(buf,pbuf,(bs+r)-pbuf);
   bs = (buf+((bs+r)-pbuf));
  } else {
   write(1,obuf,len );
  }
 };
 exit(err);
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
  int *stacktop ; char **environ ; unsigned long brk ; unsigned long brk_start ;
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
struct sIn file included from [01m[K<stdin>:5[m[K:
[01m[Kbase32d.c:23:[m[K [01;35m[Kwarning: [m[K"BSWAP" redefined
   23 | #define BSWAP(a) asm("bswap %0" : "+r"(a) )
      | 
In file included from [01m[K/home/micha/prog/minilib/minilib.h:90[m[K,
                 from [01m[K<stdin>:4[m[K:
[01m[K/home/micha/prog/minilib/minilib/macros/macrolib.h:14:[m[K [01;36m[K[00;35mnote: [m[Kthis is the location of the previous definition[00m
   14 | #define BSWAP(a) asm volatile("bswap %0" : "+r"(a) )
      | 
ockaddr;
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
static inline int volatile __attribute__((always_inline)) write( int a1, const void *a2, int a3 ){ int sysret; asm volatile ("syscall" : "=a" (sysret) : "a" ( ((1 | 0 ) | 0 ) ) , "D" (a1) , "S" (a2) , "d" (a3) : "memory","rcx", "r11" ); if ( sysret<0){ mlgl()->errno = -sysret; return(-1);} return(sysret); }
#pragma GCC diagnostic pop
static void* memcpy( void*d, const void *s, ml_size_t n );
static void ml_ewriteui(unsigned int ui);
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
 int i = 0;
 pmlgl->stream = pmlgl->_stream;
for (; i<3; i++ )
 mlgl()->stream[i]=i;
mlgl()->pstream = i;
mlgl()->errno = 0;
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
static void* memcpy( void*d, const void *s, ml_size_t n ){
 char *ret = d;
 asm volatile("rep movsb" :
  "+c"(n),"+S"(s), "+D"(d) :: "memory","cc" );
 return(ret);
}
int nread( int fd, char *buf, int len ){
 char *b = buf;
 char *e = buf+len;
 do {
  mlgl()->errno = 0;
  int ret = read(fd,b,(e-b));
  if ( ret <= 0 ){
   if ( mlgl()->errno == 11 || mlgl()->errno == 12 || mlgl()->errno == 4 )
    continue;
   return( b-buf );
  }
  b+=ret;
 } while ( b < e );
 return( len );
}
int main(int argc, char **argv){
 char buf[2000];
 char obuf[2000];
 int r;
 int len = 0;
 unsigned long l;
 int marge = 64;
 char *bs = buf;
 int err = 0;
 while( marge ) {
  r=nread(0,bs,2000 -marge);
  if ( r<2000 -marge )
   *(long*)((char*)bs+r) = (long)0;
  if ( r == 0 ){
   marge=0;
  }
  char *pbuf = buf;
  char *pobuf = obuf;
  int lb = 0;
  while ( pbuf + marge < bs+r ){
   l = 0;
   for ( int a = 0; a<8; a ++ ){
    while ( *pbuf && ( (*pbuf < '2') || (*pbuf >'Z') ) ){
     pbuf++;
     lb++;
     if ( pbuf > (bs+r) ){
      write(1,"Bad input\n",10);
      err = 22;
      continue;
     }
    }
    char c = *pbuf;
    if ( !(c == '=' || c==0) ){
     if ( c >= 'A' )
      c-='A';
     else
      c -= 24;
     l = ( l << 5 ) | c;
    } else {
     l <<= 5;
     marge = 0;
     if ( ! len )
      len = (( pbuf - buf -lb ) * 5) >> 3;
    }
    pbuf ++;
   }
   asm("bswap %0" : "+r"(l) );
   l >>= 24;
   *(long*)pobuf = l;
   pobuf += 5;
  }
  if ( marge ){
   write(1,obuf,(pobuf-obuf));
   memcpy(buf,pbuf,(bs+r)-pbuf);
   bs = (buf+((bs+r)-pbuf));
  } else {
   write(1,obuf,len );
  }
 };
 exit(err);
}

[01;32mOK[00m
