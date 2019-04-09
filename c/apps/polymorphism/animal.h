#ifndef ANIMAL_H_
#define ANIMAL_H_

/* Animal virtual table (vtable). */
typedef struct animal_ops_s {
  void (*make_sound)(void);
  void (*eat)(void);
} animal_ops_t;

typedef struct animal_s {
  animal_ops_t *ops;
} animal_t;

void animal_make_sound(animal_t *animal);
void animal_eat(animal_t *animal);

animal_t *animal_duck_alloc(void);

#endif  /* ANIMAL_H_ */
