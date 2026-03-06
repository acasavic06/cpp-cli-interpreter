# CLI Interpreter (OOP1 Project)

This project was developed as a **course project for the subject Object Oriented Programming 1** at the **School of Electrical Engineering, University of Belgrade** during the 2025/2026 academic year.

The goal of the project is to implement a simplified **Command Line Interpreter (CLI)** in **C++** that behaves similarly to a basic operating system shell.  
The interpreter reads commands from standard input, processes them, and executes them while supporting several common shell features such as command parsing, pipelines, and redirection.

---

# Project Description

The interpreter runs as an interactive console application.  
It repeatedly waits for the user to enter a command line, parses the command, and executes it.

Basic workflow:

1. The interpreter prints a **prompt** (initially `$`)
2. The user enters a command
3. The interpreter executes the command
4. The process repeats until the program is terminated

The interpreter supports:

- command parsing
- input and output streams
- file manipulation
- pipelines
- input/output redirection
- batch execution of commands
- error handling

---

# Supported Commands

The interpreter supports the following commands:

### echo
Outputs the input text to the output stream without modification.

echo "Hello world"

---

### prompt
Changes the interpreter prompt.


prompt ">>"


---

### time
Prints the current system time.


time


---

### date
Prints the current system date.


date


---

### touch
Creates a new empty file.


touch file.txt


---

### truncate
Deletes the contents of a file but keeps the file.


truncate file.txt


---

### rm
Deletes a file from the filesystem.


rm file.txt


---

### wc
Counts words or characters from the input stream.

Options:

- `-w` count words
- `-c` count characters

Examples:


wc -w "Hello world from CLI"

wc -c input.txt


---

### tr
Replaces occurrences of a substring with another substring.


tr -"a" "b"


---

### head
Outputs the first *n* lines of input.


head -n5 file.txt


---

### batch
Executes commands from a file as a batch.


batch commands.txt


---

# Pipes

Commands can be connected using **pipes (`|`)**, where the output of one command becomes the input of the next command.

Example:


time | tr -":" "." | wc -c


---

# Input / Output Redirection

Commands support redirection:

### Input redirection


wc -w < input.txt


### Output redirection


wc -w "Hello world" > output.txt


### Append output

wc -w "Hello world" >> output.txt


---

# Error Handling

The interpreter detects and reports different types of errors:

- unknown commands
- syntax errors
- invalid arguments
- file system errors
- invalid pipeline usage
- invalid redirection

Error messages are printed to the error output stream.

---

# Technologies

- **C++**
- **Object Oriented Programming**
- **Standard Template Library (STL)**
- **File streams**

---

# How to Run the Project

### 1. Clone the repository

git clone https://github.com/acasavic06/cpp-cli-interpreter

### 2. Open the project in Visual Studio

Open the `.sln` file using **Microsoft Visual Studio**.

### 3. Build the project

Build the solution using: Build → Build Solution


### 4. Run the program

Run the project from Visual Studio:


Ctrl + F5


The interpreter will start in the console and wait for commands.

Example commands:

$ echo "Hello world"
Hello world

$ wc -w "Lorem ipsum dolor sit amet"
5

**Author**: Aleksandar Savić  
**Github**: @acasavic06
