#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <dynamic.h>

#include "segment.h"
#include "atoms.h"
#include "atom.h"

int atoms_init(atoms *atoms, int depth)
{
  atoms->depth = depth;
  vector_construct(&atoms->list, sizeof(atom *));

  return 0;
}

int atoms_parse(atoms *atoms, segment *data)
{
  segment first, rest;
  atom *atom;
  int e;

  rest = *data;
  while (segment_size(&rest))
    {
      e = atoms_split(&first, &rest);
      if (e == -1)
        return -1;

      atom = atom_new(&first, atoms->depth);
      if (!atom)
        return -1;

      e = atom_parse(atom);
      if (e == -1)
        return -1;

      vector_push_back(&atoms->list, &atom);
    }

  return 0;
}

int atoms_length(atoms *atoms)
{
  return vector_size(&atoms->list);
}

atom *atoms_get(atoms *atoms, int i)
{
  return *(atom **)vector_at(&atoms->list, i);
}

int atoms_split(segment *first, segment *rest)
{
  atom_header *header;
  size_t size;

  if (segment_size(rest) < 8)
    return -1;

  header = (atom_header *) rest->base;
  size = be32toh(header->size);
  if (size == 0)
    size = segment_size(rest);
  else if (size == 1)
    {
      if (segment_size(rest) < 16)
        return -1;
      size = be64toh(header->size_extended);
    }
  if (segment_size(rest) < size)
    return -1;

  first->base = rest->base;
  first->end = first->base + size;
  rest->base += size;

  return 0;
}
