# C Coding Style Guide

## Naming:

### File Names

Should be all lower case and can include underscores ("_") or dashes ("-").

Name files using a noun or noun phrase, but NOT verbs.

The following are acceptable forms of file names (in order of preference):

* modulename.c
* module_name.c
* module-name.c

### Macro Names

Should be all upper case (SCREAMING_SNAKE_CASE) with words separated by
underscores (“_”).

```
#define BUFFER_SIZE 1024
#define PRINTER_PAUSED 0x00000001
```

### Naming Enumerations

Name enumeration (enum) types with nouns or noun phrases. All characters
in upper case (SCREAMING_SNAKE_CASE) with words separated by underscores (“_”).

Use singlar name unless its values are bit fields.

Use plural if the enumeration has bit fields as values, also called flags enum.

```
typedef enum _ENUM_NAME
{
    ...
} ENUM_NAME;
```

### Variable Names

Name variables using a noun, noun phrase, or adjective.

Should be in mixed case (camelCase) with the first word in lower case and
every other word starts with a capital letter.

```
int numEntries;
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

Function names should be verbs or verb phrases, because they are a way of doing an action.

Should be in mixed case (CamelCase or PascalCase) with the first letter
of each word capitalized.

```
void
FunctionName(void)
{
    ...
}
```

## Formatting

### Literals

* Hexadecimal prefix should be 0x.
* Hexadecimal literals should use upper case letters A-F.

## The rest:

* Avoid long functions.

* Avoid long parameter lists.
