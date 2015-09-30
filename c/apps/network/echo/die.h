#ifndef DIE_H_
#define DIE_H_

void die(const char *msg, ...) __attribute__((noreturn))
    __attribute__((format(printf, 1, 2)));

#endif  // DIE_H_
