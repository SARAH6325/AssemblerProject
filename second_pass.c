#include "header_in_all_file.h"
#include "external_declaration.h"
#include "declerations_of_functions.h"
#include "utils.h"
extern unsigned int data_image[MACHINE_RAM];

void second_pass(FILE * file_after_layout , char * filename)
{
    int line_cnt = 1;
    char curr_line[MAX_LINE_LEN] = {0};
    IC = 0;
    
    while(fgets(curr_line, MAX_LINE_LEN, file_after_layout) != NULL) /* Read lines until end of file */
    {
        error = NO_ERROR;
        if(!disregard(curr_line)) /*if the line is empty or ; continue to the next line*/
            operates_line_second_pass(curr_line); /* Analyze one line at a time */
        if(is_error()) 
        { 
            is_errors_found = true;
            report_errors_in_line(line_cnt);
        }
        line_cnt++;
    }
    if(!is_errors_found)
        write_output_files(filename);

    free_labels(&symbols_table);
    free_ext(&ext_list);
}

/* This function analyzes and extracts information needed for the second pass from a given line */
void operates_line_second_pass(char * line)
{
    int guidance_sentence_type = UNKNOWN_TYPE;
    int command_type = UNKNOWN_COMMAND;
    char curr_word[MAX_LINE_LEN] = {0};
    line = cut_from_left(line); /*Removing spaces and tabs until the first word*/
    if(end_of_line(line)) /*If we got an empty line, we will skip to the next line*/ 
        return; 

    copy_token(curr_word, line);
    if(is_label(curr_word, COLON))
    { 
        /* If it's a label, skip it */
        line = next_token(line);
        copy_token(curr_word, line);
    }
    /*Checking if the word we received is a guidance sentence*/
    if((guidance_sentence_type = is_guidance_sentence(curr_word)) != NOT_FOUND)
    {
        line = next_token(line);
        if(guidance_sentence_type == ENTRY)
        {
            copy_token(curr_word, line);
            make_entry(symbols_table, curr_word); /* Creating an entry for the symbol */
        }
    }
    /* Checking if the word we received is an instruction sentence*/
    else if((command_type = is_command(curr_word)) != NOT_FOUND)
    {
        line = next_token(line);
        handle_command_second_pass(command_type, line);
    }
}

/* This function writes all 3 output files (if they should be created)*/
int write_output_files(char * rostral)
{
    FILE * file_name;

    file_name = open_file(rostral, FILE_OBJECT,WRITING );
    write_output_object(file_name);

    if(entry_exists) 
    {
        file_name = open_file(rostral, FILE_ENTRY,WRITING);
        write_output_entry(file_name);
    }

    if(extern_exists)
    {
        file_name = open_file(rostral, FILE_EXTERN,WRITING);
        write_output_extern(file_name);
    }

    return NO_ERROR;
}

/* This function writes the .ob file output. */
void write_output_object(FILE * file_after_layout)
{
    int i;
    unsigned int address = MEMORY_START;
    char * first_param = decimal_to_base(IC), * second_param = decimal_to_base(DC);

    fprintf(file_after_layout, "%s\t%s\n\n", first_param, second_param);
    free(first_param);
    free(second_param);

    for(i = 0; i < IC; address++, i++) /* Instructions memory */
    {
        first_param = decimal_to_base(address);
        second_param = decimal_to_base(instructions[i]);

        fprintf(file_after_layout, "%s\t%s\n", first_param, second_param);

        free(first_param);
        free(second_param);
    }

    for(i = 0; i < DC; address++, i++) /* Data memory */
    {
        first_param = decimal_to_base(address);
        second_param = decimal_to_base(data_image[i]);

        fprintf(file_after_layout, "%s\t%s\n", first_param, second_param);

        free(first_param);
        free(second_param);
    }

    fclose(file_after_layout);
}

/* This function writes the output of the .ent file. */
void write_output_entry(FILE * file_after_layout)
{
    char * wanted_base_address;
    ptr_label label = symbols_table;
    /* Go through symbols table and print only symbols that have an entry */
    while(label)
    {
        if(label -> entry)
        {
            wanted_base_address = decimal_to_base(label -> address);
            fprintf(file_after_layout, "%s\t%s\n", label -> name, wanted_base_address);
            free(wanted_base_address);
        }
        label = label -> next;
    }
    fclose(file_after_layout);
}

/* 
 * This function writes the output of the .ext file. */
void write_output_extern(FILE * file_after_layout)
{
    char * wanted_base_address;
    ptr_external node = ext_list;

    /* Going through external circular linked list and pulling out values */
    do
    {
        wanted_base_address = decimal_to_base(node -> address);
        fprintf(file_after_layout, "%s\t%s\n", node -> name, wanted_base_address); /* Printing to file */
        free(wanted_base_address);
        node = node -> next;
    } 
    while(node != ext_list);
    
    fclose(file_after_layout);
}


/* This function opens a file with writing permissions, given the original input filename 
 * and the wanted file extension (by type)
 */
FILE * open_file(char * file_name, int type_of_file ,int open_file_for)
{
    FILE * file;
    file_name = create_file_name(file_name, type_of_file); /* Creating filename with extension */
    if(open_file_for == WRITING)
    {
       file = fopen(file_name, "w"); 
       free(file_name);

       if(file == NULL)
       {
            error = CANNOT_OPEN_FILE;
            return NULL;
       }
    return file;
    }
    
    else if(open_file_for == READING)
    {
        file = fopen(file_name, "r"); 
        free(file_name);

        if(file == NULL)
        {
            error = CANNOT_OPEN_FILE;
            return NULL;
        }
    return file;
    }
    return NULL;
}

/* This function creates a file name by appending suitable extension (by type) to the original string */
char * create_file_name(char * rostral, int type_of_file)
{

    char * rename =(char *)malloc(strlen(rostral) * sizeof(char) + 10);

    if(rename == NULL)
    {
        fprintf(stderr,"memory allocation failed\n");
        exit(ERROR);
    }

    strcpy(rename, rostral); /*Copying the original file name*/
    switch(type_of_file)/*Adding to the name of the file its extension*/
    {
        case FILE_INPUT:
            strcat(rename, ".am");
            break;
        case FILE_OBJECT:
            strcat(rename, ".ob");
            break;
        case FILE_ENTRY:
            strcat(rename, ".ent");
            break;
        case FILE_EXTERN:
            strcat(rename, ".ext");
            break;
        case FILE_ORIGINAL:
            strcat(rename, ".as");
        
   }
   return rename;
}

/* This function determines if source and destination operands exist by opcode */
void check_operands_exist(int type, bool * is_source, bool * is_destination)
{
    int num_of_arg;
    num_of_arg = num_operands_of_command(type);

    switch(num_of_arg)
    {   
        case NO_OP_CMD:
            *is_source = false;
            *is_destination = false;
            break;
        
        case ONE_OP_CMD:
            *is_source = false;
            *is_destination = true;
            break;
            
        case TWO_OP_CMD:
            *is_source = true;
            *is_destination = true;
     }          
}

/* This function handles commands for the second pass - encoding additional words */
int handle_command_second_pass(int type, char * line)
{
    char first_operand[MAX_LINE_LEN], second_operand[MAX_LINE_LEN]; 
    char * source = first_operand, * target = second_operand;
    bool is_exists_source, is_exists_target;
    int source_method = METHOD_UNKNOWN, target_method = METHOD_UNKNOWN;
    is_exists_source = is_exists_target = false; 

    check_operands_exist(type, &is_exists_source, &is_exists_target);

    /* Extracting source and destination addressing methods */
    if(is_exists_source)
        source_method = take_out_bits(instructions[IC], SRC_METHOD_START_POS, SRC_METHOD_END_POS);
    if(is_exists_target)
        target_method = take_out_bits(instructions[IC], DEST_METHOD_START_POS, DEST_METHOD_END_POS);

    /* Matching src and dest pointers to the correct operands (first or second or both) */
    if(is_exists_source || is_exists_target)
    {
        line = next_list_token(first_operand, line);
        if(is_exists_source && is_exists_target) /* There are 2 operands */
        {
            line = next_list_token(second_operand, line);
            next_list_token(second_operand, line);
        }
        else
        {
            target = first_operand; /* If there's only one operand, it's a destination operand */
            source = NULL;
        }
    }

    IC++; /* The first word of the command was already encoded in this IC in the first pass */
    return encode_additional_words(source, target, is_exists_source, is_exists_target, source_method, 
           target_method);
}

/* This function encodes the additional words of the operands to instructions memory */
int encode_additional_words(char * source, char * target, bool is_exists_source, bool is_exists_target, int 
source_method, int target_method) 
{
    /* There's a special case where 2 register operands share the same additional word */
    if(is_exists_source && is_exists_target && source_method == METHOD_REGISTER && 
      target_method == METHOD_REGISTER)
        encode_instructions(build_register_word(false, source) | build_register_word(true, target));
    
    else
    {
        if(is_exists_source) 
            encode_additional_word(false, source_method, source);
        if(is_exists_target) 
            encode_additional_word(true, target_method, target);
    }
    return is_error();
}

/* This function builds the additional word for a register operand */
unsigned int build_register_word(bool is_dest, char * reg)
{
    unsigned int word = (unsigned int) atoi(reg + 1); /* Getting the register's number */
    /* Inserting it to the required bits (by source or destination operand) */
    if(!is_dest)
        word <<= BITS_IN_REGISTER;
    word = put_in_are(word, ABSOLUTE);
    return word;
}

/* This function encodes a given label (by name) to memory */
void encode_label(char * label_name)
{
    unsigned int word; /* The word to be encoded */

    if(is_existing_label(symbols_table, label_name)) 
    { 
        word = get_label_address(symbols_table, label_name);
        if(is_external_label(symbols_table, label_name)) 
        { 
            /* Adding external label to external list (value should be replaced in this address) */
            add_ext(&ext_list, label_name, IC + MEMORY_START);
            word = put_in_are(word, EXTERNAL);
        }
        else
            word = put_in_are(word, RELOCATABLE); /* If it's not an external label, then it's relocatable */

        encode_instructions(word); /* Encode word to memory */
    }
    else /* It's an error */
    {
        IC++;
        error = COMMAND_LABEL_DOES_NOT_EXIST;
    }
}

/* This function encodes an additional word to instructions memory, given the addressing method */
void encode_additional_word(bool is_dest, int method, char * operand)
{
    unsigned int word = EMPTY_WORD; /* An empty word */
    const char delim = '.';
    char * temp;

    switch(method)
    {
        case METHOD_IMMEDIATE: /* Extracting immediate number */
            word = (unsigned int) atoi(operand + 1);
            word = put_in_are(word, ABSOLUTE);
            encode_instructions(word);
            break;

        case METHOD_DIRECT:
            encode_label(operand);
            break;

        case METHOD_STRUCT: /* Before the dot there should be a label, and after it a number */
            temp = strchr(operand, delim);
            *temp = '\0';
            encode_label(operand); /* Label before dot is the first additional word */
            *temp++ = delim;
            word = (unsigned int) atoi(temp);
            word = put_in_are(word, ABSOLUTE);
            encode_instructions(word); /* The number after the dot is the second */
        break;

        case METHOD_REGISTER:
            word = build_register_word(is_dest, operand);
            encode_instructions(word);
    }
}

