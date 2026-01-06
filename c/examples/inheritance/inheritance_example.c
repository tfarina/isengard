#include <stdio.h>

struct base;

struct base_vtable {
  void (*dance) (struct base*);
  void (*jump) (struct base*, int how_high);
};

struct base {
  int foo;

  struct base_vtable* vtable;
};

static void base_dance(struct base* b) {
  b->vtable->dance(b);
}

static void base_jump(struct base* b, int how_high) {
  b->vtable->jump(b, how_high);
}

struct derived {
  struct base super;

  int bar;
};

static void derived_dance(struct base* b) {
  printf("%s called!\n", __func__);
}

static void derived_jump(struct base* b, int how_high) {
  printf("%s called!\n", __func__);
}

static struct base_vtable derived_vtable = {
  .dance = derived_dance,
  .jump  = derived_jump,
};

static void derived_init(struct derived* d) {
  d->super.vtable = &derived_vtable;
}

int main(void) {
  struct derived d;
  derived_init(&d);

  struct base* base_ptr = (struct base*)&d;

  base_dance(base_ptr);
  base_jump(base_ptr, 16);

  return 0;
}
