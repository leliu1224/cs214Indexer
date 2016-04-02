all: indexer.c tokenizer.c sorted-list.c
	gcc -g -o indexer indexer.c tokenizer.c sorted-list.c

clean:
	rm -f *.o
	rm -f indexer
