CC=gcc
FLAGS=-Wall -g -lm -lwebsockets
EXEC=sock

all: $(EXEC)

sock: test-client.o mapFunctions.o bot_yellow.o chainedListFunctions.o bot_blue.o
	$(CC) -o $@ $^ $(FLAGS)

test-client.o: test-client.c client.h
	$(CC) -c $^ $(FLAGS)

mapFunctions.o: mapFunctions.c mapFunctions.h
	$(CC) -c $^ $(FLAGS)

bot_yellow.o: bot_yellow.c bot_yellow.h entities.h
	$(CC) -c $^ $(FLAGS)

bot_blue.o: bot_blue.c bot_blue.h entities.h
	$(CC) -std=c99 -c $^ $(FLAGS)

chainedListFunctions.o: chainedListFunctions.c chainedListFunctions.h
	$(CC) -c $^ $(FLAGS)

clean:
	rm -rf $(EXEC)
	rm -rf *.o *.gch

multi : $(EXEC)
		gnome-terminal -e "./sock -n yellow -p 1443 192.168.130.161"
		gnome-terminal -e "./sock -n yellow -p 1443 192.168.130.161"
		gnome-terminal -e "./sock -n yellow -p 1443 192.168.130.161"
		gnome-terminal -e "./sock -n yellow -p 1443 192.168.130.161"
		gnome-terminal -e "./sock -n yellow -p 1443 192.168.130.161"
		gnome-terminal -e "./sock -n blue -p 1443 192.168.130.161"
