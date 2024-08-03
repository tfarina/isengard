#include <id3tag.h>

#include <stdlib.h>

int
main(int argc, char **argv)
{
  struct id3_file *file;
  struct id3_tag *tag;
  struct id3_frame *frame;
  union id3_field *field;
  id3_ucs4_t const *ucs4;
  id3_latin1_t *latin1;
  id3_utf8_t *utf8;

  file = id3_file_open(argv[1], ID3_FILE_MODE_READONLY);
  if (!file)
  {
    return 1;
  }

  tag = id3_file_tag(file);
  if (!tag)
  {
    return 1;
  }

  frame = id3_tag_findframe(tag, ID3_FRAME_TITLE, 0);
  if (!frame)
  {
    return 1;
  }

  field = id3_frame_field(frame, 1);
  if (!field)
  {
    return 1;
  }

  ucs4 = id3_field_getstrings(field, 0);
  if (!ucs4)
  {
    return 1;
  }

  latin1 = id3_ucs4_latin1duplicate(ucs4);
  if (!latin1)
  {
    return 1;
  }

  free(latin1);

  utf8 = id3_ucs4_utf8duplicate(ucs4);
  if (!utf8)
  {
    return 1;
  }

  free(utf8);

  id3_file_close(file);

  return 0;
}
