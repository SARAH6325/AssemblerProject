#include "header_in_all_file.h"
#include "structs.h"

/*This function adds a node to the end of the list*/ 
ptr_external add_ext(ptr_external * hptr, char * label_name, unsigned int reference)
{
    ptr_external ptr = *hptr;
    ptr_external temp;

    temp = (ptr_external)malloc(sizeof(ext));
    if(temp == NULL)/*check that the malloc succeeded*/
    {
        fprintf(stderr,"memory allocation failed\n");
        exit(ERROR);
    }

    temp -> address = reference;
    strcpy(temp -> name, label_name);

    if(!(*hptr))
    {
        *hptr = temp;
        temp -> next = temp;
        temp -> prev = temp;
        return temp;
    }

    ((*hptr) -> prev) -> next = temp;
    temp -> next = ptr;
    temp -> prev = ptr -> prev;
    (*hptr) -> prev = temp;
    return temp;
}

/* This function frees the allocated memory for the list */
void free_ext(ptr_external * hptr)
{
    unsigned int last_reference;
    unsigned int reference;
    ptr_external ptr = *hptr;
    if(ptr) 
    {
        last_reference = ((*hptr)->prev)->address;
        reference = 0;
        do
        {
            ptr = *hptr;
            reference = ptr -> address;
            *hptr = (*hptr) -> next;
            free(ptr);
        } 
        while (reference != last_reference);
    }
}


