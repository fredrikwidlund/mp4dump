#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#include "segment.h"

int segment_read(segment *segment, char *path)
{
  struct stat st;
  int e, fd;
  ssize_t n;

  e = stat(path, &st);
  if (e == -1)
    return -1;

  segment->base = malloc(st.st_size);
  if (!segment->base)
    return -1;

  fd = open(path, O_RDONLY);
  if (fd == -1)
    return -1;

  n = read(fd, segment->base, st.st_size);
  (void) close(fd);
  if (n != st.st_size)
    return -1;

  segment->end = segment->base + n;
  return 0;
}

size_t segment_size(segment *segment)
{
  return segment->end - segment->base;
}
