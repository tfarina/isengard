#include <stdio.h>

#include "third_party/mxml/mxml.h"

int
main(int argc, char **argv)
{
  FILE *fp = NULL;
  mxml_node_t *tree = NULL;
  mxml_node_t *node = NULL;

  fp = fopen("books.xml", "r");
  if (fp == NULL) {
    return -1;
  }

  tree = mxmlLoadFile(NULL, fp, MXML_OPAQUE_CALLBACK);
  fclose(fp);

  if (tree == NULL) {
    fprintf(stderr, "Unable to read xml file '%s'.\n", "books.xml");
    return -1;
  }

  node = mxmlFindElement(tree, tree, "book", NULL, NULL, MXML_DESCEND);
  if (node == NULL) {
    fputs("Unable to find the <book> element in the XML.\n", stderr);
    mxmlDelete(tree);
    return -1;
  }

  mxmlDelete(tree);

  return 0;
}
