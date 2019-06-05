C=gcc
FLAGS=-Wall -g -lm -lwebsockets 
EXEC=sock

all: $(EXEC)

sock: test-client.c client.h
	$(CC) -o sock test-client.c client.h $(FLAGS)

clean:
	rm -rf $(EXEC)
