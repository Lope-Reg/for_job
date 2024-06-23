#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>

int kfd = 0;
FILE *ufp = NULL;
int SIZE = 1024;
void *ptr = NULL;
char ch = 0;
char ubuf[1024] = {0};
int i = 0;

int main(int argc, char *argv[])
{
    kfd = open("/dev/cdev", O_RDWR);
    if (kfd < 0)
    {
        perror("open");
        return kfd;
    }
    // 用户空间一部分地址自动映射到内核的空间，返回映射到虚拟空间首地址，其物理地址和read.c一致，但虚拟地址不一致
    ptr = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, kfd, 0);
    if (ptr == MAP_FAILED)
    {
        perror("mmap");
        close(kfd);
        return -1;
    }
    
    // 打开写文件把数据写到内核空间
    ufp = fopen("./write.txt", "r");
    if (ufp == NULL)
    {
        perror("fopen");
        munmap(ptr, SIZE);
        close(kfd);
        return -1;
    }

    fgets(ptr, SIZE, ufp);

    if (munmap(ptr, SIZE) == -1)
    {
        perror("munmap");
    }
    fclose(ufp);
    close(kfd);
    return 0;
}
