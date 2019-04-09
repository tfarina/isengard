#include "animal.h"

void
animal_make_sound(animal_t *animal)
{
  animal->ops->make_sound();
}

void
animal_eat(animal_t *animal)
{
  animal->ops->eat();
}

