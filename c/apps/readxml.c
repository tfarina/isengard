#include <stdio.h>

#include "third_party/mxml/mxml.h"

int main(int argc, char **argv)
{
  FILE *fp = NULL;
  mxml_node_t *xml = NULL;
  mxml_node_t *node = NULL;

  fp = fopen("books.xml", "r");
  if (fp == NULL) {
    return -1;
  }

  xml = mxmlLoadFile(NULL, fp, MXML_OPAQUE_CALLBACK);
  fclose(fp);

  if (xml == NULL) {
    fprintf(stderr, "Unable to read xml file '%s'.\n", "books.xml");
    return -1;
  }

  node = mxmlFindElement(xml, xml, "book", NULL, NULL, MXML_DESCEND);
  if (node == NULL) {
    fputs("Unable to find first <book> element in XML tree.\n", stderr);
    mxmlDelete(xml);
    return -1;
  }

  mxmlDelete(xml);

  return 0;
}
