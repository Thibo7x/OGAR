CC=gcc
FLAGS=-Wall -g -lm -lwebsockets
EXEC=sock

all: $(EXEC)

sock: test-client.o mapFunctions.o bot_yellow.o chainedListFunctions.o bot_blue.o bot_cyan.o
	$(CC) -o $@ $^ $(FLAGS)

test-client.o: test-client.c client.h
	$(CC) -c $^ $(FLAGS)

mapFunctions.o: mapFunctions.c mapFunctions.h
	$(CC) -c $^ $(FLAGS)

bot_yellow.o: bot_yellow.c bot_yellow.h entities.h
	$(CC) -c $^ $(FLAGS)

bot_blue.o: bot_blue.c bot_blue.h entities.h
	$(CC) -std=c99 -c $^ $(FLAGS)

bot_cyan.o: bot_cyan.c bot_cyan.h entities.h
	$(CC) -c $^ $(FLAGS)

chainedListFunctions.o: chainedListFunctions.c chainedListFunctions.h
	$(CC) -c $^ $(FLAGS)

clean:
	rm -rf $(EXEC)
	rm -rf *.o *.gch

multi : $(EXEC)
		gnome-terminal -e "gdb -ex run --args sock -n yellow -p 2006 192.168.130.150"
		gnome-terminal -e "gdb -ex run --args sock -n yellow -p 2006 192.168.130.150"
		gnome-terminal -e "gdb -ex run --args sock -n yellow -p 2006 192.168.130.150"
		gnome-terminal -e "gdb -ex run --args sock -n yellow -p 2006 192.168.130.150"
		gnome-terminal -e "gdb -ex run --args sock -n yellow -p 2006 192.168.130.150"
		gnome-terminal -e "gdb -ex run --args sock -n blue -p 2006 192.168.130.150"
