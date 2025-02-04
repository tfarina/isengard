#include <stdio.h>
#include <stdlib.h>
#include <tag_c.h>

int
main(int argc, char **argv)
{
  TagLib_File *file;
  TagLib_Tag *tag;
  const TagLib_AudioProperties *properties;

  file = taglib_file_new(argv[1]);

  if (!file)
    return 1;

  tag = taglib_file_tag(file);
  if (tag) {
    printf("-- TAG (basic) --\n");
    printf("title   - \"%s\"\n", taglib_tag_title(tag));
    printf("artist  - \"%s\"\n", taglib_tag_artist(tag));
    printf("album   - \"%s\"\n", taglib_tag_album(tag));
    printf("year    - \"%u\"\n", taglib_tag_year(tag));
    printf("comment - \"%s\"\n", taglib_tag_comment(tag));
    printf("track   - \"%u\"\n", taglib_tag_track(tag));
    printf("genre   - \"%s\"\n", taglib_tag_genre(tag));
  }

  properties = taglib_file_audioproperties(file);
  if (properties) {
    int seconds = taglib_audioproperties_length(properties) % 60;
    int minutes = (taglib_audioproperties_length(properties) - seconds) / 60;

    printf("-- AUDIO --\n");
    printf("bitrate     - %i\n", taglib_audioproperties_bitrate(properties));
    printf("sample rate - %i\n", taglib_audioproperties_samplerate(properties));
    printf("channels    - %i\n", taglib_audioproperties_channels(properties));
    printf("length      - %i:%02i\n", minutes, seconds);
  }

  taglib_tag_free_strings();
  taglib_file_free(file);

  return 0;
}
