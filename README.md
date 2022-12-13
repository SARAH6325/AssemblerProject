# AssemblerProject
Writing a C language assembler for a predefined assembly language.

Two Pass Assembler as a final project of the course 20465 - Systems Programming in C.

Run make
Run ./assembler yourFileName
The assembler will output .ent, .ext and .ob files.

Files Structure

Assembler files:

main.c - Main program.

first_pass.c - An implementation of first pass algorithm.

first_analyze.c - Auxiliary methods for encoding the rows in memory

second_pass.c - An implementation of second pass algorithm.

errors.c - A thorough check of errors during first and second pass.

macro_layuot.c - Returns a file after macro's layout.

Data Structures
structs.h - Contains the structures needed for the program.

external_linked_list_struct.c - Used to represent Extern Symbol Table of the assembler.

label_linked_list_struct.c - Used to represent  internal Symbol Table of the assembler.


Helpers and utils

reserved_word.c - main program helpers to check arguments validity.

utils.c - General program helpers.

declerations_of_constants.h - statement of operations.

declerations_of_functions.h - Statement of constants.

Our imaginary computer consists of CPU, Registers and RAM (some of the RAM is utilized as stack).

The CPU has 8 registers (r0-r7). each register size is 0 bits. lsb is 0 and msb is 9.

The CPU has a register named PSW which contains flags regarding computer status in each moment.

Memory size is 256 and each memory cell size is 10 bits.

The computer works only with Integers. Word and Sentence Structure

The last 2 lsb Represents 'A,R,E'. 'A' Stands for The Label was created on the current file. 'R' Stands for The label is created on this file. 'E' Stands for the Label was created on external file.

Each computer instruction consists between 1 to 3 words which are encoded in the following manner: 0 1 2 3 4 5 6 7 8 9 ARE ARE destination destination source source opcode opcode opcode opcode ARE The assembler consists of 16 actions, each of them consists a different opcode (mov, cmp, add, sub, lea, clr, not, inc, dec, jmp, bne, jsr, red, prn, rts and hlt).

There are 4 kinds of sentences the assembler knows:

Empty Sentence - A line contains only whitespaces.

Comment Sentence - A line that starts with ;.

Instruction Sentence - Variables assignment and declaration.

Command Sentence - Creates an action for the machine to execute upon running the program.

Line maximum length is 80.

Usage of labels is optional. A label is any word (reserved words not allowed) which is declared at the beginning of the sentence and ends with :. For example myLabel:. Instruction Sentence

Instruction Sentence may or may not start with a label. Valid instructions are:

.data - declaration of integers. For example: .data 12, 453, -6.

.string - declaration of a string contained within " ". For example: .string "OH MY GOD!".

.struct - declaration of a single Integer and a String. For example: .struct 3 "ABC".

.extern - reference to an external label, declared in another file. For example .extern myLabel.

.entry - reference to an internal label, that already was or will be declared in the program. For example .entry myLabel.

Command Sentence

Command Sentence may or may not start with a label. Valid commands are:

mov - copies origin operand to destination

cmp - performs comparison between 2 operands

add - destination operand receives the addition result of origin and destination

sub - Subtraction result between two operands

lea - load effective address

clr - clears operand

not - logical not, reverses all bits in operand

inc - increments operand's content by 1

dec - decrements operand's content by 1

jmp - jumps to instruction in destination operand

bne - branch if not equal to zero

get - Reading a character from the standard input to the operand

prn - prints char into stdout

hlt - stops the program The Two Pass Algorithm

When the assembler receives input in assembly language, it has to go over the input 2 times. The reason for that is the references to instructions which still has unknown addresses during first pass.

The assembler has 2 linked lists representing the Image Code and Image Data and another linked list representing the Symbol Table.

Symbol Table - will be updated during first pass with the addresses of the instructions.

Image Code - represents machine code of all the command sentences.

Image Data - represents machine code of all the instruction sentences.

Using an instruction counter, each instruction is being added to the counter, ensuring the next instruction will be assigned to free memory space.

At first pass, the symbols (labels) in the program are recognized and are assigned a unique number representation in the memory.

At second pass, using the symbol values, the machine code is built. The instruction's addresses are updated from the symbol table.

If any errors are found during first pass (and second), the program will continue to the next file.# Two-Pass-Assembler
