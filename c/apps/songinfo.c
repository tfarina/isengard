#include <id3tag.h>

#include <stdlib.h>

int
main(int argc, char **argv)
{
  int errcode = 0; /* success */
  struct id3_file *file;
  struct id3_tag *tag;
  struct id3_frame *frame;
  union id3_field *field;
  enum id3_field_type field_type;
  id3_ucs4_t const *ucs4;
  id3_latin1_t *latin1;
  id3_utf8_t *utf8;

  file = id3_file_open(argv[1], ID3_FILE_MODE_READONLY);
  if (!file)
  {
    errcode = 1;
    goto out;
  }

  tag = id3_file_tag(file);
  if (!tag)
  {
    errcode = 1;
    goto out;
  }

  frame = id3_tag_findframe(tag, ID3_FRAME_TITLE, 0);
  if (!frame)
  {
    errcode = 1;
    goto out;
  }

  field = id3_frame_field(frame, 1);
  if (!field)
  {
    errcode = 1;
    goto out;
  }

  field_type = id3_field_type(field);

  if (ID3_FIELD_TYPE_STRINGLIST == field_type)
  {
    ucs4 = id3_field_getstrings(field, 0);
    if (!ucs4)
    {
      errcode = 1;
      goto out;
    }

    latin1 = id3_ucs4_latin1duplicate(ucs4);
    if (!latin1)
    {
      errcode = 1;
      goto out;
    }

    free(latin1);

    utf8 = id3_ucs4_utf8duplicate(ucs4);
    if (!utf8)
    {
      errcode = 1;
      goto out;
    }

    free(utf8);
  }

out:
  if (file)
    id3_file_close(file);

  return errcode;
}