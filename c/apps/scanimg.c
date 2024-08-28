#include <png.h>
#include <stdlib.h>

/* https://github.com/ShadowsocksR-Live/zbar/blob/master/examples/scan_image.c
 */

int
main(
  int argc,
  char **argv)
{
  FILE *fp = NULL;
  png_structp png = NULL;
  png_infop info = NULL;
  png_bytep *rows = NULL;
  png_uint_32 width = 0;
  png_uint_32 height = 0;
  png_uint_32 y = 0;

  if (argc < 2)
  {
    fprintf(stderr, "Usage: scanimg <filename.png>\n");
    return 1;
  }

  fp = fopen(argv[1], "rb");
  if (!fp)
    goto out;

  png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png)
    goto out;

  if (setjmp(png_jmpbuf(png)))
    goto out;

  info = png_create_info_struct(png);
  if (!info)
    goto out;

  png_init_io(png, fp);
  png_read_info(png, info);

  width = png_get_image_width(png, info);
  height = png_get_image_height(png, info);

  rows = malloc(height * sizeof(png_bytep));
  if (!rows)
  {
    /* Out of memory */
    goto out;
  }

  for (y = 0; y < height; y++)
  {
  }

  free(rows);

  return 0;

out:
  png_destroy_read_struct(&png, &info, NULL);

  if (fp)
    fclose(fp);

  return 1;
}
