#include "header_in_all_file.h"
#include "declerations_of_constants.h"
#ifndef STRUCTS_H
#define STRUCTS_H
/* Data array of words */
extern unsigned int data_image[]; 
/* Instructions array of words */
extern unsigned int instructions[]; 

typedef struct cmd_table
{
  opcode commands;
  num_of_operands operand;
  operands_type type_op;
 
} cmd_table;

/*Linked list of labels for the macro layout and a pointer to that list */
typedef struct node * ptr;
typedef struct node
{
    char macro_name[SIZE_FIRST_CMD];
    char macro_data[MAX_LINE_LEN * NUM_LINES_IN_MACRO];
    ptr next;

}item;

/* Defining linked list of labels and a pointer to that list */
typedef struct struct_labels * ptr_label;
typedef struct struct_labels 
{
	char name[SIZE_FIRST_CMD]; 
	unsigned int address;
	bool external;
	bool entry;  
	bool in_action_statement;  
	ptr_label next; 
}Labels;

/*Defining a circular double-linked list to store each time the program uses an extern label, and a pointer to that list */
typedef struct ext * ptr_external;
typedef struct ext 
{
    char name[SIZE_FIRST_CMD]; 
    unsigned int address; 
    ptr_external next; 
    ptr_external prev;
} ext;

#endif

