#include "utils.h"
#include "declerations_of_constants.h"
#include "external_declaration.h"
extern int error;

const char *commands[] = {
        "mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp", "bne",
        "get", "prn", "jsr", "rts", "hlt"
};

const char * reg_name[NUM_OF_REGISTER] ={
        "r0",
        "r1",
        "r2",
        "r3",
        "r4",
        "r5",
        "r6",
        "r7"
};

/* This method checks whether the received word is a register. */
bool is_register(char * word)
{
    int i;
    for(i = 0; i < NUM_OF_REGISTER; i++)
    {
    if(strcmp(word,reg_name[i]) == 0)
        return true;
    }
    return false;
}

/* This method checks if a word matches a directive name */
int is_guidance_sentence(char * word)
{
    if(word == NULL || *word != '.') 
        return NOT_FOUND;
    return find_index(word, directives, AMOUNT_GUIDANCE_SENTENCE);
}

/* This function returns whether a label can be defined with the specified name.*/
bool is_label(char * word , int colon)
{
    if(word == NULL)
        return false;
   
    if(strlen(word) < (colon ? MINIMUM_LABEL_LENGTH_WITH_COLON: MINIMUM_LABEL_LENGTH_WITHOUT_COLON))
        return false;
   
    /* if colon = true, there must be a colon at the end */    
    if(colon && word[strlen(word) - 1] != ':')
        return false;
   
    return(is_valid_label(word , colon));
}        

/* This method checks that the content of the label is indeed valid */ 
bool is_valid_label(char * word , int colon)
{
    int i;
    bool flag_digits = false;
    int word_len = strlen(word);  
   
    /*If the label is too long*/
    if(word_len > SIZE_FIRST_CMD)
    {
        if(colon)
           error = LABEL_TOO_LONG;
        return false;
    }
   
    /*Checking the first character - checking that it is not a number, and that it is an alphanumeric 
    character */
    if(!isalpha(*word) || isdigit(word[0]))
    {
        if(colon)
           error = LABEL_INVALID_FIRST_CHAR;
        return false;
    }

    /*Removing the colon from the label*/
    if(colon)
    {
        word[word_len - 1] = '\0';
        word_len--;
    }

   /*Checking the rest of the characters, without the first character which are indeed alphanumeric 
   characters*/
    for(i = 1; i < word_len; i++)
    {
        if(isdigit(word[i]))
            flag_digits = true;
           
        else if(!isalpha(word[i]))
        {
            /* It's not a label but it's an error only if someone put a colon at the end of the token */
            if(colon)
                error = LABEL_ONLY_ALPHANUMERIC;
            return false;
        }
    }

    if(!flag_digits) /* It can't be a command */
    {
        if(is_command(word) != NOT_FOUND)
        {
            if(colon)
               error = LABEL_CANT_BE_COMMAND; /* Label can't have the same name as a command */
            return false;
        }
    }

    if(is_register(word))
    {
        if(colon)
            error = LABEL_CANT_BE_REGISTER;
        return false;
    }

    return true;
}

int find_index(char * word, const char * array[], int size)
{
    int index;
    for(index = 0; index < size; index++)
        if (strcmp(word, array[index]) == 0)
            return index;
    return NOT_FOUND;
}

/* This method checks if a word matches a command name */
int is_command(char * word)
{
    int word_len = strlen(word);
    if(word_len > MAX_COMMAND_LENGTH || word_len < MIN_COMMAND_LENGTH)
        return NOT_FOUND;
    return find_index(word, commands, AMOUNT_COMANDS);
}

/* This method checks whether the received word is a command. */
bool is_cmd(char * word)
{
    int i;
    if(word == NULL)
        return false;   
    for(i = 0; i < AMOUNT_COMANDS; i++)
    {
        if(strcmp(word , commands[i]) == 0)
           return true;
    }
    return false;
}

/* This function checks if a given string is a valid string  */
bool is_string(char *word)
{
    const char delim = '"';
    if(word == NULL) 
        return false;

    if (*word == delim) /* starts with " */
        word++;
    else
        return false;

    while (*word && *word != delim) 
    { /* need to skip until end of string */
        word++;
    }

    if (*word != delim) /* a string must end with " */
        return false;

    word++;
    if (*word != '\0') /* string token must end after the ending " */
        return false;

    return true;
}

/* This method checks whether the received word is a "macro". */
bool is_macro(char * word)
{
    if(strcmp(word,"macro") == 0)
        return true;
    return false;
}

