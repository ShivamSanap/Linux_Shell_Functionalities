<h1 align="center">Simple Linux Shell</h1>

<p>This project implements a custom Linux shell supporting command execution, background processes, custom signal handling, and directory navigation. It uses system calls such as fork, execvp, wait, waitpid, kill, and setpgid.</p>

## Part A: Basic Shell  (simple_shell.c)

<p>Executes simple Linux commands (ls, cat, echo, sleep). Uses fork → execvp → wait to run and reap commands. Prints current working directory as the prompt. Supports the built-in cd command using chdir in the parent. Handles invalid commands and displays exit codes from children.</p>

## Part B: Background Execution using & (background_execution.c)

<p>Commands ending with & run in the background without blocking the shell. Stores PIDs of background processes (max 64). Reaps finished background jobs using waitpid(..., WNOHANG). Prints a message when any background process finishes. Foreground and background processes run without interfering with each other.</p>

## Part C: exit Command (exit_command)

<p>Typing exit stops the shell loop. All active background processes are terminated using kill. The shell waits for them to be reaped before exiting. Cleans up allocated memory and exits cleanly.</p>

## Part D: Ctrl+C SIGINT Handling (custom_signal_handling)

<p>Ctrl+C no longer kills the shell itself. Only the current foreground process gets terminated. Background processes are protected by placing them in separate process groups (setpgid). Custom SIGINT handler sends the signal to the foreground process group only.</p>

#Compilation

1. Run:
```
gcc myshell.c -o myshell
```

2. Usage

Start the shell:
```
./myshell
```

Examples:
```
/home/user $ ls
/home/user $ sleep 5 &
/home/user $ cd Documents
/home/user $ exit
```