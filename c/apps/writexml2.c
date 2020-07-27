#include <stdio.h>

#include "third_party/mxml/mxml.h"

int main(int argc, char **argv)
{
  mxml_node_t *xml = NULL;
  mxml_node_t *abook = NULL;
  mxml_node_t *contact = NULL;
  FILE *fp = NULL;

  xml = mxmlNewXML("1.0");

  abook = mxmlNewElement(xml, "contacts");

  contact = mxmlNewElement(abook, "contact");
  mxmlElementSetAttr(contact, "first-name", "Mike");

  fp = fopen("addrbook.xml", "w");
  if (fp == NULL) {
    return -1;
  }

  mxmlSaveFile(xml, fp, MXML_NO_CALLBACK);

  fclose(fp);

  mxmlDelete(contact);
  mxmlDelete(xml);

  return 0;
}
