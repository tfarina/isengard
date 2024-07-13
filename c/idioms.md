# C Idioms

* Use `for (;;) ...` for infinite loops.

* Prefer `i++` over `++i`.

* Prefer `*x` over `x[0]`
  (unless the same code also accesses `x[1]`, `x[2]` etc.)

* Prefer `x+i` over `&x[i]`.

* Use `int` variables to store boolean values in general.

* Avoid forward declarations unless indispensable.
  Let `main` be the last function in code.

