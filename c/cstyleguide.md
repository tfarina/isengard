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

There are two kinds of macros: **object-like** and **function-like**.

#### Object-Like Macros

Object-like macros should use SCREAMING_SNAKE_CASE.

These macros typically represent constants, numeric values, or flags.

**Example:**
```
#define BUFFER_SIZE 1024
#define PRINTER_PAUSED 0x00000001
```

#### Function-Like Macros

Function-like macros should use PascalCase.

Function-like macros resemble function calls.

### Enumeration Names

Enum types and their values should use SCREAMING_SNAKE_CASE. Write all words in
uppercase and separate them with underscores.

Use a noun or noun phrase for the name.

Use singlar name unless the values are bit flags - in that case, use a plural
name.

Prefix the enum tag with an underscore (`_`). This makes the tag distinct from
the typedef name and reduces the chance of name clashes. Use the same uppercase
name for the typedef.

Alternative forms exist, such as:
- `tagENUM_NAME` — prefix with `tag`
- `_tagENUM_NAME` — prefix with both underscore and `tag`
- `ENUM_NAME` — no prefix at all

These are acceptable but not preferred. Use them only for compatibility with
existing codebases or for consistency with external codebases.

**Example:**
```
typedef enum _COLOR_MODE
{
    COLOR_MODE_RGB,
    COLOR_MODE_CMYK
} COLOR_MODE;
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

**Examples:**
```
typedef struct _PERSON
{
    char name[50];
    int age;
} PERSON;

typedef union _DATA
{
    int intValue;
    float floatValue;
} DATA;
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

* Prefix hexadecimal values with 0x.
* Use uppercase letters A-F.

## Error Variables

You can name your local error variables like: rv, ret, retval, res, result,
rc, err.

## The rest:

* Avoid long functions.

* Avoid long parameter lists.
