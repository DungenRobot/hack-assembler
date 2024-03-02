# Hack Assembler Project in C
Created by Anders Olsen for CSCI 140

## Overview

This is an assembler for the Hack platform (part of the [nand2Tetris](https://www.nand2tetris.org/) series).
It turns an assembly file into a hack file that can be interpreted by the hack simulator. 

## Usage
Build the assembler with `make` command. Then convert a file using

```
./assembler [input file] [output file]
```

Example:

```
./assembler input.asm output.hack
```

Tested in Ubuntu, made for gcc. May not run or compile on all platforms.
