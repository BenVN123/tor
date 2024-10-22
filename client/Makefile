CC = clang
CFLAGS = -pthread -g -Wall -Wextra -Wpedantic -fsanitize=address

client: main.o node_status.o picohttpparser.o cJSON.o
	$(CC) $(CFLAGS) $^ -o $@

main.o: main.c main.h node_status.h
	$(CC) $(CFLAGS) -c $< -o $@

node_status.o: node_status.c node_status.h picohttpparser.h cJSON.h
	$(CC) $(CFLAGS) -c $< -o $@

picohttpparser.o: picohttpparser.c picohttpparser.h
	$(CC) $(CFLAGS) -c $< -o $@

cJSON.o: cJSON.c cJSON.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o client 

.PHONY: clean
