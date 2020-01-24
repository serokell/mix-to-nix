#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/sendfile.h>
#include <sys/stat.h>

#define handle_error(msg)                                     \
  do { perror(msg); exit(EXIT_FAILURE); } while (0)
#define MIN_GZIP_SIZE 18

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s FILE\n", argv[0]);
    return EXIT_FAILURE;
  }
  struct stat sb;
  int fd = open(argv[1], O_RDONLY);
  if (fd == -1)
    handle_error("open");
  if (fstat(fd, &sb) == -1)
    handle_error("fstat");

  char* data = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (data == MAP_FAILED)
    handle_error("mmap");
  size_t ptr = 0;

  while (sb.st_size - MIN_GZIP_SIZE > ptr && data[ptr] != 0x1F &&
         data[ptr + 1] != 0x8B && data[ptr + 2] != 0x08)
    ptr++;
  if (munmap(data, sb.st_size) == -1)
    handle_error("munmap");

  sendfile(1, fd, &ptr, sb.st_size - ptr);
}
