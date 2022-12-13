#include "header_in_all_file.h"
#include "external_declaration.h"
#include "declerations_of_functions.h"
#include "utils.h"

extern unsigned int data_image[MACHINE_RAM];

/*This function manages all the activities of the first pass */
void first_pass(FILE * file_pointer)
{
    char curr_line[MAX_LINE_LEN] = {0};
    int line_cnt = 1;
    IC = 0; /* Initializing instructions counter */
    DC = 0; /* Initializing data counter */

    while(fgets(curr_line, MAX_LINE_LEN, file_pointer) != NULL) /* Read lines until end of file */
    {
        error = NO_ERROR; /* Reset the error global var before parsing each curr_line */
        if(!disregard(curr_line)) /* Ignore curr_line if it's blank or ; */
            operates_line_first_pass(curr_line);
        if(is_error()) 
        {
            is_errors_found = true; 
            report_errors_in_line(line_cnt); /* Output the error */
        }
        line_cnt++;
        
    }
    /*calculating the real final addresses */
    set_final_addresses(symbols_table, MEMORY_START, false); /* Instruction symbols will have addresses 
    that start from 100  */
    set_final_addresses(symbols_table, IC + MEMORY_START, true); /* Data symbols will have addresses that 
    start fron 100 + IC */
}

/*This function receives a row from a file and parses it*/
void operates_line_first_pass(char * curr_line)
{
    const char delim = '.';
    int guidance_sentence_type = UNKNOWN_TYPE;
    int command_type = UNKNOWN_COMMAND;
    bool was_label_in_line = false;
    ptr_label label_node = NULL;
    char current_token[MAX_LINE_LEN] = {0};
    
    curr_line = cut_from_left(curr_line); /*Removing spaces and tabs until the first word*/
    if(end_of_line(curr_line)) /*If we got an empty line, we will skip to the next line*/ 
        return;
    /*first non-blank character must be a letter or a dot*/ 
    if(!isalpha(*curr_line) && *curr_line != delim)
    { 
        error = SYNTAX_ERR;
        return;
    }

    copy_token(current_token, curr_line);
    if(is_label(current_token, COLON)) 
    { 
        was_label_in_line = true;
        label_node = add_label(&symbols_table, current_token, 0, false, false); 
        if(label_node == NULL) /* There was an error creating label */
            return;
        curr_line = next_token(curr_line); /* Skipping to beginning of next token */
        if(end_of_line(curr_line))
        {
            error = LABEL_ONLY; /*A current line cannot contain only a label*/
            return;
        }
        copy_token(current_token, curr_line); /* Proceed to next token */
    } 

    if(is_error())
        return;
        
    /*Checking whether the word we received is a guidance sentence*/
    if((guidance_sentence_type = is_guidance_sentence(current_token)) != NOT_FOUND) 
    {
        if(was_label_in_line)
        {
            if(guidance_sentence_type == EXTERN || guidance_sentence_type == ENTRY)
             { 
                /* we need to disregard creation of label before .entry or .extern */
                remove_label(&symbols_table, label_node->name);
                was_label_in_line = false;
            }
            else
                label_node -> address = DC; /* Address of data label is DC */
        }
        curr_line = next_token(curr_line);
        handle_guidance_sentence(guidance_sentence_type, curr_line);
    }
    /* Checking if the word we received is an instruction sentence*/
    else if((command_type = is_command(current_token)) != NOT_FOUND)
    {
        if(was_label_in_line)
        {
            /* Setting fields accordingly in label */
            label_node -> in_action_statement = true; 
            label_node -> address = IC;
        }
        curr_line = next_token(curr_line);
        handle_command(command_type, curr_line);
    }

    else
        error = COMMAND_NOT_FOUND; /* a curr_line must have a directive/command */
}

/*
 * This function handles all kinds of the guidance sentences and sends them to the right functions 
 * for analyzing them. 
 */ 
int handle_guidance_sentence(int type, char *curr_line)
{
    if(curr_line == NULL || end_of_line(curr_line)) /* All directives must have at least one parameter */
    {
        error = DIRECTIVE_NO_PARAMS;
        return ERROR;
    }

    switch(type)
    {
        case DATA:
            /* Handle .data directive and insert values separated by comma to the memory */
            return handle_data_guidance_sentence(curr_line);

        case STRING:
            /* Handle .string directive and insert all characters (including a '\0') to memory */
            return handle_string_guidance_sentence(curr_line);

        case STRUCT:
            /* Handle .struct directive and insert both number and string to memory */
            return handle_struct_guidance_sentence(curr_line);

        case ENTRY:
            /* Only check for syntax of entry (should not contain more than one parameter) */
            if(!end_of_line(next_token(curr_line))) /* If there's a next token (after the first one) */
            {
                error = DIRECTIVE_INVALID_NUM_PARAMS;
                return ERROR;
            }
            break;

        case EXTERN:
            /* Handle .extern directive */
            return handle_extern_directive(curr_line);
    }
    return NO_ERROR;
}

/*
 * This function analyzes a command, given the type and the sequence of characters starting after the command.
 * It will detect the addressing methods of the operands and will encode the first word of the command to the 
 * instructions memory.
 */
int handle_command(int type, char *curr_line)
{
    const char delim = ',';
    int first_method , second_method; /*These will hold the addressing methods of the operands*/
    bool is_first = false , is_second = false;
    char first_op[SIZE_FIRST_CMD], second_op[SIZE_FIRST_CMD]; /*These strings will hold the operands*/
    
    curr_line = next_list_token(first_op, curr_line);
    if(!end_of_line(first_op)) /* If first operand is not empty */
    {
        is_first = true; /* First operand exists*/
        curr_line = next_list_token(second_op, curr_line);
        if(!end_of_line(second_op)) /*If second operand(should hold temporarily a comma)is not empty*/
        {
            if(second_op[0] != delim) /* A comma must separate two operands of a command */
            {
                error = COMMAND_UNEXPECTED_CHAR;
                return ERROR;
            }

            else
            {
                curr_line = next_list_token(second_op, curr_line); 
                if(end_of_line(second_op)) /*If second operand is not empty*/
                {
                    error = COMMAND_UNEXPECTED_CHAR;
                    return ERROR;
                }
                is_second = true; /*Second operand exists*/
            }
       }
    }
    curr_line = cut_from_left(curr_line);
    if(!end_of_line(curr_line)) /* If the curr_line continues after two operands */
    {
        error = COMMAND_TOO_MANY_OPERANDS;
        return ERROR;
    }

    if(is_first)
        first_method = address_method(first_op); /*Checking the address method of the first operand*/
    if(is_second)
        second_method = address_method(second_op); /*Checking the address method of the second operand*/

    if(!is_error()) /*If there was no error while trying to parse addressing methods*/
    {
         /*If number of operands is valid for this specific command*/
        if(check_legal_num_of_opernds(type, is_first, is_second)) 
        {
            /*If addressing methods are valid for this specific command*/
            if(check_type_op(type, first_method, second_method)) 
            {
                /*Encode first word of the command to memory and increase IC by the number of additional words*/
                encode_instructions(build_first_word(type, is_first, is_second, first_method, second_method));
                IC += calculate_command_num_additional_words(is_first, is_second, first_method, second_method);
            }

            else
            {
                error = COMMAND_INVALID_OPERANDS_METHODS;
                return ERROR;
            }
        }
        else
        {
            error = COMMAND_INVALID_NUMBER_OF_OPERANDS;
            return ERROR;
        }
    }

    return NO_ERROR;
}

/*This function handles a .string directive by analyzing it and encoding it to data */
int handle_string_guidance_sentence(char * curr_line)
{
    char token[MAX_LINE_LEN] = {0};
    curr_line = next_token_string(token, curr_line);
    if(!end_of_line(token) && is_string(token)) 
    { 
        /*If token exists and it's a valid string*/
        curr_line = cut_from_left(curr_line);
        if(end_of_line(curr_line))
        {
            token[strlen(token) - 1] = '\0';/*Remove quotation marks*/
            write_string_to_data_image(token + 1);/*Encoding to data*/
        }

        else
        {
            error = STRING_TOO_MANY_OPERANDS;
            return ERROR;
        }

    }

    else /*Invalid string*/
    {
        error = STRING_OPERAND_NOT_VALID;
        return ERROR;
    }

    return NO_ERROR;
}

/* This function handles analyzing and encoding a .struct directive to data memory,
 * given the char sequence starting from after the ".struct"
*/
int handle_struct_guidance_sentence(char *curr_line)
{
    char token[MAX_LINE_LEN];
    const char delim = ',';
    /* Getting the first token into token array in the curr_line above */
    curr_line = next_list_token(token, curr_line); 

    if(!end_of_line(token) && is_number(token)) /* First token must be a number */
    {
        write_number_to_data_image(atoi(token)); /* Encode number to data */
        curr_line = next_list_token(token, curr_line);

        if(!end_of_line(token) && *token == delim) 
        { 
            /* There must be a comma between .struct operands */
            curr_line = next_token_string(token, curr_line); /* Get next token (second operand) */
            if(!end_of_line(token)) 
            { 
                /* There's a second operand */
                if(is_string(token))
                {
                    token[strlen(token) - 1] = '\0';/*Remove quotation marks*/
                    write_string_to_data_image(token + 1);/*Encoding to data*/
                } 
                else 
                {
                    error = STRUCT_INVALID_STRING;
                    return ERROR;
                }
            }
            else /* There is no second operand */
            {
                error = STRUCT_EXPECTED_STRING;
                return ERROR;
            }
        }
        else
        {
            error = EXPECTED_COMMA_BETWEEN_OPERANDS;
            return ERROR;
        }
    }
    else
    {
        error = STRUCT_INVALID_NUM;
        return ERROR;
    }
    if(!end_of_line(next_list_token(token, curr_line)))
    {
        error = STRUCT_TOO_MANY_OPERANDS;
        return ERROR;
    }
    return NO_ERROR;
}

/* This function parses parameters of a data directive and encodes them to memory */
int handle_data_guidance_sentence(char *curr_line)
{
    char token[SIZE_FIRST_CMD] = {0};

    /* These bool mark if there was a number or a comma before current token,
     * so that if there wasn't a number, then a number will be required and
     * if there was a number but not a comma, a comma will be required */
    bool number = false, comma = false;

    while(!end_of_line(curr_line))
    {
        curr_line = next_list_token(token, curr_line);

        if(strlen(token) > 0) /* Not an empty token */
        {
            if(!number) 
            { 
                /* if there wasn't a number before */
                if (!is_number(token))
                { 
                    /* then the token must be a number */
                    error = DATA_EXPECTED_NUM;
                    return ERROR;
                }

                else 
                {
                    number = true; /* A valid number was inputted */
                    comma = false; /* Resetting comma (now it is needed) */
                    write_number_to_data_image(atoi(token)); /* encoding number to data */
                }
            }

            else if (*token != ',') /* If there was a number, now a comma is needed */
            {
                error = DATA_EXPECTED_COMMA_AFTER_NUM;
                return ERROR;
            }

            else /* If there was a comma, it should be only once (comma should be false) */
            {
                if(comma) 
                {
                    error = DATA_COMMAS_IN_A_ROW;
                    return ERROR;
                }
                else
                {
                    comma = true;
                    number = false;
                }
            }

        }
    }
    if(comma == true)
    {
        error = DATA_UNEXPECTED_COMMA;
        return ERROR;
    }
    return NO_ERROR;
}

/* This function encodes a given number to data_image */
void write_number_to_data_image(int num)
{
    data_image[DC++] = (unsigned int) num;
}

/* This function encodes a given string to data_image */
void write_string_to_data_image(char * string)
{
    while(!end_of_line(string))
    {
        data_image[DC++] = (unsigned int) *string; /* Inserting a character to data_image array */
        string++;
    }
    data_image[DC++] = '\0'; /* Insert a null character to data */
}

/*This method returns the addressing method of the given operand*/
int address_method(char * operand)
{
    if(end_of_line(operand)) 
        return NOT_FOUND;

    if(immediate_address(operand) == true)
        return METHOD_IMMEDIATE;
    
    else if(direct_register_address(operand) == true)
        return METHOD_REGISTER;

    else if(direct_address(operand) == true) 
        return METHOD_DIRECT;

    else if(address_access_to_the_record(operand) == true)
        return METHOD_STRUCT;
    
    error = COMMAND_INVALID_METHOD;
    return NOT_FOUND;
}


/*
 * This method checks that the number of operands received is indeed equal to the number of operands 
 * the command should receive
 */
bool check_legal_num_of_opernds(int type, bool first, bool second) 
{
    int num_of_arg;
    num_of_arg = num_operands_of_command(type);

    switch(num_of_arg)
    {   
        case NO_OP_CMD:
            return !first && !second;
        
        case ONE_OP_CMD:
            return first && !second;
        
        case TWO_OP_CMD:
            return first && second;
    
    }
    return false;
}

/*This function checks that the received operands type matches the received command*/
bool check_type_op(int type , int src , int dest)
{
    /*Going over the global table and checking according to the type we received the type of operands 
    that the command can receive.*/
    int type_of_op , index;
    for(index = 0; index < AMOUNT_COMANDS; index++)
        if(global_table[index].commands == type)
            type_of_op = global_table[index].type_op;
            
    /*Checks that the received operands are indeed correct for this command*/
    if(type_of_op == ALL_NO_IMMD) 
    {
        if(src == NOT_FOUND || dest == NOT_FOUND || dest == METHOD_IMMEDIATE)
            return false;
    }
    else if(type_of_op == ALL_ALL)
    {
        if(src == NOT_FOUND || dest == NOT_FOUND)
            return false;
    }    
    
    else if(type_of_op == NO_REG_OR_IMMD_NO_IMMD)
    {
        if(src == NOT_FOUND || src == METHOD_IMMEDIATE || src == METHOD_REGISTER)
            return false;
		if(dest == NOT_FOUND || dest == METHOD_IMMEDIATE)
			return false; 
    }
    
    else if(type_of_op == NO_IMMD)
    {
        if(src == NOT_FOUND || src == METHOD_IMMEDIATE)
            return false;
    }
    
    else if(type_of_op == ALL)
    {
        if(src == NOT_FOUND)
            return false;
    }    

return true;
}

/* This function calculates number of additional words for a command */
int calculate_command_num_additional_words(int is_first, int is_second, int first_method, int second_method)
{
    int cnt = 0;
    if(is_first) cnt += num_words(first_method);
    if(is_second) cnt += num_words(second_method);

    /* If both methods are register, they will share the same additional word */
    if(is_first && is_second && first_method == METHOD_REGISTER && second_method == METHOD_REGISTER)     
        cnt--;

    return cnt;
}

/* This function encodes the first word of the command */
unsigned int build_first_word(int type, int is_first, int is_second, int first_method, int second_method)
{
    unsigned int word = 0;

    /* Inserting the opcode */
    word = type;

    word <<= BITS_IN_METHOD; /* Leave space for first addressing method */

    /* If there are two operands, insert the first */
    if(is_first && is_second)
        word |= first_method;

    word <<= BITS_IN_METHOD; /* Leave space for second addressing method */

    /* If there are two operands, insert the second. */
    if(is_first && is_second)
        word |= second_method;
    /* If not, insert the first one (a single operand is a destination operand). */
    else if(is_first)
        word |= first_method;

    word = put_in_are(word, ABSOLUTE); /* Insert A/R/E mode to the word */

    return word;
}

/* This function returns how many additional words an addressing method requires */
int num_words(int method)
{
    if(method == METHOD_STRUCT) /* Struct addressing method requires two additional words */
        return 2;
    return 1;
}

/* This function handles an .extern directive */
int handle_extern_directive(char *curr_line)
{
    char token[MAX_LINE_LEN] = {0}; 

    copy_token(token, curr_line); 
    if(end_of_line(token)) /* If the token is empty, then there's no label */
    {
        error = EXTERN_NO_LABEL;
        return ERROR;
    }
    if(!is_label(token, false)) /* The token should be a label (without a colon) */
    {
        error = EXTERN_INVALID_LABEL;
        return ERROR;  
    }  

    curr_line = next_token(curr_line);
    if(!end_of_line(curr_line))
    {
        error = EXTERN_TOO_MANY_OPERANDS;
        return ERROR;
    }

    /* Trying to add the label to the symbols table */
    if(add_label(&symbols_table, token, EXTERNAL_DEFAULT_ADDRESS, true) == NULL)
        return ERROR;
    return is_error();
}

