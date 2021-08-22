main: main.o first_pass.o second_pass.o instructions.o labels.o errors.o utils.o encoder.o
	gcc -lm -ansi -Wall -g -pedantic first_pass.o second_pass.o instructions.o labels.o errors.o utils.o encoder.o main.o -o assembler

main.o: main.c
	gcc -c -Wall -ansi -pedantic main.c -o main.o

first_pass.o: first_pass.c first_pass.h
	gcc -c -Wall -ansi -pedantic first_pass.c -o first_pass.o

second_pass.o: second_pass.c second_pass.h
	gcc -c -Wall -ansi -pedantic second_pass.c -o second_pass.o

instructions.o: instructions.c instructions.h
	gcc -c -Wall -ansi -pedantic instructions.c -o instructions.o

labels.o: labels.c labels.h
	gcc -c -Wall -ansi -pedantic labels.c -o labels.o

errors.o: errors.c errors.h
	gcc -c -Wall -ansi -pedantic errors.c -o errors.o

utils.o: utils.c utils.h
	gcc -c -Wall -ansi -pedantic utils.c -o utils.o

encoder.o: encoder.c encoder.h
	gcc -c -Wall -ansi -pedantic encoder.c -o encoder.o

clean:
	rm *.o
