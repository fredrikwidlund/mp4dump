#ifndef ATOM_H_INCLUDED
#define ATOM_H_INCLUDED

#if __BYTE_ORDER == __LITTLE_ENDIAN
#define atom_type(c1, c2, c3, c4) (((c1) << 0) + ((c2) << 8) + ((c3) << 16) + ((c4) << 24))
#elif __BYTE_ORDER == __BIG_ENDIAN
#define atom_type(c1, c2, c3, c4) (((c1) << 24) + ((c2) << 16) + ((c3) << 8) + ((c4) << 0))
#endif

#define atom_type_string(t) ((char[5]){((t) >> 0) & 0xff,0,0,0,0})

enum
{
  ATOM_TYPE_FTYP = atom_type('f', 't', 'y', 'p'),
  ATOM_TYPE_FREE = atom_type('f', 'r', 'e', 'e'),
  ATOM_TYPE_MDAT = atom_type('m', 'd', 'a', 't'),
  ATOM_TYPE_MOOV = atom_type('m', 'o', 'o', 'v'),
  ATOM_TYPE_TRAK = atom_type('t', 'r', 'a', 'k'),
  ATOM_TYPE_MVHD = atom_type('m', 'v', 'h', 'd'),
  ATOM_TYPE_MDIA = atom_type('m', 'd', 'i', 'a'),
  ATOM_TYPE_UDTA = atom_type('u', 'd', 't', 'a'),
  ATOM_TYPE_META = atom_type('m', 'e', 't', 'a'),
  ATOM_TYPE_MINF = atom_type('m', 'i', 'n', 'f'),
  ATOM_TYPE_STBL = atom_type('s', 't', 'b', 'l'),
  ATOM_TYPE_ILST = atom_type('i', 'l', 's', 't')
};

typedef struct atom_header atom_header;
struct atom_header
{
  uint32_t size;
  uint32_t type;
  uint64_t size_extended;
};

typedef struct atom atom;
struct atom
{
  segment  data;
  uint32_t type;
  size_t   header_size;
  atoms    atoms;
};

typedef struct atom_mvhd_v1 atom_mvhd_v1;
struct __attribute__((__packed__)) atom_mvhd_v1
{
  uint8_t  version;
  uint8_t  flags[3];
  uint32_t created;
  uint32_t modified;
  uint32_t scale;
  uint32_t duration;
  uint32_t speed;
  uint16_t volume;
  uint16_t reserved[5];
  uint32_t matrix[9];
  uint64_t quicktime_preview;
  uint32_t quicktime_poster;
  uint64_t quicktime_selection;
  uint32_t quicktime_current_time;
  uint32_t next_track_id;
};

typedef struct atom_meta atom_meta;
struct atom_meta
{
  uint8_t  version;
  uint8_t  flags[3];
};

/*
struct atom_mvhd_v2
{
  uint8_t  version;
  uint8_t  flags[3];
  uint64_t created;
  uint64_t modified;
  uint64_t duration;
};
*/

atom *atom_new(segment *, int);
void  atom_init(atom *, segment *, int);
int   atom_parse(atom *);
int   atom_parse_header(atom *);
int   atom_parse_ftyp(atom *);
int   atom_parse_mvhd(atom *);
int   atom_parse_meta(atom *);

#endif /* ATOM_H_INCLUDED */
