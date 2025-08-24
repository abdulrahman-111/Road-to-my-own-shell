
# Road to My Own Shell

A collection of simple Linux command-line utilities I built as stepping stones toward writing my own shell from scratch.  
Each utility focuses on learning system calls, process management, and the internals of how commands work in Linux.

This repository documents my learning journey, starting from small tools like a custom `cat` command to gradually building a fully functional shell.

---

## About the Project
This project is my personal journey to understand how Linux commands, shells, and system calls work.

- **Goal:** Build my own shell from scratch.
- **Approach:** Start small with custom utilities → learn system calls → combine everything into a shell.

---

## Utilities Implemented

- **mycat:** A simple `cat` implementation using `open`, `read`, `write`, and `close`.
- Future utilities: `myecho`, `mypwd`, `myls`, etc.

---

## Features

- Written in C using low-level system calls.
- No external libraries—pure Linux fundamentals.
- Incremental learning approach documented step by step.

---

## Learning Outcomes

- How commands are just executable programs.
- The role of shell vs. kernel.
- Basics of process creation and system calls.

---

## Usage

### Compile
```bash
gcc mycat.c -o mycat
```

### Run
```bash
./mycat file1.txt file2.txt
```

---

## Future Plans

- Add more commands (e.g., `myls`, `mymkdir`).
- Implement a basic shell supporting command execution, pipes, and redirection.

---

## License

This project is open-source and available under the MIT License.
