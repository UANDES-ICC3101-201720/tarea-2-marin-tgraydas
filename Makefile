
virtmem: main.o page_table.o disk.o program.o
	gcc main.o page_table.o disk.o program.o -o virtmem

main.o: main.c
	gcc -Wall -Werror -std=gnu11 -g -c main.c -o main.o

page_table.o: page_table.c
	gcc -Wall -Werror -std=gnu11 -g -c page_table.c -o page_table.o

disk.o: disk.c
	gcc -Wall -Werror -std=gnu11 -g -c disk.c -o disk.o

program.o: program.c
	gcc -Wall -Werror -std=gnu11 -g -c program.c -o program.o


clean:
	rm -f *.o virtmem
