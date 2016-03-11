#ifndef ATOMS_H_INCLUDED
#define ATOMS_H_INCLUDED

typedef struct atoms atoms;
struct atoms
{
  int    depth;
  vector list;
};

int   atoms_init(atoms *, int);
int   atoms_parse(atoms *, segment *);
int   atoms_length(atoms *);
struct atom *atoms_get(atoms *atoms, int);
int   atoms_split(segment *, segment *);

#endif /* ATOMS_H_INCLUDED */

