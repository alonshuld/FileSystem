# File System

This project involves creating a custom file system with commands similar to those in a Linux file system.

The program uses a file as the storage unit. You can either start a new storage by providing a new name to the program or load an existing file to resume your work.

## Commands

* ```ls [<directory>]``` - list directory content. 
* ```cat <path>``` - show file content. 
* ```touch <path>``` - create empty file. 
* ```mkdir <path>``` - create empty directory. 
* ```edit <path>``` - re-set file content. 
* ```help``` - show this help messege. 
* ```exit``` - gracefully exit. 

## Compilation

The make file has been built for linux.

```bash
make Makefile
```

## Execution

```bash
./bin/myfs <file>
```