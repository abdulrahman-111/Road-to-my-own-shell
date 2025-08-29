
# Mini Unix Utilities in C

This repository contains simplified implementations of common Unix commands written in C:
- **cp**: Copy files
- **mv**: Move files (copy + delete)
- **echo**: Print text to standard output
- **cat**: Print file contents to standard output
- **pwd**: Print the current working directory

These implementations are for learning purposes and mimic the basic functionality of the real commands.

---

## Compilation

You can compile all utilities separately using `gcc`.  
For example:

```bash
# Compile cp
gcc cp.c -o cp

# Compile mv
gcc mv.c -o mv

# Compile echo
gcc echo.c -o echo

# Compile cat
gcc cat.c -o cat

# Compile pwd
gcc pwd.c -o pwd
```

Or compile them all at once:
```bash
gcc cp.c -o cp && gcc mv.c -o mv && gcc echo.c -o echo && gcc cat.c -o cat && gcc pwd.c -o pwd
```

---

## Usage and Examples

### 1. Copy (`cp`)
Copies content of one file to another.
```bash
./cp source.txt dest.txt
```
**Output:**
```
(source.txt contents copied to dest.txt)
```

---

### 2. Move (`mv`)
Copies the file to the new location, then deletes the original.
```bash
./mv source.txt dest.txt
```
**Output:**
```
(source.txt moved to dest.txt)
```

---

### 3. Echo (`echo`)
Prints text to standard output.
```bash
./echo Hello World
```
**Output:**
```
Hello World
```

---

### 4. Cat (`cat`)
Prints file content to standard output.
```bash
./cat file.txt
```
**Output:**
```
(contents of file.txt displayed)
```

---

### 5. Print Working Directory (`pwd`)
Displays the current directory path.
```bash
./pwd
```
**Output:**
```
/home/user/project
```

---

## Notes
- These are simplified versions and do not include all options/features of real Unix commands.
- Error handling for invalid files, missing arguments, etc., is included.

---

## Author
Abdulrahman Hassan – Computer Engineering Student, EUI  
