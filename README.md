# pfcc

CLI postfix calculator written in portable, C89.

## Compiling

No external dependencies are required, so this is simply a matter of running
`gcc src/*.c` or equivalent.

### Preprocessor Flags

 - `SYSTEM_CLEAR` - If set, pfcc will clear the screen by running the `cls` or
`clear` command which **should** be supported on MS-DOS, Windows, and UNIX-based
operating systems.

 - `MAX_SIZE` - This sets the size of the calculating stack, up to a maximum of
`255`. The stack is preallocated, so reducing `MAX_SIZE` will reduce memory
usage.

 - `USE_FLOAT` - This flag will cause pfcc to store data as a single-precision,
as opposed to a double-precision, floating-point type. If the compiler
distinguishes between the two, `USE_FLOAT` should further memory savings.
