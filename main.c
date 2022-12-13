#include "utils.h"
#include"header_in_all_file.h"
#include "structs.h"
#include "declerations_of_functions.h"
#include "declerations_of_constants.h"
#include "external_declaration.h"

unsigned int data_image[MACHINE_RAM];
unsigned int instructions[MACHINE_RAM];

int IC;
int DC;
int error;
ptr_label symbols_table;
ptr_external ext_list;
bool entry_exists, extern_exists, is_errors_found;

const char symbols[SIZE_OF_SYMBOLS] = {
        '!', '@', '#', '$', '%', '^', '&', '*', '<', '>', 'a', 'b', 'c',
        'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
        'q', 'r', 's', 't', 'u', 'v'};

const char * directives[] = {
        ".data", ".string", ".struct", ".entry", ".extern"
};

int main(int argc,char ** argv)
{
    int i;
    FILE * file_des = NULL , * file_after_lay = NULL;
    char * as_file_name = "", * am_file_name = "";
   
    if(argc == MIN_FILE)
    {
        fprintf(stderr,"You must write a file name in command line\n");
        exit(ERROR);
         
    }
    for(i = 1; i < argc; i++)
    {

        file_des = open_file(argv[i], FILE_ORIGINAL, READING);
        am_file_name = create_file_name(argv[i],FILE_INPUT);

        file_after_lay = fopen(am_file_name, "w");
        if(file_after_lay == NULL)
        {
            fprintf(stderr,"Error: file %s is inaccessible for reading. \n", am_file_name);
            exit(ERROR);
        }
        operates_input(file_des , file_after_lay);
        rewind(file_after_lay);
        fclose(file_after_lay);
        file_after_lay = fopen(am_file_name, "r");
     
       
        printf("--------- Started assembling process ----------\n\n" );

        reset_global_vars();
        first_pass(file_after_lay);
        if(!is_errors_found)
        {
            /* procceed to second pass */
            rewind(file_after_lay);
            second_pass(file_after_lay, argv[i]);
        }

        else
            report_errors_in_line(CANNOT_OPEN_FILE);
           
        printf("\n\n--------- Finished sassembling process ---------\n\n");
           
    }
   

    free(am_file_name);
    fclose(file_des);
    fclose(file_after_lay);
   
return 0;    
}

void reset_global_vars()
{
    symbols_table = NULL;
    ext_list = NULL;
    entry_exists = extern_exists = is_errors_found = false;
}
