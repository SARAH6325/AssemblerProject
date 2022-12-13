#include "header_in_all_file.h"
#include "declerations_of_functions.h"
#include "declerations_of_constants.h"
#include "external_declaration.h"
#include "utils.h"

/* This functions returns one if there's an error */
int is_error()
{
    return error != NO_ERROR;
}

/* This function Checking for the end of line */
int end_of_line(char * line)
{
    return line == NULL || *line == '\0' || *line == '\n';
}

/* This function skips spaces of a string and returns a pointer to the first non-blank character */
char * cut_from_left(char * word)
{
    if(word == NULL) 
        return NULL;
    while(isspace(*word) && *word != '\0')
        word++;
    return word;
}

/* This function disregards a line if it's a blank line or a comment */
int disregard(char * word)
{
    word = cut_from_left(word);
    return *word == ';' || *word == '\0' || *word == '\n';
}

/* This function extracts bits, given start and end positions of the bit-sequence (0 is LSB) */
unsigned int take_out_bits(unsigned int word, int start, int end)
{
    unsigned int result;
    int len = end - start + 1; /* Length of bit-sequence */
    unsigned int mask = (int) pow(2, len) - 1; 

    mask <<= start; /* Moving mask to place of extraction */
    result = word & mask; /* The bits are now in their original position, and the rest is 0's */
    result >>= start; /* Moving the sequence to LSB */
    return result;
}

/* This function coverting a decimal number to the given 32 base */
char * decimal_to_base(unsigned int num)
{
    int j, count = 0, place = 0, position = 1, i = 0;
    unsigned int abs = num;
    char temp;
    char bin[BITS_IN_WORD];
    char * base = malloc(3 * sizeof(char));/*Allocates place for the num after the conversion to 32 base*/
    if(base == NULL)/*Check that the malloc succeeded*/
    {
        fprintf(stderr,"memory allocation failed\n");
        exit(ERROR);
    }
    while(i < BITS_IN_WORD)
    {
        bin[i] = abs % 2 +'0';
        abs /= 2;
        i++;
    }
    for (j = 0; j< 5; j++)
    {
        temp = bin[BITS_IN_WORD -1 - j];
        bin[BITS_IN_WORD -1 - j] = bin[j];
        bin[j] = temp;
    }
    for (i = BITS_IN_WORD -1; i>-1; i--)
    {
        place += pow(2,count)*(bin[i]-'0');
        count++;
        if(count == (BITS_IN_WORD/2))
        {
            base[position] = symbols[place];
            position--;
            count = 0;
            place = 0;
        }
    }
    base[2] ='\0';
    return base;
}

/* This function checks if the number is an integer. */
bool is_number(char * num)
{
    int i = 0;
    if(num[0] == '-' || num[0] == '+')
        i++;
    while(i <= strlen(num) - 1)
    {
        if(!isdigit(num[i])) /*if it will be a double number will also return false */
            return false; /* The string is not a legal number */
        i++;    
    }    
return true; /* The given string is a valid number */      
}

/* This function inserts given A/R/E 2 bits into given bit_sequence */
unsigned int put_in_are(unsigned int bit_sequence, int are)
{
    return (bit_sequence << BITS_IN_ARE) | are; 
}


/* This function inserts a given word to the memory */
void encode_instructions(unsigned int word)
{
    instructions[IC++] = word;
}

/*
 * This function going over the global table and checking according to the type we received the 
 * number of operands it should receive.
 */
int num_operands_of_command(int type)
{
    int index;
    for(index = 0; index < AMOUNT_COMANDS; index++)
        if(global_table[index].commands == type)
           return global_table[index].operand;
    return NOT_FOUND;        
}
/* 
 * This function copies the next token of a list  to a destination array.
 * Returns a pointer to the first character after the token 
 */
char * next_list_token(char * destination, char * current_line)
{
    char * temp_line = destination;
    const char delim[2] = ",";
    if(end_of_line(current_line)) /* if we got an empty line */
    {
        destination[0] = '\0';
        return NULL;
    }

    if(isspace(*current_line)) /* skip the whitespaces if there are */
        current_line = cut_from_left(current_line);

    if(*current_line == ',') /* A comma deserves a separate, single-character token */
    {
        strcpy(destination, delim);
        return ++current_line;
    }
    /*copy token until the comma or whitespace or the end of the line */
    while(!end_of_line(current_line) && *current_line != ',' && !isspace(*current_line))
    {
        *temp_line = *current_line;
        temp_line++;
        current_line++;
    }
    *temp_line = '\0';
    return current_line;
}

/* 
 * This function copies supposedly next string into dest array and returning a pointer to the
 * first character after it.
 */
char * next_token_string(char * destination, char * current_line)
{
    const char delim = '"';
    char temp_line[MAX_LINE_LEN] = {0};
    current_line = next_list_token(destination, current_line);
    if(*destination != delim) 
        return current_line;
    while(!end_of_line(current_line) && destination[strlen(destination) - 1] != delim)
    {
        current_line = next_list_token(temp_line, current_line);
        if(current_line) 
            strcat(destination, temp_line);
    }
    return current_line;
}

/* This function returns a pointer to the start of next token in the line */
char * next_token(char * word)
{
    if(word == NULL) 
        return NULL;
        
    while(!isspace(*word) && !end_of_line(word))
         word++; /* Skip rest of characters in the current token (until a space) */
    word = cut_from_left(word); 
    if(end_of_line(word)) 
        return NULL;
    return word;
}

/* This function copies the next token (until a space or end of line) to a destination string */
void copy_token(char * destination, char * current_line)
{
    int i = 0;
    if(destination == NULL || current_line == NULL) 
        return;
        
    /* Copying a word until the end of the word we received */
    while(i < MAX_LINE_LEN && !isspace(current_line[i]) && current_line[i] != '\0') 

    {
        destination[i] = current_line[i];
        i++;
    }
    destination[i] = '\0';
}


