CC = clang
CFLAGS = -pthread -g -Wall -Wextra -Wpedantic -fsanitize=address 

server: main.o queue.o handler.o endpoints.o http_utils.o picohttpparser.o cJSON.o
	$(CC) $(CFLAGS) $^ -o $@

main.o: main.c main.h queue.h handler.h
	$(CC) $(CFLAGS) -c $< -o $@

queue.o: queue.c queue.h
	$(CC) $(CFLAGS) -c $< -o $@

handler.o: handler.c handler.h endpoints.h http_utils.h picohttpparser.h 
	$(CC) $(CFLAGS) -c $< -o $@

endpoints.o: endpoints.c endpoints.h cJSON.h http_utils.h
	$(CC) $(CFLAGS) -c $< -o $@

http_utils.o: http_utils.c http_utils.h picohttpparser.h
	$(CC) $(CFLAGS) -c $< -o $@

picohttpparser.o: picohttpparser.c picohttpparser.h
	$(CC) $(CFLAGS) -c $< -o $@

cJSON.o: cJSON.c cJSON.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o server

.PHONY: clean
