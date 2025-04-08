# MEOW

This is a simple trans-compiled language that converts cat language into [brainfuck](https://en.wikipedia.org/wiki/Brainfuck).

Only the lowercase letters m, e, o, and w, are allowed. For any "m" in the program, an "e" must follow. For every "e", an "o". No repeats ("mm" is not accepted).\
But how can 4 letters map to 8 brainfuck operations?

## Comments and Operation Rotates
We have two more characters "!" and "#":
1. "#" allows you to comment.
2. "!" will rotate the operations that each character in "meow" does.

For example (**note**: this is not a compileable program, as the "meow" convention does not allow repeats):

```
m!m!m!m!m!m!m!m!m #This is a comment
```
will become
```
-+<>,.[]- This is a comment
```
As you can see, just the character "m" can perform any operation. As you can imagine, this is a cumbersome language.

"!" will cycle through the order of this enum:
```
enum Operation {
    INCR = '-',     **m starts here**
    DECR = '+',     **e starts here**
    LEFT = '<',     **o starts here**
    RIGHT = '>',    **w starts here**
    INPUT = ',', 
    OUTPUT = '.',
    LBRACK = '[',
    RBRACK = ']'
};
```

This can be used as reference when writing programs:
```
-+<>,.[]
^^^^
meow
```

## Hello World
**TODO!**