#include "structs.h"
#include "declerations_of_constants.h"
#define MACHINE_RAM 2000

extern int IC, DC;
extern int error;
extern bool is_errors_found;
extern ptr_label symbols_table;
extern ptr_external ext_list;
extern const char symbols[SIZE_OF_SYMBOLS];
extern const char * commands[];
extern const char * directives[];
extern bool entry_exists, extern_exists;
extern cmd_table global_table[AMOUNT_COMANDS];
