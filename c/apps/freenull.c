#include <stdlib.h>

typedef struct _person
{
  char *name;
} person;

int
main(void)
{
  person *p_person = NULL;

  free(p_person);
  free(NULL);

  return 0;
}
