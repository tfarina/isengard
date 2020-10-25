#include <stdio.h>

#include "third_party/mxml/mxml.h"

int
main(int argc, char **argv)
{
  char *xmlfile;
  FILE *fp = NULL;
  mxml_node_t *tree = NULL;
  mxml_node_t *node = NULL;

  xmlfile = "books.xml";

  fp = fopen(xmlfile, "r");
  if (fp == NULL) {
    return -1;
  }

  tree = mxmlLoadFile(NULL, fp, MXML_OPAQUE_CALLBACK);
  fclose(fp);

  if (tree == NULL) {
    fprintf(stderr, "Unable to read the XML file \"%s\".\n", xmlfile);
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
