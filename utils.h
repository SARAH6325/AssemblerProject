#include "structs.h"
#ifndef UTILS_H
#define UTILS_H


/*------ Helper functions that are used for parsing tokens and navigating through them ------*/
char * cut_from_left(char * line);
int end_of_line(char *line);
int disregard(char *line);
char * next_token_string(char * destination, char * current_line);
char * next_list_token(char * destination, char * current_line);
void copy_token(char * destination, char * current_line);
char * next_token(char * word);


/*----------- Helper functions used to check reserved words -----------*/
int find_index(char * word, const char * arr[], int n);
int is_command(char * word);
bool is_cmd(char * word);
int is_guidance_sentence(char * word);
bool is_register(char * word);
bool is_string(char * string);
bool is_number(char * num);
bool is_valid_label(char * word , int colon);
bool is_label(char * word , int colon);
bool is_macro(char * word);
int num_operands_of_command(int type);


/*--- Helper functions that are used for creating files and assigning required extensions to them ---*/
char * create_file_name(char * rostral, int type_of_file );
FILE * open_file(char * file_name, int type_of_file ,int open_file_for);
char * decimal_to_base(unsigned int num);


/*----------- Helper functions for testing addressing methods ------------*/
bool direct_register_address(char * arg);
bool immediate_address(char * arg);
bool direct_address(char * arg);
bool address_access_to_the_record(char * arg);


/*-----------Functions of external labels positions' linked list ----------- */
ptr_external add_ext(ptr_external *hptr, char *name, unsigned int reference);
void free_ext(ptr_external *hptr);


/*----------- Functions of symbols table ----------- */
ptr_label add_label(ptr_label * hptr, char * label_name, unsigned int address, bool external, ...);
int remove_label(ptr_label * hptr, char * label_name);
void free_labels(ptr_label * hptr);
void set_final_addresses(ptr_label label, int number, bool is_data);
unsigned int get_label_address(ptr_label pointer, char * label_name);
ptr_label get_label(ptr_label pointer, char * label_name);
bool is_existing_label(ptr_label pointer, char * label_name);
bool is_external_label(ptr_label pointer, char * label_name);
int make_entry(ptr_label pointer, char * label_name);


/*----------- Functions that handle errors ----------- */
void report_errors_in_line(int line_num);
int is_error();


/*-----------Helper functions for encoding and building words ----------- */
unsigned int take_out_bits(unsigned int word, int start, int end);
void encode_instructions(unsigned int word);
unsigned int put_in_are(unsigned int info, int are);





#endif
