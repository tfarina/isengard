#include <stdio.h>

#include "third_party/mxml/mxml.h"
#include "third_party/libuuid/uuid.h"

int main(int argc, char **argv)
{
  mxml_node_t *xml = NULL;
  mxml_node_t *abook = NULL;
  mxml_node_t *folder_node = NULL;
  uuid_t uuid;
  char uuid_str[37];
  FILE *fp = NULL;

  uuid_generate(uuid);
  uuid_unparse(uuid, uuid_str);

  xml = mxmlNewXML("1.0");

  abook = mxmlNewElement(xml, "addrbook");

  folder_node = mxmlNewElement(abook, "folder");
  mxmlElementSetAttr(folder_node, "uid", uuid_str);
  mxmlElementSetAttr(folder_node, "name", "NewFolder");

  uuid_generate(uuid);
  uuid_unparse(uuid, uuid_str);

  folder_node = mxmlNewElement(abook, "folder");
  mxmlElementSetAttr(folder_node, "uid", uuid_str);
  mxmlElementSetAttr(folder_node, "name", "subfolder");

  fp = fopen("addrbook-02.xml", "w");
  if (fp == NULL) {
    return -1;
  }

  mxmlSaveFile(xml, fp, MXML_NO_CALLBACK);

  fclose(fp);

  mxmlDelete(folder_node);
  mxmlDelete(xml);

  return 0;
}
