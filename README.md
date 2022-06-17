# pfcc

CLI postfix calculator written in portable, C89.

## Compiling

No external dependencies are required, so this is simply a matter of running
`gcc src/*.c` or equivalent. If the preprocessor flag `SYSTEM_CLEAR` is set,
pfcc will clear the screen by running the `cls` or `clear` command which
**should** be supported on MS-DOS, Windows, and UNIX-based operating systems.
