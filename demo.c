#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#define DEVICE "/dev/mycdev"

int main(int argc, char *argv[])
{
    int fd;
    char *buffer;
    ssize_t bytes_read, bytes_written;
    size_t size;
    FILE *outfile, *infile;

    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <write/read> <data/file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    fd = open(DEVICE, O_RDWR);
    if (fd < 0)
    {
        perror("open");
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "write") == 0)
    {
#if 0
        size = 128; // buffer size for writing
        buffer = (char *)malloc(size);
        
        infile = fopen(argv[2], "r");
        if (!infile)
        {
            perror("fopen");
            free(buffer);
            close(fd);
            return EXIT_FAILURE;
        }

        while(fgets(buffer,sizeof(buffer),infile)){
            bytes_written = write(fd, buffer, size);
            if (bytes_written < 0)
            {
                perror("write");
                free(buffer);
                close(fd);
                return EXIT_FAILURE;
            }
        }

#else

        size = strlen(argv[2]);
        buffer = (char *)malloc(size);
        strcpy(buffer, argv[2]);

        bytes_written = write(fd, buffer, size);
        if (bytes_written < 0)
        {
            perror("write");
            free(buffer);
            close(fd);
            return EXIT_FAILURE;
        }
#endif

        printf("Written %ld bytes to %s\n", bytes_written, DEVICE);
        free(buffer);
    }
    else if (strcmp(argv[1], "read") == 0)
    {
        size = 128; // buffer size for reading
        buffer = (char *)malloc(size);

        bytes_read = read(fd, buffer, size);
        if (bytes_read < 0)
        {
            perror("read");
            free(buffer);
            close(fd);
            return EXIT_FAILURE;
        }

        outfile = fopen(argv[2], "w");
        if (!outfile)
        {
            perror("fopen");
            free(buffer);
            close(fd);
            return EXIT_FAILURE;
        }

        fwrite(buffer, 1, bytes_read, outfile);
        fclose(outfile);

        printf("Read %ld bytes from %s to %s\n", bytes_read, DEVICE, argv[2]);
        free(buffer);
    }
    else
    {
        fprintf(stderr, "Invalid command. Use 'write' or 'read'.\n");
        close(fd);
        return EXIT_FAILURE;
    }

    close(fd);
    return EXIT_SUCCESS;
}
