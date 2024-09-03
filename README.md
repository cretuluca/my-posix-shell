# Custom Shell Implementation

This project is a custom shell implementation in C, which provides a simplistic command line interface. It provides both built-in commands and system utility commands. The inspiration for this project arose from my interest in the Linux Operating System and, therefore, the Unix command line.

## Building the Project
To build the project, run the `build.sh` script (ensure you have a C compiler, like `gcc`, installed):

```bash
./build.sh
```

## Usage
```bash
./my_shell
```

## Functionalities
- Built-in commands:
  - `exit`: Exit the shell
  - `echo`: Print text to the console
  - `type`: Display information about command type
  - `pwd`: Print working directory
  - `cd`: Change directory
  - `history`: Display command history
- Execution of system utilities (ls, grep, kill,..)
- Command history management

You'll be presented with a prompt (`$`) where you can enter commands.

### Example commands:

```
$ pwd
/home/user/projects
$ echo Hello, World!
Hello, World!
$ cd /tmp
$ history
```

## Implementation Details

- Uses the `readline` library for improved input handling
- Implements a hash table for efficient built-in command lookup
- Utilizes system calls like `fork()`, `execv()`, and `chdir()` for command execution and directory navigation
- Manages command history with a maximum of 1000 entries
