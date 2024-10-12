# Enumeration Types

An enumeration consists of a set of named integer constants. A variable of
the enumeration type stores one of the values of the enumeration set defined
by that type.

You use an enumeration type to represent a choice from a set of mutually
exclusive values or a combination of choices. To represent a combination
of choices, define an enumeration type as bit flags.

## Enumeration types as bit flags

If you want an enumeration type to represent a combination of choices,
define enum members for those choices such that an individual choice is a
bit field. That is, the associate values of those enum members should be
the powers of two. Then, you can use the bitwise logical operators | or &
to combine choices or intersect combinations of choices, respectively.

https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/builtin-types/enum
