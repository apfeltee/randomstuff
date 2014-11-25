
struct CfgVars_t {
    const char* key;
    const char* value;
};

extern int main();

int __texit_status;
int __twrites_fd;
int __twrites_len;
const char* __twrites_str;

struct CfgVars_t cfg[] = {
    {"HOME", "/home/helios"},
    {"SHELL", "/opt/userenv/bin/shell"},
    {"EDITOR", "/opt/userenv/bin/editor"},
    {"RCFILE", "/etc/sh/rc"},
    {0, 0},
};

unsigned int tstrlen(const char* s)
{
    int i;
    for(i=0; s[i]; i++);
    return i;
}


void texit(int status)
{
    __texit_status = status;
    __asm__ __volatile__
    (
        "movl $1, %%eax\n\t"
        "movl __texit_status, %%ebx\n\t"
        "int $0x80\n\t"
        ::: "%eax", "%ebx"
    );
}


int twrite(int fd, const char* str, int len)
{
    __twrites_fd = fd;
    __twrites_str = str;
    __twrites_len = len;
    __asm__ __volatile__
    (
        "movl $4, %%eax\n\t"
        "movl __twrites_fd, %%ebx\n\t"
        "movl __twrites_str, %%ecx\n\t"
        "movl __twrites_len, %%edx\n\t"
        "int $0x80 \n\t"
        ::: "%eax", "%ebx", "%ecx", "%edx"
    );
    return __twrites_len;
}

/*
static inline int twrite(int fd, void const *buf, int size)
{
   int r;
   __asm__("int $0x80" : "=a" (r) : "0" (4), "b" (fd), "c" (buf), "d" (size));
   return r;
}
*/

/*
static inline void texit(int status)
{
   __asm__("int $0x80" : : "a" (1), "b" (status));
}
*/

int twrites(int fd, const char* s)
{
    return twrite(fd, s, tstrlen(s));
}

#ifdef STDL
void _start()
{
    int status = 0;
    status = main(0, 0);
    texit(status + 42);
}
#endif /* STDL */


int main(int argc, char* argv[])
{
    unsigned int i;
    for(i=0; cfg[i].key != 0; i++)
    {
        twrites(1, "[");
        twrites(1, cfg[i].key);
        twrites(1, "] = ");
        twrites(1, cfg[i].value);
        twrites(1, "\n");
    }
    return 0;
}
