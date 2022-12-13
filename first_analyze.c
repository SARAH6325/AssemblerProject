#include "external_declaration.h"
#include "utils.h"
#include "declerations_of_constants.h"
#ifndef FIRST_ANALYSE_H
#define FIRST_ANALYSE_H

cmd_table global_table[AMOUNT_COMANDS] =
                {{MOV, TWO_OP_CMD, ALL_NO_IMMD},
                {CMP, TWO_OP_CMD, ALL_ALL},
                {ADD, TWO_OP_CMD, ALL_NO_IMMD},
                {SUB, TWO_OP_CMD, ALL_NO_IMMD},
                {NOT, ONE_OP_CMD, NO_IMMD},
                {CLR, ONE_OP_CMD, NO_IMMD},
                {LEA, TWO_OP_CMD, NO_REG_OR_IMMD_NO_IMMD},
                {INC, ONE_OP_CMD, NO_IMMD},
                {DEC, ONE_OP_CMD, NO_IMMD},
                {JMP, ONE_OP_CMD, NO_IMMD},
                {BNE, ONE_OP_CMD, NO_IMMD},
                {GET, ONE_OP_CMD, NO_IMMD},
                {PRN, ONE_OP_CMD, ALL},
                {JSR, ONE_OP_CMD, NO_IMMD},
                {RTS, NO_OP_CMD, NONE},
                {HLT, NO_OP_CMD, NONE}
                };


/* Checking whether the operand is of the direct register addressing type. */
bool direct_register_address(char * arg)
{
    if(is_register(arg))
       return true;
   
    return false;    
}

/* Checking whether the operand is of immediate address type. */
bool immediate_address(char * arg)
{
    if(arg[0] == '#')
    {
        arg += 1;
        if(is_number(arg) != false)
       return true;
    }
   
    return false;  
}

/* Checking whether the operand is of direct address type. */
bool direct_address(char * arg)
{
    if(is_label(arg , false))
        return true;
    return false;    
       
}

/* Checking whether the operand is of the record access address type. */
bool address_access_to_the_record(char * arg)
{
    char * first , * second;
    const char delim[2] = ".";
    first = strtok(arg , delim);
    if(is_label(first , false))
    {
        second = first + strlen(first) + 1;
        if(strlen(second) == 1)
            if((strcmp(second , "1") == 0) || (strcmp(second , "2") == 0))
                return true;
    }
   
    return false;
}

#endif
