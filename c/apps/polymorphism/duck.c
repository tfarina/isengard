#include "animal.h"

#include <stdio.h>
#include <stdlib.h>

static void
duck_make_sound(void)
{
  printf("quack\n");
}

static void
duck_eat(void)
{
  printf("duck eating...\n");
}

static animal_ops_t duck_ops = {
  duck_make_sound,
  duck_eat
};

animal_t *
animal_duck_alloc(void) {
  animal_t *duck;

  duck = malloc(sizeof(animal_t));
  if (duck == NULL) {
    return NULL;
  }

  duck->ops = &duck_ops;

  return duck;
}
