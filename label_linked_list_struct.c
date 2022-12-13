#include "header_in_all_file.h"
#include "external_declaration.h"
#include "utils.h" 


/* This function sets the final addresses of (data/instruction labels) by a given number */
void set_final_addresses(ptr_label curr_label , int number, bool is_data)
{
    while(curr_label)
    {
        if(!(curr_label -> external) && (is_data ^ (curr_label -> in_action_statement)))
        {
            curr_label -> address += number;
        }
        curr_label = curr_label -> next;
    }
}

/* 
 * This function searches a label in the list and changes his entry field to true and returns true
 * else if the label doesn't exist return false.
 */
int make_entry(ptr_label pointer, char * label_name)
{
    ptr_label label = get_label(pointer, label_name);
    if(label != NULL)
    {
        if(label -> external)
        {
            error = ENTRY_CANT_BE_EXTERN;
            return false;
        }
        label -> entry = true;
        entry_exists = true;
        return true;
    }
    else
        error = ENTRY_LABEL_DOES_NOT_EXIST;
    return false;
}

/* This function returns the address of a given label, if the label doesn't exist return false (0).*/
unsigned int get_label_address(ptr_label pointer, char * label_name)
{
    ptr_label label = get_label(pointer, label_name);
    if(label == NULL)
        return false; 
    return label -> address;
}

/* This function check if a label is in the array and an external label is so return 1 else return 0 */
bool is_external_label(ptr_label pointer, char * label_name)
{
    ptr_label label = get_label(pointer, label_name);
    if(label == NULL)
      return false;     
    return label -> external;
}

/* This function checks if a given name is a name of a label in the list */
bool is_existing_label(ptr_label pointer, char * label_name)
{
    return get_label(pointer, label_name) != NULL;
}

/* This function checks if a given label name is in the list if so return 1 else return 0. */
ptr_label get_label(ptr_label pointer, char * label_name)
{
	while(pointer)
	{
        if(strcmp(pointer -> name , label_name) == 0)
			return pointer;
		pointer = pointer -> next;
	}
	return NULL;
}

/* This function adds a new label to the linked list of labels given its info. */
ptr_label add_label(ptr_label * hptr, char * label_name, unsigned int address, bool external, ...)
{	
	va_list p;
	ptr_label ptr = *hptr;
	ptr_label temp;

	if(is_existing_label(*hptr, label_name))
	{
		error = LABEL_ALREADY_EXISTS;
		return NULL;
	}
	temp = (ptr_label)malloc(sizeof(Labels)); 
	if(temp == NULL)/*check that the malloc succeeded*/
	{
		fprintf(stderr,"memory allocation failed\n");
        exit(ERROR);
	}

	strcpy(temp -> name, label_name);
    temp -> entry = false;
	temp -> address = address;
	temp -> external = external;

	if(!external) /* An external label can't be in an action statement */
	{
		va_start(p , external);
		temp -> in_action_statement = va_arg(p , bool);
	}
    else
        extern_exists = true;
    
    /* If the list is empty then we set the head of the list to be temp */
	if(!(*hptr))
	{	
		*hptr = temp;
		temp -> next = NULL;
		return temp;	
	}

	/* Setting a pointer to go over the list until he points on the last label and then stting temp to be 
	the new last label */
	while(ptr -> next != NULL)
        ptr = ptr->next;
        
	temp -> next = NULL;
	ptr -> next = temp;

	va_end(p);
	return temp;
}

/* 
 * This function gets a label's name, searches the list for it and remove the label.
 * If it managed to remove the label return 1 else return 0.
 */
int remove_label(ptr_label * hptr, char * label_name)
{
    ptr_label temp = *hptr;
    ptr_label prev_temp;
    while(temp) 
    {
        if(strcmp(temp -> name , label_name) == 0)
        {
            if(strcmp(temp -> name, (*hptr) -> name) == 0)
            {
                *hptr = (*hptr) -> next;
                free(temp);
            }
            else
            {
                prev_temp -> next = temp -> next;
                free(temp);
            }
            return 1;
        }
        prev_temp = temp;
        temp = temp -> next;
    }
    return 0;
}

/* This function frees the allocated memory for the symbols table*/
void free_labels(ptr_label *hptr)
{
	ptr_label temp;
	while(*hptr)
	{
		temp = *hptr;
		*hptr = (*hptr) -> next;
		free(temp);
	}
}





