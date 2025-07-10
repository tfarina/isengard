# C Style Guide

## Naming

### File Names

File names should be all lowercase and may use underscores `_` or dashes `-`
as separators.

Name files using a noun or noun phrase, but NOT verbs.

The following are acceptable forms of file names (in order of preference):

* modulename.c
* module_name.c
* module-name.c

### Macro Names

There are two kinds of macros. Object-like macros, which resemble data objects,
and function-like macros, which resemble function calls. So, naming a
macro will depend on its kind.

If you are naming an object-like macro, use a descriptive noun and
the SCREAMING_SNAKE_CASE convention where all characters are upper case
and words are separated by underscores (“_”).

If, instead, you are naming a function-like macro, then use PascalCase
convention.

```
#define BUFFER_SIZE 1024
#define PRINTER_PAUSED 0x00000001
```

### Naming Enumerations

Name enumeration (enum) types with nouns or noun phrases. All characters
in upper case (SCREAMING_SNAKE_CASE) with words separated by underscores (“_”).

Use singlar name unless its values are bit fields.

Use plural if the enumeration has bit fields as values, also called flags enum.

To form the enum name you may start (prefix) it with an underscore
(_SCREAMING_SNAKE_CASE), with tag (tagSCREAMING_SNAKE_CASE), with underscore
and tag (_tagSCREAMING_SNAKE_CASE) or with nothing (SCREAMING_SNAKE_CASE).
The first one is the preferred way.

```
typedef enum _ENUM_NAME
{
    ...
} ENUM_NAME;
```

### Variable Names

Use camelCase: start with a lowercase letter and capitalize the first letter
of each subsequent word, using no separators.

Name variables with nouns, noun phrases, or adjectives.

```
int maxSpeed;
float avgTemp;
char firstName[64];
```

### Type Names

Name types (struct or union) with nouns or noun phrases, with all characters
upper case (SCREAMING_SNAKE_CASE) and words separated by underscores (“_”).

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

Function names should be verbs or verb phrases in the imperative mood,
because they are the means of doing action.

Should be in mixed case (PascalCase) with the first letter of each word
capitalized.

We have is a list of verbs you may use to begin your function names here:
[verbs](verbs.txt)

## A/HC/LC Pattern

When naming functions, there is this useful pattern you may follow:

```
action (A) + high context (HC) + low context? (LC)
```

```
void
FunctionName(void)
{
    ...
}
```

## Formatting

Tab size should be 4.

### Indenting

Indent size should be 4.

### Literals

* Hexadecimal prefix should be 0x.
* Hexadecimal literals should use upper case letters A-F.

## Error Variables

You can name your local error variables like: rv, ret, retval, res, result,
rc, err.

## The rest:

* Avoid long functions.

* Avoid long parameter lists.
