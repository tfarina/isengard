#include "animal.h"

#include <stdlib.h>

int
main(void)
{
  animal_t *duck;

  duck = animal_duck_alloc();
  if (duck == NULL) {
    return -1;
  }

  animal_make_sound(duck);
  animal_eat(duck);

  free(duck);

  return 0;
}
