#include <stdio.h>

#include "third_party/mxml/mxml.h"
#include "third_party/libuuid/uuid.h"

#define ELEM_FOLDER "folder"
#define ELEM_ITEM_LIST "item-list"
#define ELEM_ITEM "item"
#define ATTR_UID "uid"
#define ATTR_NAME "name"
#define ATTR_TYPE "type"

int main(int argc, char **argv)
{
  mxml_node_t *xml = NULL;
  mxml_node_t *abook = NULL;
  mxml_node_t *folder_node = NULL;
  mxml_node_t *child_node = NULL;
  mxml_node_t *item_list_node = NULL;
  mxml_node_t *item_node = NULL;
  uuid_t uuid;
  char uuid_str[37];
  FILE *fp = NULL;

  uuid_generate(uuid);
  uuid_unparse(uuid, uuid_str);

  xml = mxmlNewXML("1.0");

  abook = mxmlNewElement(xml, "addrbook");

  folder_node = mxmlNewElement(abook, ELEM_FOLDER);
  mxmlElementSetAttr(folder_node, ATTR_UID, uuid_str);
  mxmlElementSetAttr(folder_node, ATTR_NAME, "NewFolder");

  uuid_generate(uuid);
  uuid_unparse(uuid, uuid_str);

  child_node = mxmlNewElement(abook, ELEM_FOLDER);
  mxmlElementSetAttr(child_node, ATTR_UID, uuid_str);
  mxmlElementSetAttr(child_node, ATTR_NAME, "subfolder");

  item_list_node = mxmlNewElement(folder_node, ELEM_ITEM_LIST);
  item_node = mxmlNewElement(item_list_node, ELEM_ITEM);
  mxmlElementSetAttr(item_node, ATTR_TYPE, "folder");
  mxmlElementSetAttr(item_node, ATTR_UID, uuid_str);

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
