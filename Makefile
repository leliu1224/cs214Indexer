all: indexer.c tokenizer.c sorted-list.c listHelper.c
	gcc -g -o indexer indexer.c tokenizer.c sorted-list.c listHelper.c

clean:
	rm -f *.o
	rm -f indexer
