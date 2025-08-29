#  Road-to-My-Own-Shell

A collection of custom command-line utilities and progressively advanced shell implementations in C, created by **Abdulrahman Gomaa**. Starting from basic commands and culminating in a fully integrated **Micro Shell**, this project demonstrates a step-by-step journey in building a functional shell environment.

---

##  Repository Contents

| Component          | Description                                                                 |
|-------------------|-----------------------------------------------------------------------------|
| `echo.c`           | Custom `echo` implementation — prints command-line arguments.               |
| `pwd.c`            | Prints the current working directory, mimicking `pwd` command.              |
| `cd.c`             | `cd` implementation — changes the shell's current working directory.        |
| `cp.c`             | Copies files from source to destination, like `cp`.                        |
| `mv.c`             | Moves or renames files, similar to `mv`.                                   |
| `femto_shell.c`    | First minimal shell — executes simple external commands.                   |
| `pico_shell.c`     | Enhanced shell with basic command parsing, error codes, and history.       |
| `nano_shell.c`     | Adds input/output/error redirection (`<`, `>`, `2>`), improving utility.    |
| `micro_shell.c`    | **Final integrated shell** — supports all utilities, variables, redirection, built-ins, and environment. |

---

##  Compilation

Compile each utility or shell individually:
```bash
gcc -o echo echo.c
gcc -o pwd pwd.c
gcc -o cd cd.c
gcc -o cp cp.c
gcc -o mv mv.c
gcc -o femto femto_shell.c
gcc -o pico pico_shell.c
gcc -o nano nano_shell.c
gcc -o micro micro_shell.c
```

Or compile everything in one go (Linux/macOS):
```bash
for src in *.c; do gcc -o "${src%.c}" "$src"; done
```

---

##  Usage Examples

### Individual Utilities:
```bash
$ ./echo Hello World
Hello World

$ ./pwd
/home/user

$ ./cd /tmp
$ ./pwd
/tmp

$ ./cp source.txt copy.txt
$ ./mv old.txt new.txt
```

---

### Shell Versions:

#### femto_shell
Basic command execution:
```
Nano Shell Prompt > ls -l
[output of ls]
Nano Shell Prompt >
```

#### pico_shell
Better input handling and exit codes.

#### nano_shell
Supports redirections:
```
Nano Shell Prompt > cat < input.txt
[contents of input.txt]
Nano Shell Prompt >

Nano Shell Prompt > ls non_existing 2> err.log
Nano Shell Prompt >
```

#### micro_shell (Final Version)
Full-featured, integrates all utilities:

- Command execution  
- Redirection support  
- Variable assignment & expansion  
- Built-ins: `echo`, `pwd`, `cd`, `export`  
- Environment handling

Example:
```
Micro Shell Prompt > YOU_ARE=a_programmer
Micro Shell Prompt > echo $YOU_ARE
a_programmer
Micro Shell Prompt >
```

---

##  Exit Codes

- `0` — success  
- `1` — failure (e.g., invalid command, file not found, wrong syntax)

---

##  Author

**Abdulrahman Gomaa**  
Contact: [your-email@example.com]

---

##  Notes

- The shell implementation evolved from `femto` → `pico` → `nano` → `micro`.
- `micro_shell.c` is the final, integrated version that brings together all utility implementations and features.
- Feel free to explore each file to follow the learning progression.

---
