CC=gcc
FLAGS=-Wall -g -lm -lwebsockets
EXEC=sock

all: $(EXEC)

sock: test-client.o mapFunctions.o bot_yellow.o chainedListFunctions.o
	$(CC) -o $@ $^ $(FLAGS)

test-client.o: test-client.c client.h
	$(CC) -c $^ $(FLAGS)

mapFunctions.o: mapFunctions.c mapFunctions.h
	$(CC) -c $^ $(FLAGS)

bot_yellow.o: bot_yellow.c bot_yellow.h entities.h map_functions.h
	$(CC) -c $^ $(FLAGS)

bot_blue.o: bot_blue.c bot_blue.h entities.h map_functions.h
	$(CC) -c $^ $(FLAGS)

chainedListFunctions.o: chainedListFunctions.c chainedListFunctions.h
	$(CC) -c $^ $(FLAGS)

clean:
	rm -rf $(EXEC)
	rm -rf *.o *.gch
