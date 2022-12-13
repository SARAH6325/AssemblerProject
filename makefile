assembler: main.o first_pass.o label_linked_list_struct.o external_linked_list_struct.o second_pass.o utils.o macro_layout.o reserved_words.o errors_file.o first_analyze.o
	gcc -g -ansi -Wall -pedantic main.o first_pass.o external_linked_list_struct.o second_pass.o utils.o label_linked_list_struct.o macro_layout.o reserved_words.o errors_file.o first_analyze.o -lm -o assembler

main.o: main.c header_in_all_file.h structs.h declerations_of_functions.h declerations_of_constants.h external_declaration.h utils.h
	gcc -c -ansi -Wall -pedantic main.c -o main.o	
utils.o: utils.c header_in_all_file.h declerations_of_functions.h declerations_of_constants.h external_declaration.h utils.h
	gcc -c -ansi -Wall -pedantic utils.c -o utils.o
	
errors_file.o: errors_file.c declerations_of_constants.h header_in_all_file.h external_declaration.h utils.h
	gcc -c -ansi -Wall -pedantic errors_file.c -o errors_file.o

reserved_words.o: reserved_words.c utils.h declerations_of_constants.h external_declaration.h 
	gcc -c -ansi -Wall -pedantic reserved_words.c -o reserved_words.o

first_analyze.o: first_analyze.c external_declaration.h utils.h declerations_of_constants.h
	gcc -c -ansi -Wall -pedantic first_analyze.c -o first_analyze.o
    
first_pass.o: first_pass.c header_in_all_file.h external_declaration.h declerations_of_functions.h utils.h
	gcc -c -ansi -Wall -pedantic first_pass.c -o first_pass.o

label_linked_list_struct.o: label_linked_list_struct.c header_in_all_file.h external_declaration.h utils.h
	gcc -c -ansi -Wall -pedantic label_linked_list_struct.c -o label_linked_list_struct.o

second_pass.o: second_pass.c header_in_all_file.h external_declaration.h declerations_of_functions.h utils.h
	gcc -c -ansi -Wall -pedantic second_pass.c -o second_pass.o

external_linked_list_struct.o: external_linked_list_struct.c header_in_all_file.h structs.h
	gcc -c -ansi -Wall -pedantic external_linked_list_struct.c -o external_linked_list_struct.o
	
macro_layout.o: macro_layout.c utils.h declerations_of_constants.h declerations_of_functions.h header_in_all_file.h structs.h
	gcc -c -ansi -Wall -pedantic macro_layout.c -o macro_layout.o
	






