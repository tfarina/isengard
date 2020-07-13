#include <stdio.h>

#include "third_party/mxml/mxml.h"

int main(int argc, char **argv)
{
  mxml_node_t *xml = NULL;
  mxml_node_t *book = NULL;
  mxml_node_t *node = NULL;
  FILE *fp = NULL;

  xml = mxmlNewXML("1.0");

  book = mxmlNewElement(xml, "book");
  node = mxmlNewElement(book, "author");
  mxmlNewText(node, 0, "Gambardella, Matthew");

  node = mxmlNewElement(book, "title");
  mxmlNewText(node, 0, "XML Developer's Guide");

  fp = fopen("books.xml", "w");
  mxmlSaveFile(xml, fp, MXML_NO_CALLBACK);
  fclose(fp);

  mxmlDelete(xml);

  return 0;
}
