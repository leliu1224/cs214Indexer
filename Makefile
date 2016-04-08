all: indexer.c tokenizer.c sorted-list.c
	gcc -Wall -Werror -g -o index main.c indexer.c tokenizer.c sorted-list.c

clean:
	rm -f outputtext
	rm -f *.o
	rm -f index
