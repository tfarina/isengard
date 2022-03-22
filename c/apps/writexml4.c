#include <stdio.h>

#include "alpm_list.h"
#include "fstrdup.h"
#include "third_party/mxml/mxml.h"
#include "third_party/libuuid/uuid.h"

#define ELEM_FOLDER "folder"
#define ELEM_ITEM_LIST "item-list"
#define ELEM_ITEM "item"
#define ATTR_UID "uid"
#define ATTR_NAME "name"
#define ATTR_TYPE "type"
#define ATTR_TYPE_VAL_FOLDER "folder"

typedef enum {
  AB_ITEM_TYPE_NONE,
  AB_ITEM_TYPE_CONTACT,
  AB_ITEM_TYPE_GROUP,
  AB_ITEM_TYPE_FOLDER,
} ABItemType;

typedef struct _ABItem ABItem;
struct _ABItem {
  char *uid;
  char *name;
  ABItemType type;
  ABItem *parent;
};

typedef struct _ABFolder ABFolder;
struct _ABFolder {
  ABItem base;
  int is_root;
  alpm_list_t *list_folder;
};

#define ABITEM(item) ((ABItem *)item)
#define ABITEM_UID(item) (ABITEM(item)->uid)
#define ABITEM_NAME(item) (ABITEM(item)->name)
#define ABITEM_TYPE(item) (ABITEM(item)->type)
#define ABITEM_PARENT(item) (ABITEM(item)->parent)

static ABFolder *
addrbook_folder_create(void)
{
  ABFolder *folder;

  folder = malloc(sizeof(ABFolder));
  if (folder == NULL) {
    return NULL;
  }

  ABITEM_UID(folder) = NULL;
  ABITEM_NAME(folder) = NULL;
  ABITEM_TYPE(folder) = AB_ITEM_TYPE_FOLDER;
  ABITEM_PARENT(folder) = NULL;
  folder->is_root = 0;
  folder->list_folder = NULL;

  return folder;
}

static void
addrbook_folder_destroy(ABFolder *folder)
{
  if (folder == NULL) {
    return;
  }

  free(ABITEM_UID(folder));
  free(ABITEM_NAME(folder));

  ABITEM_UID(folder) = NULL;
  ABITEM_NAME(folder) = NULL;
  ABITEM_TYPE(folder) = AB_ITEM_TYPE_NONE;
  ABITEM_PARENT(folder) = NULL;
  folder->is_root = 0;
  folder->list_folder = NULL;

  free(folder);
}

static int
addrbook_add_folder(ABFolder *folder, ABFolder *child)
{
  if (folder == NULL || child == NULL) {
    return 0;
  }

  folder->list_folder = alpm_list_add(folder->list_folder, child);
  ABITEM_PARENT(child) = ABITEM(folder);

  return 1;
}

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
  ABFolder *root_folder, *subfolder;
  alpm_list_t *node;

  root_folder = addrbook_folder_create();
  if (root_folder == NULL) {
    return 1;
  }
  ABITEM_PARENT(root_folder) = NULL;
  root_folder->is_root = 1;

  uuid_generate(uuid);
  uuid_unparse(uuid, uuid_str);

  subfolder = addrbook_folder_create();
  if (subfolder == NULL) {
    return 1;
  }
  ABITEM_UID(subfolder) = f_strdup(uuid_str);
  ABITEM_NAME(subfolder) = f_strdup("SubFolder");

  addrbook_add_folder(root_folder, subfolder);

  xml = mxmlNewXML("1.0");

  abook = mxmlNewElement(xml, "addrbook");

  folder_node = mxmlNewElement(abook, ELEM_FOLDER);
  mxmlElementSetAttr(folder_node, ATTR_UID, ABITEM_UID(subfolder));
  mxmlElementSetAttr(folder_node, ATTR_NAME, ABITEM_NAME(subfolder));

  item_list_node = mxmlNewElement(folder_node, ELEM_ITEM_LIST);

  for (node = subfolder->list_folder;
       node;
       node = alpm_list_next(node)) {
    ABFolder *folder_item = node->data;
    item_node = mxmlNewElement(item_list_node, ELEM_ITEM);
    mxmlElementSetAttr(item_node, ATTR_TYPE, ATTR_TYPE_VAL_FOLDER);
    mxmlElementSetAttr(item_node, ATTR_UID, ABITEM_UID(folder_item));
  }

  fp = fopen("addrbook-02.xml", "w");
  if (fp == NULL) {
    return -1;
  }

  mxmlSaveFile(xml, fp, MXML_NO_CALLBACK);

  fclose(fp);

  addrbook_folder_destroy(root_folder);

  mxmlDelete(item_node);
  mxmlDelete(item_list_node);
  mxmlDelete(child_node);
  mxmlDelete(folder_node);
  mxmlDelete(xml);

  return 0;
}
