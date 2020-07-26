#include <stdio.h>

#include "third_party/mxml/mxml.h"

int main(int argc, char **argv)
{
  mxml_node_t *xml = NULL;
  mxml_node_t *book = NULL;
  mxml_node_t *author = NULL;
  mxml_node_t *title = NULL;
  FILE *fp = NULL;

  xml = mxmlNewXML("1.0");

  book = mxmlNewElement(xml, "book");

  author = mxmlNewElement(book, "author");
  mxmlNewText(author, 0, "Gambardella, Matthew");

  title = mxmlNewElement(book, "title");
  mxmlNewText(title, 0, "XML Developer's Guide");

  fp = fopen("books.xml", "w");
  if (fp == NULL) {
    return -1;
  }

  mxmlSaveFile(xml, fp, MXML_NO_CALLBACK);

  fclose(fp);

  mxmlDelete(title);
  mxmlDelete(author);
  mxmlDelete(xml);

  return 0;
}
