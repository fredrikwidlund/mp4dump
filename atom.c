#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <dynamic.h>

#include "segment.h"
#include "atoms.h"
#include "atom.h"

atom *atom_new(segment *data, int depth)
{
  atom *atom;

  atom = malloc(sizeof *atom);
  if (!atom)
    return NULL;

  atom_init(atom, data, depth);
  return atom;
}

void atom_init(atom *atom, segment *data, int depth)
{
  atom->data = *data;
  atoms_init(&atom->atoms, depth + 1);
}

int atom_parse(atom *atom)
{
  (void) atom_parse_header(atom);
  switch(atom->type)
    {
    case ATOM_TYPE_FTYP:
      return atom_parse_ftyp(atom);
    case ATOM_TYPE_MVHD:
      return atom_parse_mvhd(atom);
    case ATOM_TYPE_META:
      return atom_parse_meta(atom);
    case ATOM_TYPE_STYP:
      return atom_parse_styp(atom);
    case ATOM_TYPE_SIDX:
      return atom_parse_sidx(atom);
    case ATOM_TYPE_MDAT:
      return atom_parse_mdat(atom);
    case ATOM_TYPE_MOOV:
    case ATOM_TYPE_TRAK:
    case ATOM_TYPE_TRAF:
    case ATOM_TYPE_MDIA:
    case ATOM_TYPE_UDTA:
    case ATOM_TYPE_MINF:
    case ATOM_TYPE_STBL:
    case ATOM_TYPE_MOOF:
    case ATOM_TYPE_ILST:
      return atoms_parse(&atom->atoms, (segment[]){{.base = atom->data.base + atom->header_size, .end = atom->data.end}});
    case ATOM_TYPE_TRUN:
    case ATOM_TYPE_FREE:
      default:
      break;
    }

  return 0;
}

double atom_parse_fixed_point_32(uint32_t number)
{
  return (number >> 16) + ((double) (number & 0xffff) / (1 << 8));
}

double atom_parse_fixed_point_16(uint32_t number)
{
  return (number >> 16) + ((double) (number & 0xffff) / (1 << 8));
}

int atom_parse_header(atom *atom)
{
  atom_header *header;

  header = (atom_header *) atom->data.base;
  atom->type = header->type;
  atom->header_size = be32toh(header->size) == 1 ? 16 : 8;
  (void) printf("%*s", atom->atoms.depth * 2, "");
  (void) printf("[%.*s %lu]\n", 4, (char *) &atom->type, segment_size(&atom->data));

  return 0;
}

int atom_parse_ftyp(atom *atom)
{
  char *label;

  (void) printf("%*s", atom->atoms.depth * 2, "");
  (void) printf("- labels");
  for (label = atom->data.base + atom->header_size; label < atom->data.end; label += 4)
    (void) printf("%c%.*s", label == atom->data.base  + atom->header_size ? ' ' : ',', 4, label);
  (void) printf("\n");
  return 0;
}

int atom_parse_styp(atom *atom)
{
  char *label;

  (void) printf("%*s", atom->atoms.depth * 2, "");
  (void) printf("- labels");
  for (label = atom->data.base + atom->header_size; label < atom->data.end; label += 4)
    (void) printf("%c%.*s", label == atom->data.base  + atom->header_size ? ' ' : ',', 4, label);
  (void) printf("\n");
  return 0;
}

int atom_parse_mdat(atom *atom)
{
  return 0;
}

typedef struct atom_sidx atom_sidx;
struct __attribute__((__packed__)) atom_sidx
{
  uint8_t  v;
  uint8_t  x[3];
  uint32_t id;
  uint32_t timescale;
  uint64_t ept;
  uint64_t offset;
  uint16_t reserved;
  uint16_t ref;
  uint32_t type:1;
  uint32_t size:31;
  uint32_t duration;
  uint32_t sap_type:3;
  uint32_t sap_delta_time:28;
};

int atom_parse_sidx(atom *atom)
{
  atom_sidx *sidx;

  sidx = (atom_sidx *) (atom->data.base + atom->header_size);

  (void) printf("%*s", atom->atoms.depth * 2, "");
  (void) printf("- %u %u %u %lu %lu %u %u\n", sidx->v, be32toh(sidx->id),  be32toh(sidx->timescale), be64toh(sidx->ept), be64toh(sidx->offset),
                be16toh(sidx->reserved), be16toh(sidx->ref));
  if (be16toh(sidx->ref) != 1)
    return -1;
  (void) printf("%*s", atom->atoms.depth * 2, "");
  (void) printf("- %u %u %u %u %u\n", sidx->type, sidx->size, be32toh(sidx->duration), sidx->sap_type, sidx->sap_delta_time);

  return 0;
}

int atom_parse_mvhd(atom *atom)
{
  atom_mvhd_v1 *v1;
  time_t t;
  struct tm tm;
  char created[80], modified[80];

  v1 = (atom_mvhd_v1 *) (atom->data.base + atom->header_size);

  t = be32toh(v1->created) - 2082844800;
  gmtime_r(&t, &tm);
  strftime(created, sizeof created, "%FT%T", &tm);
  t = be32toh(v1->created) - 2082844800;
  gmtime_r(&t, &tm);
  strftime(modified, sizeof modified, "%FT%T", &tm);
  (void) printf("%*s", atom->atoms.depth * 2, "");
  (void) printf("- version %d, created %s, modified %s, scale %u, duration %g, speed %g, volume %g, next track %u\n",
                v1->version,
                created,
                modified,
                be32toh(v1->scale),
                (double) be32toh(v1->duration) / (double) be32toh(v1->scale),
                atom_parse_fixed_point_32(be32toh(v1->speed)),
                atom_parse_fixed_point_16(be16toh(v1->volume)),
                be32toh(v1->next_track_id));
  return 0;
}

int atom_parse_meta(atom *atom)
{
  atom_meta *meta;

  meta = (atom_meta *) (atom->data.base + atom->header_size);
  (void) printf("%*s", atom->atoms.depth * 2, "");
  (void) printf("- version %d\n", meta->version);
  return atoms_parse(&atom->atoms, (segment[]){{.base = atom->data.base + atom->header_size + sizeof *meta, .end = atom->data.end}});
}
