Two pass assembler

Usage: assembler file1.as file2.as...  

Assemble assembler code (.as file)  

The assembling is done in two passes:
The first pass mostly calculate the address of each label and store them into a table.
The second pass encode every line and dump it to a file.

Temporary files are used during the second pass, and will be automatically deleted at the end of the assembling.

Files:
- encoder: Functions related to the encoding of the data and the filesystem I/O operations
 
- errors: Error checking functions
 
- instructions: Instructions related functions (parsers, checkers..)
    - Parse instructions out of a string
    - Check if an instruction is of a certain type
    - Retrieve additional information out of instructions (for example: opcode)

- labels: Labels creator and handlers
 
- first_pass: First pass of the assembling, map every label to its corresponding address

- second_pass: Second pass of the assembling, encode every line to the object (.ob) file.
 
- utils: Utilitaries functions
 
- main: Main entry point

