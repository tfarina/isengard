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

**Example:**
```
#define BUFFER_SIZE 1024
#define PRINTER_PAUSED 0x00000001
```

### Enumeration Names

Enum types and their values should use SCREAMING_SNAKE_CASE. Write all words in
uppercase and separate them with underscores.

Use a noun or noun phrase for the name.

Use singlar name unless its values are bit fields.

Use plural if the enumeration has bit fields as values, also called flags enum.

To form the enum name you may start (prefix) it with an underscore
(_SCREAMING_SNAKE_CASE), with tag (tagSCREAMING_SNAKE_CASE), with underscore
and tag (_tagSCREAMING_SNAKE_CASE) or with nothing (SCREAMING_SNAKE_CASE).
The first one is the preferred way.

**Example:**
```
typedef enum _ENUM_NAME
{
    ...
} ENUM_NAME;
```

### Variable Names

Variables should use camelCase. Start with a lowercase letter and capitalize
the first letter of each subsequent word, using no separators.

Use a noun, noun phrase, or adjective for the name.

**Example:**
```
int maxSpeed;
float avgTemp;
char firstName[64];
```

### Type Names

Structs and unions should use SCREAMING_SNAKE_CASE. Write all words in
uppercase and separate them with underscores.

Use a noun or noun phrase for the name. Prefix the tag with an underscore (_),
and use the same uppercase name for the typedef.

**Example:**
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

Functions should use PascalCase. Capitalize the first letter of each word
and join them without separators.

Use a verb or verb phrase in the imperative mood.

We have is a list of verbs you may use to begin your function names here:
[verbs](verbs.txt)

#### Function Naming Pattern: A/HC/LC

Use this pattern when applicable:

- **A** = Action (verb)
- **HC** = High-context noun (what)
- **LC** = Optional low-context (where, how, why)

**Example:**
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
