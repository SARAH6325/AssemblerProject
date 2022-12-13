#ifndef PROTOTYPES_H
#define PROTOTYPES_H
#include "structs.h"

void first_pass(FILE * file_pointer);
void second_pass(FILE * file_pointer, char * filename);

/*------------------ Macro Layout Functions ------------------*/

void freeList(ptr * hptr);
void operates_input(FILE * filename , FILE* file_after_lay);
char * copy_data(FILE * pointer);
void add_to_list(char *macro_name , char *macro_data, ptr * head);
ptr is_exist_macro( char * macro_name ,ptr head);

/*------------------ First Pass Functions ------------------*/
void reset_global_vars();
unsigned int build_first_word(int type, int is_first, int is_second, int first_method, int second_method);
int calculate_command_num_additional_words(int is_first, int is_second, int first_method, int second_method);
bool check_type_op(int type, int first_method, int second_method);
bool check_legal_num_of_opernds(int type, bool first, bool second);
int address_method(char * operand);
int handle_command(int type, char *line);
int handle_data_guidance_sentence(char *line);
int handle_guidance_sentence(int type, char *line);
int handle_extern_directive(char *line);
int handle_string_guidance_sentence(char *line);
int handle_struct_guidance_sentence(char *line);
int num_words(int method);
void operates_line_first_pass(char *line);
void write_string_to_data_image(char *str);
void write_number_to_data_image(int num);
       


/*------------------ Second Pass Functions ------------------*/

unsigned int build_register_word(bool is_dest, char *reg);
void check_operands_exist(int type, bool *is_src, bool *is_dest);
int encode_additional_words(char *src, char *dest, bool is_src, bool is_dest, int src_method, int dest_method);
void encode_additional_word(bool is_dest, int method, char *operand);
void encode_label(char *label);
int handle_command_second_pass(int type, char *line);
void operates_line_second_pass(char *line);
void write_output_entry(FILE *fp);
void write_output_extern(FILE *fp);
int write_output_files(char *original);
void write_output_object(FILE *fp);


#endif

