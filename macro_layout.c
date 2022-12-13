#include "utils.h"
#include "declerations_of_constants.h"
#include "declerations_of_functions.h"
#include "header_in_all_file.h"
/*#include "structs.h"*/

int line_cnt = 1; 
 
/*This method goes through the list of macros and checks if the macro name already exists*/
ptr is_exist_macro(char * macro_name , ptr head)
{
    ptr curr = head;
    while(curr != NULL)
    {
        if(strcmp(macro_name , curr -> macro_name) == 0)
            return curr;
        curr = curr -> next;
    }
    return NULL;
}

/*This method returns the data of the macro*/
char * returns_macro_data(char * macro_name , ptr * head)
{
    ptr curr = * head;
    while(curr != NULL)
    {
        if(strcmp(macro_name , curr -> macro_name) == 0)
            return curr -> macro_data;
        curr = curr->next;
    }
    return NULL;
}

/*
  This method goes line by line until the end of the file, and each time writes to the post-deployment 
  file, every time it encounters a macro it looks for its name in the list of macros and writes its 
  contents to the file.
 */
void operates_input(FILE * filename , FILE  * file_after_lay)
{
    ptr head = NULL;
    ptr curr = NULL;
    const char delim[4] = " \t\n";
    char * first_word , * name_macr;
    char temp_line[MAX_LINE_LEN] = {0};
    char curr_line[MAX_LINE_LEN] = {0};
     /*maximum size of macro, ten lines , each line eighty one */
    char * macr_data = (char *)malloc(MAX_LINE_LEN * NUM_LINES_IN_MACRO);
    while(fgets(curr_line, sizeof(curr_line)-1, filename) != NULL)/*read the whole file*/
    {
        line_cnt++;
        /*Checking that the length of the line is indeed not greater than 80*/
        if(strchr(curr_line , '\n') == NULL) 
        {
           fprintf(stderr,"ERROR (line %d)",line_cnt);
           fprintf(stderr,"Line is too long\n");
        }   

        strcpy(temp_line, curr_line);
        first_word = strtok(temp_line, delim);
        if(first_word == NULL)
            continue;
        if(is_macro(first_word))/*if we found a macro we need to save the macro name*/
        {
            name_macr = strtok(NULL, delim);
            if(name_macr == NULL)
                continue;
           
            if(!(is_cmd(name_macr)) && (!(is_register(name_macr))))
            {
                macr_data = copy_data(filename);
                add_to_list(name_macr, macr_data, &head);
            }
            else
            {
                fprintf(stderr,"ERROR (line %d)",line_cnt);
                fprintf(stderr,"cant use reserved word as a macro name\n");
            }    
        }
        else if((curr = is_exist_macro(first_word, head)))
            fprintf(file_after_lay, "%s", curr->macro_data);
        else
            fprintf(file_after_lay, "%s", curr_line);
    }
    freeList(&head);
}


/*This method copies the data of the macro until it sees the word "endmacro".*/
char * copy_data(FILE * pointer)
{
    char * tmp_arr = "", * first_word = "";
    char curr_line[MAX_LINE_LEN] = {0};
    char tmp_line[MAX_LINE_LEN] = {0};
    const char delim[4] = " \t\n";
    /*maximum size of macro, ten lines , each line eighty one */
    tmp_arr = (char *)malloc(NUM_LINES_IN_MACRO * MAX_LINE_LEN);
    while(fgets(curr_line, MAX_LINE_LEN-1, pointer) != NULL) 
    {
        /*line_cnt++;*/
        strcpy(tmp_line,curr_line);
        first_word = strtok(tmp_line, delim);
        if(!first_word)
            continue;
        if(!strcmp(first_word, "endmacro"))
            break;
        else
            strcat(tmp_arr, curr_line);
    }
    return tmp_arr;
}

/*This method adds the macro name to the list of macros*/
void add_to_list(char * macro_name , char * macro_data, ptr * head)
{
    ptr curr = *head;
    ptr new_node;
    if(*head == NULL)/*if we are the first macro-head of the list */
    {
        curr = (ptr)malloc(sizeof(item));/*allocates the first node */
        if(curr == NULL)
        {
            fprintf(stderr,"memory allocation failed\n");
            exit(ERROR);
        }
        strcpy(curr->macro_name, macro_name);
        strcpy(curr->macro_data, macro_data);
        *head = curr;
   }
   /*we are not the first macro*/
   else
   {
        new_node = (ptr)malloc(sizeof(item));/* allocates another node.*/
        if(new_node == NULL)
        {
            fprintf(stderr,"memory allocation failed\n");
            exit(ERROR);
        }
        strcpy(new_node->macro_name, macro_name);
        strcpy(new_node->macro_data, macro_data);
        curr->next = new_node;
   }

}

/*This method receives a pointer to the list, the method releases the list.*/
void freeList(ptr * hptr)
{
    ptr temp;
    while(*hptr)
    {
        temp = *hptr;
        *hptr = (*hptr)->next;
        free(temp);
    }
}

