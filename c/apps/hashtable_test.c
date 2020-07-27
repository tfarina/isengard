#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "third_party/clark/hashtable.h"
#include "third_party/libuuid/uuid.h"

typedef struct item_s {
  char *uid;
} item_t;

/* Basic string hash function from Python (as found in Niels Provos's hash table implementation) */
static int unsigned hash_from_key_fn(void *k)
{
  unsigned h;
  const unsigned char * cp = (const unsigned char *) k;
  h = * cp << 7;
  while (* cp)
    h = (1000003 * h) ^ * cp ++;

  /* This conversion truncates the length of the string, but that's ok */
  h ^= (unsigned) (cp - (const unsigned char *) k);

  return h;
}

static int keys_equal_fn(void *key1, void *key2)
{
  return strcmp(key1, key2) == 0;
}

int main(void)
{
  struct hashtable *h;
  item_t *item;
  uuid_t uuid;

  h = create_hashtable(16, hash_from_key_fn, keys_equal_fn);

  item = malloc(sizeof(item_t));
  item->uid = malloc(sizeof(char) * 37);

  uuid_generate(uuid);
  uuid_unparse(uuid, item->uid);

  if (!hashtable_insert(h, item->uid, item)) {
    return 1;
  }

  item = hashtable_search(h, item->uid);
  if (item == 0) {
    fprintf(stderr, "item not found!\n");
  }

  item = hashtable_remove(h, item->uid);
  if (item == 0) {
    fprintf(stderr, "item not found!\n");
  }

  hashtable_destroy(h, 0);

  return 0;
}
