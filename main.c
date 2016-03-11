#include <stdlib.h>
#include <stdint.h>
#include <err.h>
#include <dynamic.h>

#include "segment.h"
#include "atoms.h"
#include "atom.h"

int main(int argc, char **argv)
{
  segment file;
  atoms atoms;
  int e;

  e = segment_read(&file, argv[1]);
  if (e == -1)
    err(1, "segment_read");

  atoms_init(&atoms, 0);
  e = atoms_parse(&atoms, &file);
  if (e == -1)
    err(1, "parse_atoms");
}
