# C Coding Style Guide

## Literals

* Hexadecimal prefix should be 0x.
* Hexadecimal literals should use upper case letters A-F.

## Idioms:

* Use `for (;;) ...` for infinite loops.

* Prefer `i++` over `++i`.

* Prefer `*x` over `x[0]`
  (unless the same code also accesses `x[1]`, `x[2]` etc.)

* Prefer `x+i` over `&x[i]`.

* Use `int` variables to store boolean values in general.

* Avoid forward declarations unless indispensable.
  Let `main` be the last function in code.

## Naming:

### Macro Names

Should be all upper case (SCREAMING_SNAKE_CASE) with words separated by
underscores (“_”).

```
#define BUFFER_SIZE 1024
```

### Variable Names

Variable names should typically be nouns.

Should be in mixed case (camelCase) with the first word in lower case and
every other word starts with a capital letter.

```
int numEntries;
char firstName[64];
```

### Type Names

Type names should typically be nouns.

The names of all types (enums, structs, unions and typedefs) should follow
the same naming convention. Should be all upper case (SCREAMING_SNAKE_CASE)
with words separated by underscores (“_”).

```
typedef enum _ENUM_NAME
{
    ...
} ENUM_NAME;
```

```
typedef struct _STRUCT_NAME
{
    ...
} STRUCT_NAME;
```

```
typedef union _UNION_NAME
{
    ...
} UNION_NAME;
```

### Function Names

Function names should typically be in the form of verb + noun.

Should be in mixed case (CamelCase or PascalCase) with the first letter
of each word capitalized.

```
void
FunctionName(void)
{
    ...
}
```

## The rest:

* Avoid long functions.

* Avoid long parameter lists.
