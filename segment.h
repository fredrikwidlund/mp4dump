#ifndef SEGMENT_H_INCLUDED
#define SEGMENT_H_INCLUDED

typedef struct segment segment;
struct segment
{
  char *base;
  char *end;
};

int    segment_read(segment *, char *);
size_t segment_size(segment *);

#endif /* SEGMENT_H_INCLUDED */
