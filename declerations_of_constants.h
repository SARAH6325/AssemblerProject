#ifndef ASSEMBLER_H
#define ASSEMBLER_H


/* --------------------------------- Main constant ---------------------------------*/

#define as_ext ".as"
#define am_ext ".am"
#define ent_ext ".ent"
#define obj_ext ".ob"
#define ext_ext ".ext"

/* --------------------------------- Defaults ---------------------------------*/

#define MEMORY_START 100
#define EMPTY_WORD 0
#define EXTERNAL_DEFAULT_ADDRESS 0
#define NOT_FOUND -1
#define NO_ERROR 0
#define ERROR 1
 
/* --------------------------------- Limitations ---------------------------------*/

#define MIN_FILE 1
#define MAX_LINE_LEN 81
#define MINIMUM_LABEL_LENGTH_WITH_COLON 2
#define MINIMUM_LABEL_LENGTH_WITHOUT_COLON 1
#define SIZE_FIRST_CMD 30
#define NUM_OF_REGISTER 8
#define MAX_COMMAND_LENGTH 4 
#define MIN_COMMAND_LENGTH 3 
#define SIZE_OF_SYMBOLS 32


/*--------------------------------- Other Constants ---------------------------------*/

#define AMOUNT_GUIDANCE_SENTENCE  5 /* number of existing guidance sentence*/
#define AMOUNT_COMANDS 16 /* number of existing commands */
#define FIRST_STRUCT_FIELD 1 /* Index of first struct field */
#define SECOND_STRUCT_FIELD 2 /* Index of second struct field */

/* Bit-related info */
#define BITS_IN_WORD 10
#define BITS_IN_OPCODE 4
#define BITS_IN_METHOD 2
#define BITS_IN_ARE 2
#define BITS_IN_REGISTER 4
#define BITS_IN_ADDRESS 8

/* Addressing methods bits location in the first word of a command */
#define SRC_METHOD_START_POS 4
#define SRC_METHOD_END_POS 5
#define DEST_METHOD_START_POS 2
#define DEST_METHOD_END_POS 3

#define MACHINE_RAM 2000

/* macro layout */
#define NUM_LINES_IN_MACRO 10

/*---------------------------------  Enums --------------------------------- */

/* Directives types */
enum directives {DATA, STRING, STRUCT, ENTRY, EXTERN, UNKNOWN_TYPE};

/* Enum of commands ordered by their opcode */
typedef enum {MOV,CMP,ADD,SUB,NOT,CLR,LEA,INC,DEC,JMP,BNE,GET,PRN,JSR,RTS,HLT,UNKNOWN_COMMAND} opcode;

typedef enum {NO_OP_CMD,ONE_OP_CMD,TWO_OP_CMD} num_of_operands;

/* ALL_NO_IMMD - Source: 0, 1, 2, 3  Destination: 1, 2, 3
 * ALL_ALL - Source: 0, 1, 2, 3  Destination: 0, 1, 2, 3
 * NO_REG_OR_IMMD_NO_IMMD - Source: 1, 2  Destination: 1, 2, 3
 * ALL - Source: NONE  Destination: 0, 1, 2, 3
*/
typedef enum {NONE = -1,ALL_NO_IMMD,ALL_ALL,NO_REG_OR_IMMD_NO_IMMD,NO_IMMD,ALL} operands_type;

/* Enum of all errors that can be outputted */
enum errors {
    SYNTAX_ERR = 1, LABEL_ALREADY_EXISTS, LABEL_TOO_LONG, LABEL_INVALID_FIRST_CHAR, LABEL_ONLY_ALPHANUMERIC,
    LABEL_CANT_BE_COMMAND, LABEL_ONLY, LABEL_CANT_BE_REGISTER,
    DIRECTIVE_NO_PARAMS, DIRECTIVE_INVALID_NUM_PARAMS, DATA_COMMAS_IN_A_ROW, DATA_EXPECTED_NUM,
    DATA_EXPECTED_COMMA_AFTER_NUM, DATA_UNEXPECTED_COMMA,
    STRING_TOO_MANY_OPERANDS, STRING_OPERAND_NOT_VALID, STRUCT_EXPECTED_STRING,
    STRUCT_INVALID_STRING, EXPECTED_COMMA_BETWEEN_OPERANDS, STRUCT_INVALID_NUM, STRUCT_TOO_MANY_OPERANDS,
    EXTERN_NO_LABEL, EXTERN_INVALID_LABEL, EXTERN_TOO_MANY_OPERANDS,
    COMMAND_NOT_FOUND, COMMAND_UNEXPECTED_CHAR, COMMAND_TOO_MANY_OPERANDS,
    COMMAND_INVALID_METHOD, COMMAND_INVALID_NUMBER_OF_OPERANDS, COMMAND_INVALID_OPERANDS_METHODS,
    ENTRY_LABEL_DOES_NOT_EXIST, ENTRY_CANT_BE_EXTERN, COMMAND_LABEL_DOES_NOT_EXIST,
    CANNOT_OPEN_FILE
};

/* When we need to specify if label should contain a colon or not */
enum {NO_COLON, COLON};

/* Addressing methods ordered by their code */
enum methods {METHOD_IMMEDIATE, METHOD_DIRECT, METHOD_STRUCT, METHOD_REGISTER, METHOD_UNKNOWN};

/* A/R/E modes ordered by their numerical value */
enum ARE {ABSOLUTE, EXTERNAL, RELOCATABLE};

/* Types of files that indicate what is the desirable file extension */
enum filetypes {FILE_INPUT, FILE_OBJECT, FILE_ENTRY, FILE_EXTERN, FILE_ORIGINAL};
enum opening{WRITING=1,READING};

#endif

