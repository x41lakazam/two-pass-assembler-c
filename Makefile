main: main.o first_pass.o second_pass.o instruction.o labels.o errors.o utils.o linked_list.o
	gcc -ansi -Wall -g -pedantic first_pass.o second_pass.o instruction.o labels.o errors.o utils.o linked_list.o -o main.o

second_pass.o: first_pass.c first_pass.h 
	gcc -c -Wall -ansi -pedantic first_pass.c -o first_pass.o 

second_pass.o: second_pass.c second_pass.h 
	gcc -c -Wall -ansi -pedantic second_pass.c -o second_pass.o 

instruction.o: instruction.c instruction.h 
	gcc -c -Wall -ansi -pedantic instruction.c -o instruction.o 

labels.o: labels.c labels.h 
	gcc -c -Wall -ansi -pedantic labels.c -o labels.o 

errors.o: errors.c errors.h 
	gcc -c -Wall -ansi -pedantic errors.c -o errors.o 

utils.o: utils.c utils.h 
	gcc -c -Wall -ansi -pedantic utils.c -o utils.o 

linked_list.o: linked_list.c linked_list.h 
	gcc -c -Wall -ansi -pedantic linked_list.c -o linked_list.o 

