#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <string.h>

int kfd = 0;
FILE *ufp = NULL;
int SIZE = 1024;
void *ptr = NULL;
char ch = 0;
char ubuf[1024] = {0};
int i = 0;

int main(int argc, char *argv[])
{
    kfd = open("/dev/cdev", O_RDONLY);
    if (kfd < 0)
    {
        perror("open");
        return kfd;
    }

    // 用户空间一部分地址自动映射到内核的空间，返回映射到虚拟空间首地址
    ptr = mmap(NULL, SIZE, PROT_READ, MAP_SHARED, kfd, 0);
    if (ptr == MAP_FAILED)
    {
        perror("mmap");
        close(kfd);
        return -1;
    }
    // 打开读文本
    ufp = fopen("./read.txt", "w");
    if (ufp == NULL)
    {
        perror("fopen");
        munmap(ptr, SIZE);
        close(kfd);
        return -1;
    }

    fputs(ptr, ufp);

    if (munmap(ptr, SIZE) == -1)
    {
        perror("munmap");
    }
    fclose(ufp);
    close(kfd);
    return 0;
}
