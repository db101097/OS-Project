At the start, your program asks the user three questions:

How much RAM memory is there on the simulated computer? Your program receives the number in bytes (no kilobytes or words). I can enter any number up to 4000000000 (4 billions).
What is the size of a page/frame. The enumeration of pages starts from 0.How many hard disks does the simulated computer have? The enumeration of the hard disks starts with 0.
After these questions are answered, the simulation begins. You program constantly listens for the user inputs. You should NOT ask for a confirmation to enter another input. The user inputs signal some system events. Your program simulates the corresponding system behavior.
The possible inputs are:

A       ‘A’ input means that a new process has been created. 

When a new process arrives, your program should create its PCB and allocate memory for it’s first page (page #0).

Also, when a new process is created your program should send it to the top level of the ready-queue or allow it to use the CPU right away.

When choosing a PID for the new process start from 1 and go up. Do NOT reuse PIDs of the terminated processes.

Q       The currently running process has spent a time quantum using the CPU. If the same process continues to use the CPU and one more Q command arrives, it means that the process has spent one more time quantum.

t         The process that is currently using the CPU terminates. It leaves the system immediately. Make sure you release the memory used by this process. 

d number file_name       The process that currently uses the CPU requests the hard disk #number. It wants to read or write file file_name.

D number   The hard disk #number has finished the work for one process.

m address   The process that is currently using the CPU requests a memory operation for the logical address.

S r     Shows what process is currently using the CPU and what processes are waiting in the ready-queue. 

S i      Shows what processes are currently using the hard disks and what processes are waiting to use them. For each busy hard disk show the process that uses it and show its I/O-queue. Make sure to display the filenames (from the d command) for each process. The enumeration of hard disks starts from 0.

S m   Shows the state of memory. For each used frame display the process number that occupies it and the page number stored in it. The enumeration of pages and frames starts from 0.





Compiling
Compile with g++ -std=c++11 os_project.cpp while in the same directory as the file
