#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <string.h>

void *reader_thread(void *arg)
{
    int fd;
    char buf[256];
    int ret;

    fd = open("/dev/waitqdev", O_RDWR);

    if (fd < 0) {
        perror("open");
        return NULL;
    }

    printf("Reader: waiting for data...\n");

    ret = read(fd, buf, sizeof(buf)-1);

    if (ret > 0) {
        buf[ret] = '\0';
        printf("Reader received: %s\n", buf);
    }

    close(fd);
    return NULL;
}

void *writer_thread(void *arg)
{
    int fd;

    sleep(3);

    fd = open("/dev/waitqdev", O_RDWR);

    if (fd < 0) {
        perror("open");
        return NULL;
    }

    printf("Writer: sending data\n");

    write(fd,
          "Hello from writer thread",
          strlen("Hello from writer thread"));

    close(fd);

    return NULL;
}

int main(void)
{
    pthread_t reader;
    pthread_t writer;

    pthread_create(&reader, NULL, reader_thread, NULL);
    pthread_create(&writer, NULL, writer_thread, NULL);

    pthread_join(reader, NULL);
    pthread_join(writer, NULL);

    return 0;
}
