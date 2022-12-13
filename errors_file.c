#include "declerations_of_constants.h"
#include "header_in_all_file.h"
#include "external_declaration.h"
#include "utils.h"
/* This function receives line number as a parameter and prints the details of the specific error */
void report_errors_in_line(int line_cnt)
{
   /* fprintf(stderr, "ERROR (line %d): ", line_cnt);*/

    switch(error)
    {
        case SYNTAX_ERR:
            fprintf(stderr, "first non-blank character must be a letter or a dot.\n");

            break;

        case LABEL_ALREADY_EXISTS:
            fprintf(stderr, "label already exists.\n");

            break;

        case LABEL_TOO_LONG:
            fprintf(stderr, "label is too long (LABEL_MAX_LENGTH: %d).\n", SIZE_FIRST_CMD);

            break;

        case LABEL_INVALID_FIRST_CHAR:
            fprintf(stderr, "label must start with Letter.\n");

            break;

        case LABEL_ONLY_ALPHANUMERIC:
            fprintf(stderr, "label must only contain alphanumeric characters.\n");

            break;

        case LABEL_CANT_BE_COMMAND:
            fprintf(stderr, "label can't have the same name as a command.\n");

            break;

        case LABEL_CANT_BE_REGISTER:
            fprintf(stderr, "label can't have the same name as a register.\n");

            break;

        case LABEL_ONLY:
            fprintf(stderr, "label must be followed by a command or a directive.\n");

            break;

        case DIRECTIVE_NO_PARAMS:
            fprintf(stderr, "directive must have parameters.\n");

            break;

        case DIRECTIVE_INVALID_NUM_PARAMS:
            fprintf(stderr, "illegal number of parameters for a directive.\n");

            break;

        case DATA_COMMAS_IN_A_ROW:
            fprintf(stderr, "incorrect usage of commas in a .data directive.\n");

            break;

        case DATA_EXPECTED_NUM:
            fprintf(stderr, ".data expected a numeric parameter.\n");

            break;

        case DATA_EXPECTED_COMMA_AFTER_NUM:
            fprintf(stderr, ".data expected a comma after a numeric parameter.\n");

            break;

        case DATA_UNEXPECTED_COMMA:
            fprintf(stderr, ".data got an unexpected comma after the last number.\n");

            break;

        case STRING_TOO_MANY_OPERANDS:
            fprintf(stderr, ".string must contain exactly one parameter.\n");

            break;

        case STRING_OPERAND_NOT_VALID:
            fprintf(stderr, ".string operand is invalid.\n");

            break;

        case STRUCT_INVALID_NUM:
            fprintf(stderr, ".struct first parameter must be a number.\n");

            break;

        case STRUCT_EXPECTED_STRING:
            fprintf(stderr, ".struct must have 2 parameters.\n");

            break;

        case STRUCT_INVALID_STRING:
            fprintf(stderr, ".struct second parameter is not a string.\n");

            break;

        case STRUCT_TOO_MANY_OPERANDS:
            fprintf(stderr, ".struct must not have more than 2 operands.\n");

            break;

        case EXPECTED_COMMA_BETWEEN_OPERANDS:
            fprintf(stderr, ".struct must have 2 operands with a comma between them.\n");

            break;

        case EXTERN_NO_LABEL:
            fprintf(stderr, ".extern directive must be followed by a label.\n");

            break;

        case EXTERN_INVALID_LABEL:
            fprintf(stderr, ".extern directive received an invalid label.\n");

            break;

        case EXTERN_TOO_MANY_OPERANDS:
            fprintf(stderr, ".extern must only have one operand that is a label.\n");

            break;

        case COMMAND_NOT_FOUND:
            fprintf(stderr, "invalid command or directive.\n");

            break;

        case COMMAND_UNEXPECTED_CHAR:
            fprintf(stderr, "invalid syntax of a command.\n");

            break;

        case COMMAND_TOO_MANY_OPERANDS:
            fprintf(stderr, "command can't have more than 2 operands.\n");

            break;

        case COMMAND_INVALID_METHOD:
            fprintf(stderr, "operand has invalid addressing method.\n");

            break;

        case COMMAND_INVALID_NUMBER_OF_OPERANDS:
            fprintf(stderr, "number of operands does not match command requirements.\n");

            break;

        case COMMAND_INVALID_OPERANDS_METHODS:
            fprintf(stderr, "operands' addressing methods do not match command requirements.\n");

            break;

        case ENTRY_LABEL_DOES_NOT_EXIST:
            fprintf(stderr, ".entry directive must be followed by an existing label.\n");

            break;

        case ENTRY_CANT_BE_EXTERN:
            fprintf(stderr, ".entry can't apply to a label that was defined as external.\n");

            break;

        case COMMAND_LABEL_DOES_NOT_EXIST:
            fprintf(stderr, "label does not exist.\n");

            break;

        case CANNOT_OPEN_FILE:
            fprintf(stderr, "there was an error while trying to open the requested file.\n");
    }
}

