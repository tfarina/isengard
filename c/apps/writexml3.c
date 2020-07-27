#include <stdio.h>

#include "third_party/mxml/mxml.h"
#include "third_party/libuuid/uuid.h"

int main(int argc, char **argv)
{
  mxml_node_t *xml = NULL;
  mxml_node_t *abook = NULL;
  mxml_node_t *contact = NULL;
  uuid_t uuid;
  char uuid_str[37];
  FILE *fp = NULL;

  uuid_generate(uuid);
  uuid_unparse(uuid, uuid_str);

  xml = mxmlNewXML("1.0");

  abook = mxmlNewElement(xml, "contacts");

  contact = mxmlNewElement(abook, "contact");
  mxmlElementSetAttr(contact, "uid", uuid_str);

  fp = fopen("addrbook-01.xml", "w");
  if (fp == NULL) {
    return -1;
  }

  mxmlSaveFile(xml, fp, MXML_NO_CALLBACK);

  fclose(fp);

  mxmlDelete(contact);
  mxmlDelete(xml);

  return 0;
}
